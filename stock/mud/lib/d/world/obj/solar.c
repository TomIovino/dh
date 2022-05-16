// for ebony

#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;
#define TP this_player()
#define QN query_name()
#define TR B_RED_F
#define GR B_YELLOW_F
#define WH B_WHITE_F
reset (status arg){
    if (arg) return;
    set_name("solar");
    set_alias("katana");
    set_short(GR"su"WH"n r"TR"ay "GR"ka"WH"ta"TR"na"NORM);
    set_long("A sword made of the finest of metals, jewels and magic\n");
    set_type("slash");
    set_apb(1);
    set_dpb(1);
    set_no_break(1);
    set_ego(100);
    set_length(40);
    set_wc(22);
    set_weight(22);
    adj_enchant(5);
    set_no_steal_flag(1);
    set_light(1);
    set_value(2500);
    set_sell_destruct(1);
    set_hit_func(this_object());
    set_info(B_YELLOW_F+"This is the Sunray Katana, named Solar.\n"+NORM);

}

int weapon_hit(object attacker) {
    int r;
    r = random(100);
    if(r < 15){
	write(B_YELLOW_F+"A sunray flares at "+attacker->QN+".\n"NORM);
	say(B_YELLOW_F+"A sunray flares at "+attacker->QN+".\n"NORM);
        this_player()->adj_sp(1+random(2));
        this_player()->adj_hp(1+random(2));
        return 5;
	}
	return 1;
}






