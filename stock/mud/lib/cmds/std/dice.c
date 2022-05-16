#include <bar.h>
#include <mudlib.h>
#include <ansi.h>
string *query_cmd_aliases() { return
({"stones", }); }

#define TPQN GREY_F+this_player()->query_name()

status main(int cheat){
         string x;
         int r,y,b;
         r = random(9)+1;
         y = random(9)+1;
         b = random(9)+1;
  if(cheat)
  if(this_player()->query_name() == "Ash") r = y = b = cheat;
         x = TPQN+" throws the colored stones onto the ground,\n";
         x += "they read:..... "B_RED_F+r+" "+YELLOW_F+y+" "+BLUE_F+b+".\n";
         write(x);
         say(x);
      return 1;
  }

