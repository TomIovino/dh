#include <ansi.h>
#include <mudlib.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if(arg) return;
  if(!random(5))
    add_object("solar","/d/world/obj/solar");
    add_object("ivorychain","/d/world/obj/ivorychain");
    add_object("emeraldshield","/d/world/obj/emeraldshield");
    set_name("ivory");
  // set_wander(50, 30, 0, "/d/world");
    set_alias("avatar");
    set_short(B_WHITE_F+"female avatar"+NORM);
    set_long(
      "Ivory smiles at you with a demure charm. Her blue\n"+
      "eyes seem to examine your very soul. Her light blond\n"+
      "hair flows over her shoulders and over her skin tight\n"+
      "robes and lush features.\n");
    set_race("avatar");
    set_gender(2);
    load_chat(10,({
         "*absorb","*deflect","*reflect",
	"Ivory chants in an unknown tongue.\n",
	"Ivory says: Have you prayed today?\n",
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
  if(this_player()->query_alignment() != "good"){
      write("Ivory points at You!\n");
      adj_sp(50);
      command("turn "+this_player()->query_real_name(),this_object());
  }
}
*/
