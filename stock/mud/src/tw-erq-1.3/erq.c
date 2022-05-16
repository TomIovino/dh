#include "defs.h"

struct child_s *childs;
struct socket_s *sockets;
struct retry_s *retries;
void erq_cmd();
void sig_child();

int in_select, seq_number, seq_interval;

void main(int argc, char *argv[])
{
    int num;

    if (argc > 1 && !strcmp(argv[1], "--forked")) {
	write1("1", 1);
    } else {
	fprintf(stderr, "dynamic attatchment unimplemented\n");
	die();
    }
    in_select=0;
    signal(SIGCLD, sig_child);
    signal(SIGPIPE, SIG_IGN);

    childs=0;
    retries=0;
    randomize(time(0));
    seq_number=get_ticket();
    seq_interval=get_ticket() | 1; /* make sure it is odd */

#ifdef DETACH
    num = open("/dev/tty", O_RDWR);
    if (num >= 0) {
	ioctl(num, TIOCNOTTY, 0);
	close(num);
    }
#endif
    while(1) {
	fd_set read_fds, write_fds;
	int num_fds, fd;
	struct child_s *chp, **chpp;
	struct retry_s *rtp, **rtpp;
	struct socket_s *sp;
	struct timeval timeout;

	/* look for sockets */
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_SET(0, &read_fds);
	num_fds=2;
	for (sp=sockets; sp; sp=sp->next) {
	    switch(sp->type) {
	      case SOCKET_WAIT_CONNECT:
	      case SOCKET_WAIT_AUTH:
		FD_SET(sp->fd, &write_fds);
	      default:
		FD_SET(sp->fd, &read_fds);
		if (sp->fd > num_fds) num_fds=sp->fd+1;
	      case SOCKET_WAIT_ACCEPT:
		/* do nothing */
	    }
	}
	for (chpp=&childs; *chpp;) {
	    chp=*chpp;
	    if (chp->status==CHILD_EXITED) {
		*chpp=chp->next;
		remove_child(chp);
	    } else {
		chpp=&chp->next;
	    }
	}

	if (retries) {
	    time_t t;
	    
	    t=retries->time;
	    for (rtp=retries; rtp; rtp=rtp->next) {
		if (rtp->time < t) t=rtp->time;
	    }
	    timeout.tv_sec=t-time(0);
	    timeout.tv_usec=0;
	}

#ifdef DEBUG
	fprintf(stderr, "Starting select...\n");
#endif
	in_select=1; /* so sig_child() can write reply directly */
	num=select(num_fds, &read_fds, &write_fds, 0, retries ? &timeout : 0);
	in_select=0; /* don't wnat sig_child() writing now */

#ifdef DEBUG
	fprintf(stderr, "Select returns %d\n", num);
	if (num<0) fprintf(stderr, " errno=%d.\n", errno);
#endif
		
	/* check for retries */
	for (rtpp=&retries; *rtpp; ) {
	    rtp=*rtpp;
	    if (rtp->time <= time(0)) {
		(*(rtp->func))(rtp->mesg);
		*rtpp=rtp->next;
		free(rtp);
	    } else {
		rtpp=&rtp->next;
	    }
	}
	
	/* check for input from driver */
	if (FD_ISSET(0, &read_fds)) erq_cmd();

	/* check sockets */

	for (sp=sockets; sp; sp=sp->next) {
	  if (FD_ISSET(sp->fd, &read_fds)) read_socket(sp, 0);
	  if (FD_ISSET(sp->fd, &write_fds)) read_socket(sp, 1);
	}
    }
}

#define ERQ_REQUEST_MAX ERQ_LOOKUP

void (*erq_table[])(char *)={
    erq_rlookup,
    erq_execute,
    erq_fork,
    erq_auth,
    erq_spawn,
    erq_send,
    erq_kill,
    erq_open_udp,
    erq_open_tcp,
    erq_listen,
    erq_accept,
    erq_lookup
};

void erq_cmd()
{
    char request;
    static char buf[ERQ_MAX_SEND];
    static int pos=0;
    int len, mesg_len;

    if (pos < 9) {
	len=read(0, buf+pos, 9-pos);
	if (len <= 0) {
	    perror("read");
	    die();
	}
	pos+=len;
	if (pos < 9) return;
    }

    mesg_len=read_32(buf);

    if (pos < mesg_len) {
	len=read(0, buf+pos, mesg_len-pos);
	if (len <= 0) {
	    perror("read");
	    die();
	}
	pos+=len;
	if (pos < mesg_len) return;
    }
    pos=0;
    request=buf[8];
    if (request<=ERQ_REQUEST_MAX) {
#ifdef DEBUG
	char *mesg, *mesgs[]={
	    "rlookup","fork","auth","execute","spawn","send","kill",
	    "open_udp","open_tcp","listen","accept","lookup"};
	mesg=mesgs[request];
	fprintf(stderr, "command: %s\n", mesg);
#endif
	(*erq_table[request])(buf);
    } else bad_request(buf);
}

void die()
{
    fprintf(stderr, "Erq demon exiting.\n");
    exit(1);
}

void sig_child()
{
    wait_status_t status;
    pid_t pid;
    struct child_s *chp;

    pid=wait(&status);
#ifdef DEBUG
    fprintf(stderr, "Erq: sigchild called, pid=%d status=%d\n", pid, status);
#endif
    for (chp=childs; chp; chp=chp->next) {
	if (chp->pid!=pid) continue;
	chp->status=CHILD_EXITED;
	chp->return_code=status;
	if (in_select) remove_child(chp); /* safe to do it from here */
	/*  if we're in select, we know we're not going to be messing up
	    the main loop with stuff we're doing here */
	break;
    }
    if (!chp) fprintf(stderr,
	"Caught SIGCLD for pid %d, not in child list.\n", pid);
    signal(SIGCLD, sig_child);
}

add_retry(void (*func)(char *), char *mesg, int len, int t)
{
    struct retry_s *retry;

    retry=(struct retry_s *)malloc(sizeof(struct retry_s)+len);
    retry->time=time(NULL)+t;
    retry->func=func;
    memcpy(&retry->mesg, mesg, len);
    retry->next=retries;
    retries=retry;
}

int32 read_32(char *a)
{
    return ntohl(*(int32 *)a);
}

void write_32(char *a, int32 i)
{
    *(int32*)a=htonl(i);
}

void writen(int n, char *mesg, long len)
{
    long l;
    do
	l=write(n,mesg,len);
    while(l==-1 && errno==EINTR);
    if (l!=len) {
	fprintf(stderr, "wrote %ld, should be %ld", l, len);
	die();
    }
}

void write1(char *m, int l)
{
    return writen(1, m, l);
}

void bad_request(char *mesg)
{
    fprintf(stderr, "Bad request %d\n", mesg[8]);
    fprintf(stderr, "%x %x %x %x %x %x %x %x %x\n",
	mesg[0], mesg[1], mesg[2], mesg[3], mesg[4],
	mesg[5], mesg[6], mesg[7], mesg[8]);
    fprintf(stderr, "%c %c %c %c %c %c %c %c %c\n",
	mesg[0], mesg[1], mesg[2], mesg[3], mesg[4],
	mesg[5], mesg[6], mesg[7], mesg[8]);
    write_32(mesg, 8);
    write1(mesg, 8);
    return;
}

void reply_errno(int32 handle)
{
    char mesg[10];
    write_32(mesg, 10);
    write_32(mesg+4, handle);
    switch(errno) {
      case EWOULDBLOCK:
#if EAGAIN != EWOULDBLOCK
	case EAGAIN:
#endif
	  mesg[8]=ERQ_E_WOULDBLOCK;
	  break;
	case EPIPE:
	  mesg[8]=ERQ_E_PIPE;
	  break;
	default:
	  mesg[8]=ERQ_E_UNKNOWN;
	  break;
    }
    mesg[9]=errno;
    write1(mesg, 10);
    return;
}

void erq_rlookup(char *mesg)
{
    char reply[12];
    struct hostent *hp;
    int len;
    
    hp=gethostbyaddr(mesg+9, 4, AF_INET);
    if (!hp && mesg[8]==ERQ_RLOOKUP) {
	mesg[8]++;
	add_retry(erq_rlookup, mesg, 13, 5);
	return;
    }
    if (hp) len=strlen(hp->h_name)+1;
    else len=0;
    
    write_32(reply, len+12);
    memcpy(reply+4, mesg+4, 4);
    memcpy(reply+8, mesg+9, 4);
    write1(reply, 12);
    if (hp) write1((char *)hp->h_name, len);
}

void erq_lookup(char *mesg)
{
    char reply[13];
    struct hostent *hp;
    int len;

    len=read_32(mesg);
    if (mesg[len-1]!=0) {
	mesg[len]=0;
	len++;
    }
    hp=gethostbyname(mesg+9);
    if (!hp && mesg[8]==ERQ_LOOKUP) {
	mesg[8]++;
	add_retry(erq_lookup, mesg, len, 5);
	return;
    }
    write_32(reply, len+4);
    memcpy(reply+4, mesg+4, 4);
    if (hp) {
	reply[8]=ERQ_OK;
	memcpy(reply+9, hp->h_addr, 4);
    } else {
	reply[8]=ERQ_E_NOTFOUND;
	write_32(reply+9, 0);
    }
    write1(reply, 13);
    write1(mesg+9, len-9);
}

