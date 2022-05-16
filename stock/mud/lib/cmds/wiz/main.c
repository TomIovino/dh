#include <ansi.h>
#include <mudlib.h>
#define TP this_player()
status main(string arg) {
   if(file_size("log/log") == -1){
        write("no /log/log file.\n");
        return 1;
   }
    command("tail /log/log",this_player());
    return 1;
}
