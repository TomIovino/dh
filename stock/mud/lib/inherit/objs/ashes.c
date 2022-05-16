#include <mudlib.h>
#include <ansi.h>
 
inherit TREASURE;
 
reset (arg){
    if(arg) return;
 
    set_weight(1);
    set_value(333);
    set_name("ashes");
    set_short(GREY_F+"handful of ashes"+NORM);
   set_long("A handful of ashes, what good is it..\n");
}
