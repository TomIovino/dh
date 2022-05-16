#include "erq.h"
#include "../machine.h"
#include "../config.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>

#ifdef WEXITSTATUS
typedef int wait_status_t;
#else
typedef union wait wait_status_t;
#define WEXITSTATUS(status) ((status).w_retcode)
#define WTERMSIG(status) ((status).w_termsig)
#endif
#ifndef SIGCLD
#define SIGCLD SIGCHLD
#endif

#ifndef SIGKILL
#define SIGKILL 9
#endif
#ifndef SIGTERM
#define SIGTERM 15
#endif

#ifdef RANDOM
#define randomize(n) srandom(n)
#define get_ticket() random()
#else /* RANDOM */
#ifdef DRAND48
#define randomize(n) srand48(n)
#define get_ticket() ((unsigned long)(drand48() * 4294967295.4))
#else /* DRAND48 */
#ifdef RAND
#define randomize(n) srand(n)
#define get_ticket() rand()
#else
#define randomize(n)
#define get_ticket() time(0)
#endif /* RAND */
#endif /* DRAND48 */
#endif /* RANDOM */

#ifdef TIOCNOTTY
#define DETACH
#endif

struct ticket_s {
    int32 seq;
    int32 rnd;
};

#define TICKET_SIZE sizeof(struct ticket_s)

struct socket_s {
    int32 handle;
    struct ticket_s ticket;
    struct socket_s *next;
    char type;
    int fd;
};

struct child_s {
    int32 handle;
    struct ticket_s ticket;
    struct child_s *next;
    char type;
    char status;
    pid_t pid;
    wait_status_t return_code;
    struct socket_s *fd, *err;
};

struct auth_s {
    struct socket_s s;
    int32 local_port,remote_port;
    char buf[ERQ_MAX_REPLY];
    int pos;
};

#define AUTH_PORT 113

struct retry_s {
    time_t time;
    void (*func)(char *);
    struct retry_s *next;
    char mesg[0];
};

#define CHILD_EXECUTE		1
#define CHILD_SPAWN		2

#define CHILD_RUNNING		1
#define CHILD_EXITED		2

#define SOCKET_UDP		1
#define SOCKET_LISTEN		2
#define SOCKET_CONNECTED	3
#define SOCKET_WAIT_ACCEPT	4
#define SOCKET_WAIT_CONNECT	5
#define SOCKET_STDOUT		6
#define SOCKET_STDERR		7
#define SOCKET_WAIT_AUTH	8
#define SOCKET_AUTH		9

int32 read_32(char *);
void write_32(char *, int32);
void write1(char *, int);
void die();
void bad_request(char *);
void erq_cmd();
void sig_child();
void remove_child(struct child_s *);
void read_socket(struct socket_s *, int);
struct socket_s *new_socket(int, char);
void reply_errno(int32);
void send_auth(struct auth_s *);

void erq_rlookup(char *);
void erq_execute(char *);
void erq_fork(char *);
void erq_auth(char *);
void erq_spawn(char *);
void erq_send(char *);
void erq_kill(char *);
void erq_open_udp(char *);
void erq_open_tcp(char *);
void erq_listen(char *);
void erq_accept(char *);
void erq_lookup(char *);

extern struct child_s *childs;
extern struct socket_s *sockets;
extern int seq_number, seq_interval;

#define DEBUG

#ifndef ERQ_MAX_SEND
#define ERQ_MAX_SEND 256
#endif
