 
#include <ansi.h>
#include <mudlib.h>
inherit TREASURE;
 
reset (arg){
    if(arg) return;
 
    set_weight(1);
    set_value(510);
    set_name("sulphur");
   set_short(YELLOW_F+"sulphur"+NORM);
    set_long("A small piece of dirty yellow rock.\n");
}
