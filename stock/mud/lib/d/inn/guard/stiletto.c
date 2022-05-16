
#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

reset(arg) {
  if(arg) return;
  set_name("stiletto");
  set_short(GREY_F+"jagged dark crystal stiletto"+NORM);
  set_long("Made of a crytalline substance this weapon has unuasal\n"+
           "qualities. It appears to be extremly light and well balanced,\n"+
           "and has no signs of being made by mear tools.\n");
  set_wc(21);
  set_length(20);
  set_type("thrust");
  set_weight(15);
  set_value(2000);
}

