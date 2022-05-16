#include <ansi.h>
#include <mudlib.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if(arg) return;
  if(!random(5))
    add_object("luna","/d/world/obj/luna");
    add_object("ebonychain","/d/world/obj/ebonychain");
    add_object("rubyshield","/d/world/obj/rubyshield");
    set_name("ebony");
    set_alias("shadow");
    set_short(GREY_F+"female shadow"+NORM);
    set_long(
      "Ebony inspects you with her cool, detached gaze.  Her brown\n"+
      "eyes hold you frozen in their deep pools.  Dark, gleaming,\n"+
      "curling masses of hair tumble around her body in a wild\n"+
      "tangle.  Ebony wears a skin tight robe of dark brown that\n"+
      "displays her lush curves.\n");
    set_race("shadow");
    set_gender(2);
    load_chat(10,({
         "*absorb","*deflect","*reflect",
	"Ebony chants in an unknown tongue.\n",
	"Ebony says: Have you got a problem?\n",
      }));
    load_spells(10,({
      "defender","avenger","heal","spike",
      }));
    set_level(119);
    set_magic_resist(100);
}

/*
 init(){
  ::init();  
  if(this_player()->query_alignment() != "evil"){
      write("Ebony points at You!\n");
      adj_sp(50);
      command("turn "+this_player()->query_real_name(),this_object());
  }
}
*/
