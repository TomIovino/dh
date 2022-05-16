#include <ansi.h>
#include <mudlib.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    set_name("golem");
    set_alias("frog");
    set_short(B_BLUE_F+"giant mithril frog golem"+NORM);
    set_long(
     "An animated giant frog made from mithril.\n"+NORM);
    set_race("avatar");
    set_gender(2);
    load_chat(10,({
        "Frog croaks a warning at you!\n",
      }));
    set_level(60);
    set_money(5000);
    set_magic_resist(100);
}

/*
 init(){
  ::init();  
  if(this_player()->query_alignment() != "good"){
      write("Ivory points at You!\n");
      adj_sp(50);
      command("turn "+this_player()->query_real_name(),this_object());
  }
}
*/
