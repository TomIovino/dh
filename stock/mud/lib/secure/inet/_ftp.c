#include <udp.h>
#define DBG(X) if(find_player("geewhiz")) tell_object(find_player("geewhiz"),sprintf("%O\n",(X)))
#undef DBG
#define DBG(X)
#define SUCCESS (printf("inetd:  Request transmitted.\n"),1)
#define ERR(X)  (notify_fail(X),0)
 
#define FTP_ROOT_DIR        "/ftp/"
#define MAX_READ_LINES        1000
#define COMMAND                "cmd"
#define LOCAL                "local"
#define REMOTE                "remote"
#define USER                "user"
 
#define MUD                0
#define ACTION                1
#define ARG1                2
#define ARG2                3
 
int valid_file(string file) {
    return stringp(file) && member_array("..", explode(file, "/")) == -1;
}
 
status main(string str) {
    string *arg;
    string tmp, command;
    mapping data;
    int i;
 
 
    notify_fail("ftp: Syntax:  ftp <mudname> <action> [<args...>]\n"
                "\tValid Actions:\n"
                "\t\tls [<directory>]\n"
                "\t\tdir [<directory>]\n"
                "\t\tget <remotefile> [<localfile>]\n"
                "\t\tput <localfile> [<remotefile>]\n");
    if (!str) return 0;
    if (sizeof(arg = explode(str, " ")) < 2) return 0;
    data = ([ ]);
    switch(arg[ACTION]) {
        case "ls":
        case "dir":
            data[REMOTE] = (sizeof(arg) > ARG1 ? arg[ARG1] : "");
            command = "dir";
            break;
        case "get":
            if (sizeof(arg) < 3) return 0;
            command = "get";
            data[REMOTE] = arg[ARG1];
            if (sizeof(arg) > ARG2)
                data[LOCAL] = arg[ARG2];
            else {
                data[LOCAL] = explode(arg[ARG1], "/");
                data[LOCAL] = data[LOCAL][sizeof(data[LOCAL])-1];
            }
            if (!valid_file(data[LOCAL])) {
                write("Illegal local filename.\n");
                return 1;
            }
            if (file_size(FTP_ROOT_DIR + data[LOCAL]) != -1) {
                write("Local file already exists.\n");
                return 1;
            }
            break;
        case "put":
            if (sizeof(arg) < 3)
                return 0;
            command = "put";
             data[LOCAL] = FTP_ROOT_DIR+arg[ARG1];
            if (!data[LOCAL] || file_size(data[LOCAL]) <= 0) {
                write("Illegal local filename.\n");
                return 1;
            }
            if (sizeof(arg) > ARG2)
                data[REMOTE] = arg[ARG2];
            else {
                data[REMOTE] = explode(arg[ARG1], "/");
                data[REMOTE] = data[REMOTE][sizeof(data[REMOTE])-1];
            }
            data[DATA] = read_file(data[LOCAL]);
            break;
        default:
            return 0;
    }
    data[REQUEST] = "ftp";
    data[SENDER] = this_object();
    data[USER] = this_player()->query_real_name();
    data[COMMAND] = command;
    if (tmp = INETD->send_udp(arg[MUD], data, 1))
    {
        return ERR(tmp);
    }
    else
        return SUCCESS;
}
 
void udp_reply(mapping data) {
    switch(data[REQUEST]) {
        case "ftp":
        call_out("ftp_reply", 1, data);
        break;
        default:
    }
}
 
void ftp_reply(mapping data) {
    object ob;
    int rp, du;
    string tmp;
    mixed *d;
 
DBG(data);
    if (!data[USER])
        return;
    if (!data[COMMAND] || data[SYSTEM] == TIME_OUT) {
        if (ob = find_player(data[USER]))
            tell_object(ob, (data[SYSTEM] == TIME_OUT ?
            "ftp: " + data[COMMAND] + "@" + data[NAME] + " timed out.\n" :
            data[DATA]));
        return;
    }
    switch(data[COMMAND]) {
        case "dir":
            if (!data[DATA])
            {   data[DATA] = "Ftp@"+data[NAME]+
                        ": No files in directory.\n";
                break;
            }
            rp = sizeof(d = explode(data[DATA], ":"));
            tmp = "Ftp@"+data[NAME];
            tmp += ": Directory listing.\n";
            for(du = 0; du < rp; du+=2)
            {
                d[du+1] = to_int(d[du+1]);
                if (d[du+1] == -2) 
                {        d[du] += "/";
                        d[du+1] = "";
                }
                tmp += sprintf("%-20s %s\n", d[du], 
                 intp(d[du+1]) ? to_string(d[du+1]) : "");
            }
            data[DATA] = tmp;
            break;
        case "get":
            if (!data[LOCAL] || file_size(FTP_ROOT_DIR + data[LOCAL]) != -1 ||
            !write_file(FTP_ROOT_DIR + data[LOCAL], data[DATA]))
                data[DATA] = "Ftp@"+data[NAME]+":Error writing local file: " +
                FTP_ROOT_DIR + data[LOCAL] + "\n";
            else
                data[DATA] = "Ftp@"+data[NAME]+": Remote file retreived: " +
                FTP_ROOT_DIR + data[LOCAL] + "\n";
            break;
        default:
            return;
    }
    if (data[USER] && (ob = find_player(data[USER])))
        tell_object(ob, sprintf("\n%s\n",to_string(data[DATA])));
}
