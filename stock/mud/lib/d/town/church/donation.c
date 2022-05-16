#include <mudlib.h>
#include <ansi.h>
inherit CHURCH;


void reset(status arg) {
    object obj;
   call_other("/inherit/objs/churchchest","query_weight()");
    move_object(find_object("/inherit/objs/churchchest"),this_object());
    ::reset(arg);
    if(arg) return;
//    set_type("neutral");
    set_short("The Temple");
    set_long(CYAN_F+
"This is the donation room for the needy of the world. New and old\n"+
"adventurers are welcomed to take what they need. Be warned however\n"+
"the gods dont favor the greedy.\n"+
  "");

    set_no_fight(1);
    set_no_magic(1);
    set_no_steal(1);

    set_light(2);
    set_items(({
	"marble#walls",
	"White marble which glows with an inner light and is colored by the jewelled\n"+
	"tones of the stained glass windows.\n",
	"windows#stained glass#stained glass windows#light",
	"Huge stained glass windows depicting various good deeds by the founders of\n"+
	"fair Konkaria spill colored light across the white marble room.\n",
	"floor","A white marbled floor that has been worn by many feet.\n",
	"statues#dragons",
	"Crystalline statues of dragons in various poses stand in each corner of the\n"+
	"room, watching over the citizens of Konkaria.\n",
	"ceiling#gallery",
	"The ceiling rises high above the gallery of windows, forming a steepled\n"+

	"pinnacle at the center of the room.\n",
      }));
    set_exits(({
        "d/town/church/church","south",
         "/d/town/church/aboard","north",
      }));
   }

x(){
  ::init();
  add_action("dlog","dlog");
  add_action("slog","slog");
  add_action("take","get");
  add_action("get","get");
      }
int get(string what){
    string x,y;
    if(what == "all from chest")   {
    write("Something prevents that action.\n");
    return 1;    }

   if(sscanf(what, "%s from %s", x,y) == 2 && y == "chest"){
   log_file("DLOG.dat",
   this_player()->query_name()+" welfared "+x+"\n"); 
   return 0;
   }
   return 0;
}

int dlog(){ tail("/log/DLOG.dat"); return 1; }

int slog(){ tail("/log/SLOG.dat"); return 1; }

