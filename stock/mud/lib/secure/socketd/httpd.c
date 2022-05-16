#include "socket.h"

#if 1
#define debug(x) if (find_player("geewhiz")) \
            tell_object(find_player("geewhiz"), (x))
#else
#define debug(x)
#endif

#define WWW_PATH "secure/socketd/www"
#define ERROR "<PLAINTEXT>Error\n"

int sock;

static string parse_request(string file);
static void listen_call(int fd, int act, mixed a, mixed b);
static void callback(int fd, int act, mixed a, mixed b);

void reset(int arg)
{
    if (arg) return;
    sock=SOCKETD->socket_listen(8889, #'listen_call);
    debug("httpd started.\n");
}

static void listen_call(int fd, int act, mixed a, mixed b)
{
    debug(sprintf("listen_call(%d,%d,%O,%O)\n",fd,act,a,b));
    switch(act) {
      case SOCKET_READY:
debug("Socket ready.\n");
    return;
      case SOCKET_ACCEPT:
debug("Socket accept.\n");

    SOCKETD->socket_accept(fd, #'callback);
    return;
      case SOCKET_ERROR:
debug("Socket Error.\n");

    sock=-1;
      case SOCKET_CLOSE:
debug("Socket close\n");
    destruct(this_object());
    return;
    }
}


static void callback(int fd, int act, mixed a, mixed b)
{
    debug(sprintf("callback(%d,%d,%O,%O)\n",fd,act,a,b));
    switch(act) {
      case SOCKET_READY:
        debug("cb: Socket Ready\n");
        break;
      case SOCKET_READ: {
        string file, *tmp;
        debug("cb: Socket Read\n");
        tmp=explode(a, "\r\n");
debug(sprintf("tmp = %O\n",tmp));
    tmp=explode(tmp[0], " ");
debug(sprintf("tmp = %O\n",tmp));
    if (tmp[0]!="GET") {
      if(SOCKETD->socket_close(fd)) 
          debug("Socket close successful\n");
        return;
    }
    file=tmp[1];
    a=to_array(parse_request(file));
    debug(sprintf("cb: Calling socket_write.. sizeof(a) = %O\n",sizeof(a)));
    SOCKETD->socket_write(fd, a);
        debug("cb: Calling Socket Close\n");
    if(SOCKETD->socket_close(fd)) 
        debug("Socket close successful\n");
    return;
      }
      case SOCKET_ERROR: {
        debug("cb: Socket Error\n");
    if(SOCKETD->socket_close(fd)) 
        debug("Socket close successful\n");
      }
      case SOCKET_CLOSE:
        debug("cb: Socket Close\n");
    }
}

static string parse_request(string file)
{
    string data, tmp, *args;

    args=explode(file, "/")-({""});
    debug(sprintf("parse_request(%O)\nargs = %O\n",file,args));
    if (!sizeof(args)) return ERROR;

    file=sprintf("%s/%s.c", WWW_PATH, args[0]);
debug(sprintf("file = %O\nfile_size(%O) = %d\n",file,file,file_size(file)));

    if (file_size(file) > 0) {
    if (tmp=catch(data=call_other(file, "www_main", args[1..]))) {
        debug(sprintf("tmp = %O\n",tmp));
        return ERROR;
    }
       debug(sprintf("data = %O\n",data));
       return data;
    }
    return ERROR;
}

void destructor()
{
    if (sock<0) return;
    SOCKETD->socket_close(sock);
}
