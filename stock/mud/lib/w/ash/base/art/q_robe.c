#define QN query_cap_name()
#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;
inherit "/inherit/tell_all";

void reset(status arg){
    if(arg) return;
    set_name("stormtamer");
    set_short(
    BLUE_F"ancient m"B_BLUE_F"i"NORM CYAN_F"t"B_CYAN_F
    "h"NORM CYAN_F+"r"B_BLUE_F"i"NORM BLUE_F"l robe");
    set_long(B_WHITE_F+
    "ancient "NORM+BLUE_F"m"BOLD"i"CYAN_F"t"NORM+CYAN_F+
    "h"B_CYAN_F+"r"+B_BLUE_F"i"NORM+BLUE_F"l"B_WHITE_F" robe"+
    "Try 'pose stormtamer' for a neat special affect.\n");
    set_alias("robe");
    set_ac(30);
    set_weight(25);
    set_dpb(5);
    set_ego(175);
    adj_enchant(5);
    set_light(1);
    set_no_break(1);
    set_no_steal_flag(1);
   set_wear_str(
   B_CYAN_F+"The power of elements mixes with your soul as you wear Stormtamer.\n");
    set_type("armour");
    set_value(10000);

}


pose(string to){
     if(to != "stormtamer")  return 0;
     if(this_player()->query_sp() < 100) return 0;
     this_player()->adj_sp(-100);
     tell_all(CYAN_F+this_player()->query_cap_name()+
       " dispels the Storms of the Elements.\n");
    return 1;
}

old_do_death(){ destruct(this_object()); }
old_death(){ call_out("do_death",2); }

init(){
  ::init();
  add_action("pose","pose");
 }

