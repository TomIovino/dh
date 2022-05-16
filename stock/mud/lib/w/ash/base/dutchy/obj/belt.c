
#include "obj.h"
#define NAME "belt"
#define TYPE "belt"
#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name(NAME);
  set_short(COLOR+SHORT+" steel "+NAME+" "NORM);
  set_long(COLOR+LONG+" steel "+NAME+" set with gems.\n"+NORM);
  set_type(TYPE);
  set_ac(7);  
  set_weight(7);
  set_value(250);
}

