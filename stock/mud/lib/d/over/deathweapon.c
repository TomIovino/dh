#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

void query_info();
void info();
void reset(status arg) {
  if(arg) return;
  set_name("Sheperd");
  set_short(GREY_F+"Death's Scythe"+NORM);
  set_alias("scythe");
  set_long("\
This blade appears to be made of some kind of\n\
magic fire, although warm it is not uncomfortable\n\
  ");
  set_wc(27);
  set_type("cleave");
  set_length(21);  
  set_weight(5);
  set_value(1600);
  set_enchanted(1);
set_no_steal_flag(1);
}

