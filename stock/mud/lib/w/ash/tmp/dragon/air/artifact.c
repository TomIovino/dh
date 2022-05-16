#include <ansi.h>
#include <mudlib.h>
#include "path.h"

inherit WEAPON;
#define TP this_player()
#define QN query_name()

reset (status arg){
        if (arg) return;
        set_name("razortooth");
        set_alias("broadaxe");
  set_short(NORM+B_WHITE_F+"white broadaxe with a "+
  BOLD+MAGENTA_F+"j"+BLUE_F+"e"+BROWN_F+"w"+GREY_F"e"+RED_F+"l"+
  WHITE_F"e"CYAN_F"d " +NORM+B_WHITE_F+"hilt"+NORM);
      set_wield_str(CO"You feel the power of dark"+BOLD+
                     " winds flowing through you.\n");
      set_room_wield_str(CO+this_player()->query_name()+
                     "'s controls the power of wind.\n");
      set_info(CO+"This broadaxe is known as 'RAZORTOOTH'.\n"+NORM+
                   "You sense this axe is part of something ancient.\n");
        set_alignment(KI);
        set_type("cleave");
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
  write(B_CYAN_F+"Razortooth crushes "+attacker->QN+".\n"NORM);
  say(B_CYAN_F+"Razortooth crushes "+attacker->QN+".\n"NORM);
  if(attacker->query_level() > 25){
    write(B_CYAN_F+"You feel energy sift into you from Razortooth!\n"NORM);
    attacker->adj_hp(-5);
    attacker->adj_mana_points(-5);
    this_player()->adj_mana_points(5);
    this_player()->adj_hp(5);
    }
  return 10;
  }
}

