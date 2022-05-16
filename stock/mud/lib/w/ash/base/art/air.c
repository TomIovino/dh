#include <ansi.h>
#include <mudlib.h>

inherit WEAPON;
#define TP this_player()
#define QN query_name()
#define CC B_WHITE_F
#define KI "air"

reset (status arg){
        if (arg) return;
        set_name("razortooth");
        set_alias("broadaxe");
        set_type("cleave");
        set_short(NORM+CC+"white broadaxe with a "+
        BOLD+MAGENTA_F+"j"+BLUE_F+"e"+BROWN_F+"w"+GREY_F"e"+RED_F+"l"+
        WHITE_F"e"CYAN_F"d " +NORM+B_WHITE_F+"hilt"+NORM);
        set_wield_str(CC"You feel the power of"+BOLD+
                     " winds flowing through you.\n");
        set_room_wield_str(CC+this_player()->query_name()+
                     "'s calls the power of wind.\n");
        set_info(CC+"This broadaxe is known as 'RAZORTOOTH'.\n"+NORM+
                   "You sense this axe is part of something ancient.\n");
        set_alignment(KI);
        set_apb(1);     
        set_dpb(1);
        set_length(60);
        set_wc(27);
        set_ego(175);
        set_weight(24);
        set_value(10000);
        set_sell_destruct(1);
        set_hit_func(this_object());
}

int weapon_hit(object attacker) {
        int r;
        r = random(100);
   if(r < 10){
        write(CC+"You call the power of wind.\n"NORM); 
        say(CC+environment()->query_name()+
          "'s "+QN+" calls the power of the wind.\n"NORM);
     if(attacker->query_level() > 25){
      write(CC+"You feel energy flow into you from Razortooth!\n"NORM);
      attacker->adj_hp(-4);
      attacker->adj_sp(-4);
      this_player()->adj_sp(4);
      this_player()->adj_hp(4);
     }
  return 10;
  }
}

