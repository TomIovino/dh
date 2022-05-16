// edited by Jada
#define TP this_player()
#define EXTRA this_player()->query_total_remorts()*5;
#include <mudlib.h>
#include <ansi.h>
inherit INNROOM;
reset (arg) {
  if (arg) return;
   set_short("The Pleasant Inn");
   set_long(RED_F+
"The white walled room of the inn proper attests to the fact that\n"+
"despite the heavy use of its patrons the owners keep the place in\n"+
"very good shape. West is a door that leads directly back onto the\n"+
"street. The stairs lead up to many rooms. A sign is posted.\n"+
   NORM);
   set_light(2);
   set_items(({
   "sign","You should read the sign.\n",
   "wall#walls","Well maintained white walls.\n",
   "window#outside","Always a beautiful day in Twilite.\n",
   "fireplace","Burning softly it keeps the room warm.\n",
   "door","It leads back to the Street.\n",
   }));
   set_exits (({
   "/d/town/market/ave5","west",
  "/d/inn/code/UPSTAIRS","up",
   }));

}


