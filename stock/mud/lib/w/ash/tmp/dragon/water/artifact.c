#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;
#define TP this_player()
#define QN query_name()

reset (status arg){
        if (arg) return;
        set_name("bladestar");
        set_alias("blade");
  set_short(CO"blue blade with a "+
  BOLD+MAGENTA_F+"j"+BLUE_F+"e"+BROWN_F+"w"+GREY_F"e"+RED_F+"l"+
  WHITE_F"e"CYAN_F"d " +NORM+CO+"hilt"+NORM);
     set_wield_str(CO"You feel the power of dark"+BOLD+
                     " ice flowing through you.\n");
  set_info(CO+"This blade is known as 'BLADESTAR'.\n"+NORM+
                   "You sense this blade is part of something.\n");
        set_type("slash");
        set_apb(1);     
        set_dpb(1);
        set_length(20);
        set_wc(25);
        set_ego(150);
        set_weight(20);
        set_value(10000);
        set_sell_destruct(1);
        set_hit_func(this_object());
}

int weapon_hit(object attacker) {
        int r;
        r = random(100);
        if(r < 10){
  write(B_BLUE_F+"Bladestar sinks into "+attacker->QN+".\n"NORM);
  say(B_BLUE_F+"Bladestar sinks into "+attacker->QN+".\n"NORM);
  if(attacker->query_level() > 25){
    write(B_BLUE_F+"You feel energy sift into you from Bladestar!\n"NORM);
    attacker->adj_hp(-5);
    attacker->adj_mana_points(-5);
    this_player()->adj_hp(5);
    this_player()->adj_mana_points(5);
    }
  return 10;
  }
}

