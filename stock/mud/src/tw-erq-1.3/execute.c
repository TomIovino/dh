#include "defs.h"

int execute(char *buf, int buflen, char *status, int *sockets)
{
    char path[256], argbuf[1024], *p, *args[96], **argp, c;
    pid_t pid;

    status[1] = 0;
    if (buflen >= sizeof argbuf) {
	status[0] = ERQ_E_ARGLENGTH;
	return 0;
    }
    argp = &args[0];
    *argp++ = p = argbuf;
    while (--buflen >= 0) {
	c = *buf++;
	if (c == '\\') {
	    if (--buflen >= 0) {
		*p++ = *buf++;
	    } else {
		status[0] = ERQ_E_ARGFORMAT;
		return 0;
	    }
	} else if (isgraph(c)) {
	    *p++ = c;
	} else {
	    *p++ = '\0';
	    *argp++ = p;
	    if (argp == &args[sizeof args/sizeof args[0]]) {
		status[0] = ERQ_E_ARGNUMBER;
		return 0;
	    }
	    while( ( (c = *buf) == ' ' || c == '\t') && --buflen >= 0)
		buf++;
	}
    }
    *p++ = '\0';
    *argp++ = 0;
    p = args[0];
    if (p[0] == '/' || strstr(p, "..")) {
	status[0] = ERQ_E_ILLEGAL;
	return 0;
    }
    if (strlen(ERQ_DIR) + strlen(p) + 2 > sizeof(path)) {
	status[0] = ERQ_E_PATHLEN;
	return 0;
    }
    sprintf(path, "%s/%s", ERQ_DIR, p);
    if (sockets) {
	if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) {
	    perror("socketpair");
	    status[0] = ERQ_E_FORKFAIL;
	    status[1] = errno;
	    return 0;
	}
	if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockets + 2) < 0) {
	    perror("socketpair");
	    close(sockets[0]);
	    close(sockets[1]);
	    status[0] = ERQ_E_FORKFAIL;
	    status[1] = errno;
	    return 0;
	}
    }
    pid = fork();
    if (!pid) {
	close(0);
	close(1);
	if (sockets) {
	    dup2(sockets[0], 0);
	    dup2(sockets[0], 1);
	    dup2(sockets[2], 2);
	    close(sockets[0]);
	    close(sockets[1]);
	    close(sockets[2]);
	    close(sockets[3]);
	}
	execv(path, args);
	_exit(1);
    }
    if (sockets) {
	close(sockets[0]);
	close(sockets[2]);
    }
    if (pid < 0) {
	if (sockets) {
	    close(sockets[1]);
	    close(sockets[3]);
	}
	status[0] = ERQ_E_FORKFAIL;
	status[1] = errno;
	return 0;
    }
    return pid;
}

struct child_s *new_child()
{
    struct child_s *chp;
    
    chp=(struct child_s *)malloc(sizeof(struct child_s));
    chp->next=childs;
    chp->ticket.seq=(seq_number+=seq_interval);
    chp->ticket.rnd=get_ticket();
    childs=chp;
    return chp;
}

void free_child(struct child_s *chp)
{
    struct child_s **chpp;
    for (chpp=&childs; *chpp; chpp=&(*chpp)->next) {
	if (*chpp!=chp) continue;
	*chpp=chp->next;
	break;
    }
    free(chp);
}

void remove_child(struct child_s *chp)
{
    char mesg[10];

    write_32(mesg, 10);
    write_32(mesg+4, chp->handle);
    switch(chp->type) {
      case CHILD_EXECUTE: {
	if (WIFEXITED(chp->return_code)) {
	    mesg[8]=ERQ_OK;
	    mesg[9]=WEXITSTATUS(chp->return_code);
	} else if (WIFSIGNALED(chp->return_code)) {
	    mesg[8]=ERQ_SIGNALED;
	    mesg[9]=WTERMSIG(chp->return_code);
	} else {
	    mesg[8]=ERQ_E_UNKNOWN;
	}
      }
      case CHILD_SPAWN: {
	if (WIFEXITED(chp->return_code)) {
	    mesg[8]=ERQ_EXITED;
	    mesg[9]=WEXITSTATUS(chp->return_code);
	} else if (WIFSIGNALED(chp->return_code)) {
	    mesg[8]=ERQ_SIGNALED;
	    mesg[9]=WTERMSIG(chp->return_code);
	} else {
	    mesg[8]=ERQ_E_UNKNOWN;
	}
	if (chp->fd) close_socket(chp->fd);
	if (chp->err) close_socket(chp->err);
      }
    }
    write1(mesg, 10);
    free_child(chp);
}

void erq_fork(char *mesg)
{
    int msglen;
    char status[2];

    msglen=read_32(mesg);
    if (execute(&mesg[9], msglen-9, status, 0)) {
	status[0]=ERQ_OK;
    }
    write_32(mesg, 10);
    write1(mesg, 8);
    write1(status, 2);
}

void erq_execute(char *mesg)
{
    int msglen;
    char status[2];
    pid_t pid;

    msglen=read_32(mesg);
    if ((pid=execute(&mesg[9], msglen-9, status, 0))) {
	struct child_s *chp;
	chp=new_child();
	chp->pid=pid;
	chp->type=CHILD_EXECUTE;
	chp->status=CHILD_RUNNING;
	chp->handle=read_32(mesg+4);
	return;
    }
    write_32(mesg, 10);
    write1(mesg, 8);
    write1(status, 2);
}

void erq_spawn(char *mesg)
{
    int msglen, sockets[4];
    char status[2];
    pid_t pid;
    struct ticket_s ticket;

    msglen=read_32(mesg);
    if ((pid=execute(&mesg[9], msglen-9, status, sockets))) {
	struct child_s *chp;
	chp=new_child();
	chp->pid=pid;
	chp->type=CHILD_SPAWN;
	chp->status=CHILD_RUNNING;
	chp->handle=read_32(mesg+4);
	chp->fd=new_socket(sockets[1], SOCKET_STDOUT);
	memcpy(&chp->fd->ticket, &chp->ticket, TICKET_SIZE);
	fcntl(sockets[1], F_SETFD, 1);
	fcntl(sockets[1], F_SETFL, O_NONBLOCK);
	chp->err=new_socket(sockets[3], SOCKET_STDERR);
	fcntl(sockets[3], F_SETFD, 1);
	fcntl(sockets[3], F_SETFL, O_NONBLOCK);
	chp->fd->handle=chp->handle;
	write_32(mesg, (msglen=13+TICKET_SIZE));
	write_32(mesg+4, ERQ_HANDLE_KEEP_HANDLE);
	write_32(mesg+8, chp->handle);
	mesg[12]=ERQ_OK;
	memcpy(mesg+13, (char *)&chp->ticket, TICKET_SIZE);
	write1(mesg, msglen);	
	return;
    }
    write_32(mesg, 10);
    write1(mesg, 8);
    write1(status, 2);
}

void erq_kill(char *mesg)
{
    struct ticket_s *ticket;
    struct child_s *chp;
    struct socket_s *sp;
    int msglen, sig;

    msglen=read_32(mesg);
    switch(msglen-TICKET_SIZE) {
      case 9:
	sig=SIGKILL;
	break;
      case 13:
	sig=read_32(mesg+9+TICKET_SIZE);
	break;
      default:
	return bad_request(mesg);
    }

    ticket=(struct ticket_s *) (mesg+9);

    for (chp=childs; chp; chp=chp->next)
        if (!memcmp(ticket, &chp->ticket, TICKET_SIZE)) break;

    if (chp) {
        if (sig >= 0) sig=kill(chp->pid, sig);
	write_32(mesg, 9);
	mesg[8]=sig < 0 ? ERQ_E_ILLEGAL : ERQ_OK;
	write1(mesg, 9);
	return;
    }
    
    for (sp=sockets; sp; sp=sp->next)
	if (!memcmp(ticket, &sp->ticket, TICKET_SIZE)) break;

    if (sp) {
	close_socket(sp);
	mesg[8]=ERQ_OK;
	write_32(mesg, 9);
	write1(mesg, 9);
	return;
    }

    write_32(mesg, 9);
    mesg[8]=ERQ_E_TICKET;
    write1(mesg, 9);
    return;
}
