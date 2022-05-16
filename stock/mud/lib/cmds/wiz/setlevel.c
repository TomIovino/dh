#include <ansi.h>
#include <mudlib.h>
#define TP this_player()
status main(string arg) {
    int i;
    i = to_int(arg);
    if(i > 100){ i = 100; }
    if(i < 1){ i = 1; }
    TP->set_level(i);
    TP->set_exp(1);
  write("You are level "+i+".\n");
    return 1;
}
