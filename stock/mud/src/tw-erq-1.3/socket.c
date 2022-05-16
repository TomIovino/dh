#include "defs.h"

struct socket_s *new_socket(int fd, char type)
{
    struct socket_s *sp;

    sp=(struct socket_s *)malloc(sizeof(struct socket_s));
    sp->next=sockets;
    sp->fd=fd;
    sp->type=type;
    sp->ticket.seq=(seq_number+=seq_interval);
    sp->ticket.rnd=get_ticket();
    sockets=sp;
    return sp;
}

void free_socket(struct socket_s *sp)
{
    struct socket_s **spp;
    for (spp=&sockets; *spp; spp=&(*spp)->next) {
	if (*spp!=sp) continue;
	*spp=sp->next;
	break;
    }
    free(sp);
}

void close_socket(struct socket_s *sp)
{
    struct child_s *chp;
    switch(sp->type) {
      case SOCKET_STDOUT: {
	for (chp=childs; chp; chp=chp->next)
	    if (chp->fd == sp) break;
	if (chp) chp->fd=0;
	break;
      }
      case SOCKET_STDERR: {
	for (chp=childs; chp; chp=chp->next)
	    if (chp->err == sp) break;
	if (chp) chp->err=0;
	break;
      }
      case SOCKET_WAIT_AUTH: 
      case SOCKET_AUTH: {
	struct auth_s *ap=(struct auth_s *)sp;
	write_32(ap->buf, ap->pos);
	write_32((ap->buf)+4, ap->s.handle);
	write1(ap->buf, ap->pos);
	break;
      }
      default: {
	char mesg[10];
	write_32(mesg, 10);
	write_32(mesg+4, sp->handle);
	mesg[8]=ERQ_EXITED;
	write1(mesg, 10);
      }
    }
    close(sp->fd);
    free_socket(sp);
}    

void read_socket(struct socket_s *sp, int rw)
{
    char buf[ERQ_MAX_REPLY];
    int num, len;
    
    switch(sp->type) {
      case SOCKET_LISTEN: {
	write_32(buf, 13);
	write_32(buf+4, ERQ_HANDLE_KEEP_HANDLE);
	write_32(buf+8, sp->handle);
	buf[12]=ERQ_STDOUT;
	write1(buf, 13);
	sp->type=SOCKET_WAIT_ACCEPT;
	return;
      }
      case SOCKET_WAIT_ACCEPT:
	return;
      case SOCKET_UDP: {
	struct sockaddr_in addr;
	int len;
	do {
	  len=sizeof(addr);
	  num=recvfrom(sp->fd, buf+19, ERQ_MAX_REPLY-20, 0,
	    (struct sockaddr *)&addr, &len);
	  if (num<0 && errno==EINTR) continue;
	  if (num<=0) break;
	  num+=19;
	  write_32(buf, num);
	  write_32(buf+4, ERQ_HANDLE_KEEP_HANDLE);
	  write_32(buf+8, sp->handle);
	  buf[12]=ERQ_STDOUT;
	  memcpy(buf+13, &addr.sin_addr.s_addr, 4);
	  memcpy(buf+17, &addr.sin_port, 2);
	  write1(buf, num);
	} while(1);
	break;
      }
      case SOCKET_WAIT_CONNECT: {
	if (rw) {
	    sp->type=SOCKET_CONNECTED;
	    write_32(buf, 13+TICKET_SIZE);
	    write_32(buf+4, ERQ_HANDLE_KEEP_HANDLE);
	    write_32(buf+8, sp->handle);
	    buf[12]=ERQ_OK;
	    memcpy(&sp->ticket, buf+13, TICKET_SIZE);
	    write1(buf, 13+TICKET_SIZE);
	    return;
	}
	/* fall through */
      }
      case SOCKET_STDOUT:
      case SOCKET_STDERR:
      case SOCKET_CONNECTED: {
	do{
	  num=read(sp->fd, buf+13, ERQ_MAX_REPLY-14);
	  if (num<0 && errno==EINTR) continue;
	  if (num<=0) break;
	  num+=13;
	  write_32(buf, num);
	  write_32(buf+4, ERQ_HANDLE_KEEP_HANDLE);
	  write_32(buf+8, sp->handle);
	  buf[12]=sp->type==SOCKET_STDERR ? ERQ_STDERR : ERQ_STDOUT;
	  write1(buf, num);
	} while(1);
	break;
      }
      case SOCKET_WAIT_AUTH: {
        if (rw) {
	    sp->type=SOCKET_AUTH;
	    send_auth((struct auth_s *)sp);
	    return;
	}
	/* fall through */
      }
      case SOCKET_AUTH: {
        struct auth_s *ap=(struct auth_s *)sp;
	int pos;
	do {
	  num=read(ap->s.fd, &ap->buf[ap->pos], sizeof(ap->buf)-ap->pos);
	  if (num<0 && errno==EINTR) continue;
	  if (num<=0) {
	    close_socket(sp);
	    return;
	  }
	  ap->pos+=num;
	} while(1);
      }
    }
    if (!num) {
	close_socket(sp);
	return;
    }
    if (errno!=EWOULDBLOCK) {
	reply_errno(sp->handle);
	close_socket(sp);
    }
}

void erq_send(char *mesg)
{
    struct ticket_s *ticket;
    struct socket_s *sp;
    int msglen, num, type, status;
    char *data;

    msglen=read_32(mesg);
    if (msglen < 9+TICKET_SIZE) {
	mesg[8]=ERQ_E_ARGLENGTH;
	write_32(mesg, 9);
	write1(mesg, 9);
	return;
    }

    ticket=(struct ticket_s *) (mesg+9);
    data=mesg+9+TICKET_SIZE;
    msglen-=(TICKET_SIZE+9);

    for (sp=sockets; sp; sp=sp->next)
        if (!memcmp(ticket, &sp->ticket, TICKET_SIZE)) break;

    if (!sp) {
	write_32(mesg, 9);
	mesg[8]=ERQ_E_TICKET;
	write1(mesg, 9);
	return;
    }
    switch(sp->type) {
      case SOCKET_STDOUT:
      case SOCKET_CONNECTED: {
	do
	  num=write(sp->fd, data, msglen);
	while (num==-1 && errno==EINTR);
	break;
      }
      case SOCKET_UDP: {
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	memcpy(&addr.sin_addr.s_addr, data, 4);
	memcpy(&addr.sin_port, data+4, 2);
	data+=6;
	msglen-=6;
	do 
	  num=sendto(sp->fd, data, msglen, 0, (struct sockaddr *)&addr,
	      sizeof(addr));
	while(num==-1 && errno==EINTR);
	break;
      }
      case SOCKET_WAIT_ACCEPT: {
	erq_accept(mesg);
	return;
      }
      default:
	write_32(mesg, 9);
	mesg[8]=ERQ_E_TICKET;
	write1(mesg, 9);
	return;
    }
    write_32(mesg, 10);
    if (num==msglen) {
	write_32(mesg, 10);
	mesg[8]=ERQ_OK;
	write1(mesg, 10);
	return;
    } else if (num>=0) {
	write_32(mesg, 13);
	mesg[8]=ERQ_E_INCOMPLETE;
	write_32(mesg+9, num);
	write1(mesg, 13);
	return;
    }
    reply_errno(sp->handle);
    if (errno!=EWOULDBLOCK) close_socket(sp);
}

void erq_listen(char *mesg)
{
    struct ticket_s ticket;
    struct sockaddr_in addr;
    struct socket_s *sp;
    int fd, msglen, tmp;

    msglen=read_32(mesg);
    if (msglen != 11) {
	mesg[8]=ERQ_E_ARGLENGTH;
	mesg[9]=0;
	write_32(mesg, 10);
	write1(mesg, 10);
	return;
    }

#ifdef MAX_CHILDS
    if (num_childs >= MAX_CHILDS) {
	mesg[8]=ERQ_E_NSLOTS;
	mesg[9]=MAX_CHILDS;
	write_32(mesg, 10);
	write1(mesg, 10);
	return;
    }
#endif

    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_family=AF_INET;
    memcpy(&addr.sin_port, mesg + 9, 2);

    tmp=((fd=socket(AF_INET, SOCK_STREAM, 0)) < 0);
    tmp=tmp || (fcntl(fd, F_SETFD, 1) < 0);
    tmp=tmp || (fcntl(fd, F_SETFL, O_NONBLOCK) < 0);
    tmp=tmp || (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&msglen,
	    sizeof(msglen)) < 0);
    tmp=tmp || (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0);
    tmp=tmp || (listen(fd, 5) < 0);
    if (tmp)
/*
    if ((fd=socket(AF_INET, SOCK_STREAM, 0)) < 0 ||
	fcntl(fd, F_SETFD, 1) < 0 ||
	fcntl(fd, F_SETFL, O_NONBLOCK) < 0 ||
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&tmp,
	    sizeof(tmp)) < 0 ||
	bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0 ||
	listen(fd, 5) < 0)
*/
    {
	if (fd >= 0) close(fd);
	return reply_errno(read_32(mesg+4));
    }

    sp=new_socket(fd, SOCKET_LISTEN);
    sp->handle=read_32(mesg + 4);
    write_32(mesg, (msglen=13 + TICKET_SIZE));
    write_32(mesg + 4, ERQ_HANDLE_KEEP_HANDLE);
    write_32(mesg + 8, sp->handle);
    mesg[12]=ERQ_OK;
    memcpy(mesg + 13, (char *) &sp->ticket, TICKET_SIZE);
    write1(mesg, msglen);
    return;
}

void erq_open_udp(char *mesg)
{
    struct ticket_s ticket;
    struct sockaddr_in addr;
    struct socket_s *sp;
    int fd, msglen, tmp;

    msglen=read_32(mesg);
    
    if (msglen != 11) {
	mesg[8]=ERQ_E_ARGLENGTH;
	mesg[9]=0;
	write_32(mesg, 10);
	write1(mesg, 10);
	return;
    }
    
#ifdef MAX_CHILDS
    if (num_childs >= MAX_CHILDS) {
	mesg[8]=ERQ_E_NSLOTS;
	mesg[9]=MAX_CHILDS;
	write_32(mesg, 10);
	write1(mesg, 10);
	return;
    }
#endif

    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_family=AF_INET;
    memcpy(&addr.sin_port, mesg + 9, 2);
    
    if ((fd=socket(AF_INET, SOCK_DGRAM, 0)) < 0 ||
	fcntl(fd, F_SETFD, 1) < 0 ||
	fcntl(fd, F_SETFL, O_NONBLOCK) < 0 ||
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&tmp,
	    sizeof(tmp)) < 0 ||
	bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
	if (fd >= 0) close(fd);
	return reply_errno(read_32(mesg+4));
    }

    sp=new_socket(fd, SOCKET_UDP);
    sp->handle=read_32(mesg + 4);
    write_32(mesg, (msglen=13 + TICKET_SIZE));
    write_32(mesg + 4, ERQ_HANDLE_KEEP_HANDLE);
    write_32(mesg + 8, sp->handle);
    mesg[12]=ERQ_OK;
    memcpy(mesg + 13, (char *) &sp->ticket, TICKET_SIZE);
    write1(mesg, msglen);
    return;
}

void erq_open_tcp(char *mesg)
{
    struct ticket_s ticket;
    struct sockaddr_in addr;
    struct socket_s *sp;
    int fd, msglen, tmp, status;

    msglen=read_32(mesg);
    if (msglen != 15) {
	mesg[8]=ERQ_E_ARGLENGTH;
	mesg[9]=0;
	write_32(mesg, 10);
	write1(mesg, 10);
	return;
    }
    
#ifdef MAX_CHILDS
    if (num_childs >= MAX_CHILDS) {
	mesg[8]=ERQ_E_NSLOTS;
	mesg[9]=MAX_CHILDS;
	write_32(mesg, 10);
	write1(mesg, 10);
	return;
    }
#endif

    memcpy(&addr.sin_addr.s_addr, mesg + 9, 4);
    addr.sin_family=AF_INET;
    memcpy(&addr.sin_port, mesg + 13, 2);
    status=SOCKET_CONNECTED;

    if ((fd=socket(AF_INET, SOCK_STREAM, 0)) < 0 ||
	fcntl(fd, F_SETFD, 1) < 0 ||
	fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
    {
	if (fd >= 0) close(fd);
	return reply_errno(read_32(mesg+4));
    }
    do 
	tmp=connect(fd, (struct sockaddr *) &addr, sizeof(addr));
    while (tmp && errno==EINTR);
    if (!tmp) status=SOCKET_CONNECTED;
    else if (errno==EINPROGRESS) {
	status=SOCKET_WAIT_CONNECT;
	tmp=0;
    } else {
	if (fd >= 0) close(fd);
	return reply_errno(read_32(mesg+4));
    }

    sp=new_socket(fd, status);
    sp->handle=read_32(mesg + 4);
    if (status==SOCKET_WAIT_CONNECT) return;
    write_32(mesg, (msglen=13 + TICKET_SIZE));
    write_32(mesg + 4, ERQ_HANDLE_KEEP_HANDLE);
    write_32(mesg + 8, sp->handle);
    mesg[12]=ERQ_OK;
    memcpy(mesg + 13, (char *) &sp->ticket, TICKET_SIZE);
    write1(mesg, msglen);
    return;
}

void erq_accept(char *mesg)
{
    struct ticket_s ticket;
    struct sockaddr_in addr;
    struct socket_s *sp;
    int fd, msglen, tmp, status;

    msglen=read_32(mesg);
    if (msglen != 9+TICKET_SIZE) {
	mesg[8]=ERQ_E_ARGLENGTH;
	mesg[9]=0;
	write_32(mesg, 10);
	write1(mesg, 10);
	return;
    }
    
#ifdef MAX_CHILDS
    if (num_childs >= MAX_CHILDS) {
	mesg[8]=ERQ_E_NSLOTS;
	mesg[9]=MAX_CHILDS;
	write_32(mesg, 10);
	write1(mesg, 10);
	return;
    }
#endif

    for (sp=sockets; sp; sp=sp->next)
        if (!memcmp(mesg+9, &sp->ticket, TICKET_SIZE)) break;

    if (sp->type != SOCKET_WAIT_ACCEPT) {
	write_32(mesg, 9);
	mesg[8]=ERQ_E_TICKET;
	write1(mesg, 9);
	return;
    }
    sp->type=SOCKET_LISTEN;

    tmp=sizeof(addr);
    if ((fd=accept(sp->fd, (struct sockaddr *)&addr, &tmp)) < 0 ||
	fcntl(fd, F_SETFD, 1) < 0 ||
	fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
    {
	if (fd >= 0) close(fd);
	return reply_errno(read_32(mesg+4));
    }

    sp=new_socket(fd, SOCKET_CONNECTED);
    sp->handle=read_32(mesg + 4);
    write_32(mesg, (msglen=19 + TICKET_SIZE));
    write_32(mesg + 4, ERQ_HANDLE_KEEP_HANDLE);
    write_32(mesg + 8, sp->handle);
    mesg[12]=ERQ_OK;
    memcpy(mesg + 13, (char *)&addr.sin_addr.s_addr, 4);
    memcpy(mesg + 17, (char *)&addr.sin_port, 2);
    memcpy(mesg + 19, (char *)&sp->ticket, TICKET_SIZE);
    write1(mesg, msglen);
    return;
}

void erq_auth(char *mesg)
{
    struct sockaddr_in addr;
    struct socket_s *sp;
    struct auth_s *ap;
    int32 host;
    int fd, msglen, tmp, status, local_port, remote_port;

#ifdef MAX_CHILDS
    if (num_childs >= MAX_CHILDS) {
	mesg[8]=ERQ_E_NSLOTS;
	mesg[9]=MAX_CHILDS;
	write_32(mesg, 10);
	write1(mesg, 10);
	return;
    }
#endif

    msglen=read_32(mesg);
    switch(msglen) {
      case 17: { /* new way */
        remote_port=ntohs(*(unsigned short *)(mesg+13));
	local_port=ntohs(*(unsigned short *)(mesg+15));
	
	memcpy(&addr.sin_addr.s_addr, mesg+9, 4);
	addr.sin_family=AF_INET;
	addr.sin_port=htons(AUTH_PORT);
	break;
      }
      case 13+sizeof(addr): {
	memcpy(&addr, mesg+9, sizeof(addr));
	remote_port=ntohs(addr.sin_port);
	local_port=read_32(mesg+sizeof(addr)+9);
	addr.sin_port=htons(AUTH_PORT);
	break;
      }
      default: {
        write_32(mesg, 8);
	write1(mesg, 8);
	return;
      }
    }

    if ((fd=socket(AF_INET, SOCK_STREAM, 0))<0 ||
	fcntl(fd, F_SETFD, 1)<0 ||
	fcntl(fd, F_SETFL, O_NONBLOCK)<0)
    {
	if (fd >= 0) close(fd);
	write_32(mesg, 8);
	write1(mesg, 8);
	return;
    }
    do
	tmp=connect(fd, (struct sockaddr *) &addr, sizeof(addr));
    while(tmp && errno==EINTR);
    if (!tmp) status=SOCKET_AUTH;
    else if (errno==EINPROGRESS) status=SOCKET_WAIT_AUTH;
    else {
	tmp=errno;
	if (fd >= 0) close(fd);
	write_32(mesg, 8);
	write1(mesg, 8);
	return;
    }

    ap=(struct auth_s *)malloc(sizeof(struct auth_s));
    ap->s.next=sockets;
    ap->s.fd=fd;
    ap->s.type=status;
    ap->s.ticket.seq=(seq_number+=seq_number);
    ap->s.ticket.rnd=get_ticket();
    ap->s.handle=read_32(mesg+4);
    ap->local_port=local_port;
    ap->remote_port=remote_port;
    ap->pos=8;
    sockets=(struct socket_s *)ap;

    if (status==SOCKET_AUTH) send_auth(ap);
    return;
}

void send_auth(struct auth_s *ap)
{
    char req[100];
    int num;

    sprintf(req, "%ld,%ld\n", ap->remote_port, ap->local_port);
    do
	num=write(ap->s.fd, req, strlen(req));
    while(num==-1 && errno==EINTR);
    if (num<=0) close_socket((struct socket_s *)ap);
}

