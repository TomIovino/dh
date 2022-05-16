#define QN query_cap_name()
#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;
inherit "/inherit/tell_all";

void reset(status arg){
    if(arg) return;

    set_name("stormbringer");
    set_short(
    BLUE_F"ancient m"B_BLUE_F"i"NORM CYAN_F"t"B_CYAN_F
    "h"NORM CYAN_F+"r"B_BLUE_F"i"NORM BLUE_F"l wand");
    set_long(B_WHITE_F+
    "ancient "NORM+BLUE_F"m"BOLD"i"CYAN_F"t"NORM+CYAN_F+
    "h"B_CYAN_F+"r"+B_BLUE_F"i"NORM+BLUE_F"l"B_WHITE_F" wand"+
    "Try 'pose stormbringer' for a neat special affect.\n");
    set_alias("wand");
    set_wc(30);
    adj_enchant(5);
    set_light(1);
    set_no_break(1);
    set_no_steal_flag(1);
    set_apb(3);
    set_ego(175);
    set_dpb(3);
   set_wield_str(
   B_CYAN_F+"The power of elements mixes with your soul as you wield Stormbringer.\n");
/*
   set_room_wield_str(B_CYAN_F+-->query_cap_name()+
  " wields the power of the Elements in Stormbringer.\n");
*/
    set_type("artifact");
    set_length(35);  
    set_weight(15);
    set_value(10000);
    set_attack_msg(({
        "missed", "",
        YELLOW_F+"burned","with a minor shock"+NORM,
        GREEN_F+"beamed","with a small bolt of lightning"+NORM,
        BLUE_F+"crushed","with a magical shock to the head"+NORM,
        RED_F+"electrified","with the bolt of lightning"+NORM,
        RED_F+"INCINERATED","with the Stormbringer"+NORM,
      }));

    set_hit_func(this_object());

}


pose(string to){
     if(to != "stormbringer")  return 0;
     if(this_player()->query_sp() < 100) return 0;
     this_player()->adj_sp(-100);
     tell_all(CYAN_F+this_player()->query_cap_name()+
       " calls forth the Storms of the Elements.\n");
    return 1;
}

old_do_death(){ destruct(this_object()); }
old_death(){ call_out("do_death",2); }

int weapon_hit(object attacker) {
        int r;
        r = random(100);
        if(r < 10){
  write(B_CYAN_F+"Stormbringer drains "+attacker->QN+".\n"NORM);
  say(B_CYAN_F+"Stormbringer drains "+attacker->QN+".\n"NORM);
  if(attacker->query_level() > 25){
    write(B_CYAN_F"You feel energy sift into you from Stormbringer!\n"NORM);
    attacker->adj_hp(-10);
    attacker->adj_mana_points(-10);
    this_player()->adj_hp(10);
    this_player()->adj_mana_points(10);
    }
  return 10;
  }
}
init(){
  ::init();
  add_action("pose","pose");
 }

