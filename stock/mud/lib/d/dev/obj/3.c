#define CLONE  ({ "helm","boots","gloves","sleeves","pants","floater",\
                  "shield","armour","weapon","belt","amulet","ring",\
                  "cloak","ankles","wrists","mask",})

#include <ansi.h>
#include <mudlib.h>
#include "../element.h"



init(){
  string name;
  int c;  
  c            = random(sizeof(CLONE));
  name = call_other(PATH+"obj/3"+CLONE[c],"query_name");
  move_object(clone_object(PATH+"obj/3"+CLONE[c]),this_player());
  command("wear "+name,this_player());
  command("wield "+name,this_player());
  destruct(this_object());

}
