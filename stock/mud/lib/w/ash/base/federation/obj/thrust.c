
#include "obj.h"
#define NAME "rapier"
#define TYPE "thrust"
#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

reset(arg) {
  if(arg) return;
  set_name(NAME);
  set_short(COLOR+SHORT+" steel "+NAME+" "NORM);
  set_long(COLOR+LONG+" steel "+NAME+" set with gems.\n"+NORM);
  set_type(TYPE);
  set_wc(14);
  set_ego(10);  
  set_length(20);
  set_type(TYPE);
  set_weight(14);
  set_value(1000);
}

