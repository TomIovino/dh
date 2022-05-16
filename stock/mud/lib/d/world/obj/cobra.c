#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;
#define TP this_player()
#define QN query_name()
#define TR B_GREEN_F
#define GR RED_F
#define WH GREEN_F
reset (status arg){
    if (arg) return;
    set_name("serpentine");
    set_alias("staff");
    set_short(GR+"s"+TR+"erpentin"+NORM+WH+"e"+ 
              GR+" q"+TR+"uarterstaf"+NORM+WH+"f"+NORM);
    set_long("LOOKS LIKE A COBRA..HISSS\n");
    set_no_break(1);
    set_type("pound");
    set_apb(1);     
    set_dpb(1);
    set_ego(100);
    set_length(28);
    set_wc(24);
    set_weight(23);
    adj_enchant(0);
    set_no_steal_flag(1);
    set_light(1);
    set_value(1);
    set_sell_destruct(1);
    set_hit_func(this_object());
    set_info(B_GREEN_F+
"This is the Staff, Named Serpentine, it casts poison at your attacker.\n"+NORM);
}

int weapon_hit(object attacker) {
    int r;
    r = random(100);
    if(r < 25){
	write(B_RED_F+"Serpentine staff bites "+attacker->QN+".\n"NORM);
	say(B_RED_F+"You hear a hissing and see a flash near "+
         attacker->QN+".\n"NORM);
        this_player()->adj_mana_points(1+random(2));
        this_player()->adj_hp(1+random(2));
        attacker->set_var("poison",100);
        attacker->set_var("poison_time",20); 
        return 1;
	}
	return 1;
}






