// for merlin

#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("spectacles");
    set_alt_name("glasses");
    set_short(B_WHITE_F"silver rimmed "RED_F"red "B_WHITE_F"and "BLUE_F+
             "blue "+B_WHITE_F+"spectacles"+NORM);
    set_long("Stranges eye-pieces with different colored lenses.\n");
    set_type("mask");
    set_ac(11);
    set_bonus_wis(1);
    set_bonus_int(1);
    set_bonus_foc(1);
    set_weight(10);
    set_value(850);
    set_ego(70);
}
