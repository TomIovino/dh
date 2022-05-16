#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;
#define TP this_player()
#define QN query_name()

reset (status arg){
    if (arg) return;
    set_name("broadsword");
    set_alias("sword");
    set_short(B_RED_F"rune etched broadsword"+NORM);
    set_long("This armour is made of the finest metals.\n"+
             "It also bears obvious magical enhancement.\n");
    set_type("cleave");
    set_length(25);
    set_light(1);
    set_wc(25);
    set_weight(25);
    set_value(3200);
    set_sell_destruct(1);
    set_hit_func(this_object());
}

int weapon_hit(object attacker) { 
    int r;
    r = random(100);
    if(r < 20){
	write(RED_F+"You carve "+attacker->QN+" with your sword.\n"+NORM);
	say(B_WHITE_F+TP->QN+" carves up "+attacker->QN+" with "+this_player()->query_possessive()+" sword.\n"+NORM,this_player());
	return 10;
    }
}
