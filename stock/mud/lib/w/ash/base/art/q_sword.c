#define QN query_cap_name()
#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;
inherit "/inherit/tell_all";

void reset(status arg){
    if(arg) return;
    set_name("ashphoenixember");
    set_short(
    BLUE_F"ornate ancient m"B_BLUE_F"i"NORM CYAN_F"t"B_CYAN_F
    "h"NORM CYAN_F+"r"B_BLUE_F"i"NORM BLUE_F"l long sword");
    set_long(B_WHITE_F+
    "ancient "NORM+BLUE_F"m"BOLD"i"CYAN_F"t"NORM+CYAN_F+
    "h"B_CYAN_F+"r"+B_BLUE_F"i"NORM+BLUE_F"l"B_WHITE_F" sword"+
    "Try 'pose ashphoenixember' for a neat special affect.\n");
    set_alias("sword");
    set_wc(35);
    set_length(50);  
    set_weight(33);
    set_opb(5);
    set_dpb(5);
    set_ego(175);
    adj_enchant(5);
    set_light(1);
    set_no_break(1);
    set_no_steal_flag(1);
   set_wield_str(
 B_CYAN_F+"The power of elements mixes with your soul as you wield Ashphoenixember.\n");
    set_type("cleave");
    set_value(10000);
    set_hit_func(this_object());

}


pose(string to){
     if(to != "ashphoenixember")  return 0;
     if(this_player()->query_sp() < 200) return 0;
     this_player()->adj_sp(-200);
     tell_all(YELLOW_F+this_player()->query_cap_name()+
       " calls forth the Spirit of the Phoenix Dragon.\n"+NORM);
    return 1;
}

old_do_death(){ destruct(this_object()); }
old_death(){ call_out("do_death",2); }

int weapon_hit(object attacker) {
        int r;
        r = random(100);
        if(r < 10){
  write(BLINK+B_RED_F+
   "Ashphoenixember devours the spirit of "+attacker->QN+".\n"NORM);
  say(BLINK+B_RED_F+"Ashphoenixember devours the spirit of "+attacker->QN+".\n"NORM);
  if(attacker->query_level() > 33){
    write(GREY_F"You feel energy sift into you!\n"NORM);
    attacker->adj_hp(-15);
    attacker->adj_sp(-15);
    this_player()->adj_hp(15);
    this_player()->adj_sp(15);
    }
  return 10;
  }
}
init(){
  ::init();
  add_action("pose","pose");
}

