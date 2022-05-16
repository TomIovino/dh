#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;
#define TP this_player()
#define QN query_name()

reset(status arg) {
    if(arg) return;
    set_name("scimitar");
    set_alias("sword");
    set_short(B_RED_F+"flaming scimitar"+NORM);
    set_long("An ornate scimitar of polished admantite.\n");
    set_wc(23);
    set_type("slash");
    set_length(30);
    set_ego(70);
    set_weight(20);
    set_value(1500);
    set_light(1);
    set_hit_func(this_object());
}

int weapon_hit(object attacker) {
   if(!random(8)){
   write(B_RED_F+"You burn "+attacker->query_name()+"!\n"+NORM);
   return 5;
   }
   return 1;
}
