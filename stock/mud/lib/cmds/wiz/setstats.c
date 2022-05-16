/* this code is correct for the 100 level system.. =) */
#include <ansi.h>
#include <mudlib.h>
#define TP this_player()
status main(string arg) {
    int i;
    i = to_int(arg);
    if(i > 100){ i = 100; }
    if(i < 1){ i = 1; }
    TP->set_all_stats(i);
  write("All your stats are now "+i+".\n");
    return 1;
}
