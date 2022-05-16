#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;
#define TP this_player()
#define QN query_name()

reset(status arg) {
    if(arg) return;
    set_name("longsword");
    set_alias("sword");
    set_short(B_RED_F+"ornate red longsword"+NORM);
    set_long("An ornate red longsword of polished admantite.\n");
    set_wc(23);
    set_type("slash");
    set_length(30);
    set_ego(70);
    set_weight(20);
    set_value(1500);
    set_light(1);
}
