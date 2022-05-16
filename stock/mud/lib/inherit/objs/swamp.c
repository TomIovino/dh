 
#include <mudlib.h>
inherit BASE;
 
reset(arg){
   if (arg) return;
 
    set_short("swamp water");
   set_value(10);
   set_long("Some icky swamp water.\n");
   set_name("swamp water");
   set_alias("water");
   set_alt_name("Swamp Water");
   set_weight(1);
}
