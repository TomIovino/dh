#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
#define TP this_player()
int load;

#define DEF time()

extra(){
  write(B_RED_F);
  write("\t  OFFICIAL MUD TIME       "+ctime(DEF)+"\n");
  write(YELLOW_F);
  write("\t  Pacific  (California)   "+ctime(DEF-10800)+"\n");
  write("\t  Mountain (Colorado)     "+ctime(DEF-7200)+"\n");
  write("\t  Central  (Minnesota)    "+ctime(DEF-3600)+"\n");
  write("\t  Eastern  (New York)     "+ctime(DEF)+"\n");
  write("\t  Gmt      (Ireland)      "+ctime(DEF+18000)+"\n");
  write("\t  C.Europe (Germany)      "+ctime(DEF+21600)+"\n");

}

int uptime() { return to_int(__MASTER_OBJECT__->uptime()); }

status main(string str) {

command("*uptime",this_player());

   write(PRINTBAR NORM "\n" B_GREEN_F);
printf("%-=65s","It is " + query_mud_time() + 
 " in the Realm of Dragonheart.\n\n"); 
     extra();
write(NORM+PRINTBAR+NORM+"\n"); 

command("*timeleft",this_player());

  return 1;
}


