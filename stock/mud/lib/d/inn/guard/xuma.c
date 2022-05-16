#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;

reset(arg) {
    object l,r;
    ::reset(arg);
    if(arg) return;

    set_name("Xuma");
    set_short(GREY_F+"darkhaired man"+NORM);
    set_alias("man");
    add_money(random(1000));
    set_long(
      "The malevolent glare exuding from his beady, shifting eyes pierces\n"+
      "through shadows, stripping you down to your soul. His left hand\n"+
      "frequently twitches towards his side, where a slight bulge may indicate\n"+
      "a weapon of some sort.\n"+NORM);
    set_race("human");
    load_chat(1, ({ 
	"Xuma searches around looking for something..\n",
	"Xuma mumbles about losing something..\n",
      }));
    load_spells(10, ({
  "stun","heal","poison","curse","storm",
      }));
    set_gender(1);
    set_level(90);
    set_half_exp(1);
    set_magic_resist(100);
    r = clone_object("/d/inn/guard/stiletto");
    r = move_object(r,this_object());
    command("wield stiletto",this_object());
    l = clone_object("/d/inn/guard/stiletto");
    l = move_object(l,this_object());
    command("wield stiletto",this_object());

}
/*
init(){
    ::init();
}
*/
