#include <ansi.h>
#include <mudlib.h>
inherit SHOPKEEP;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    add_object("NOBLE_OB","d/town/obj/NOBLE_OB");
    STOREROOM = "/d/town/market/m_warehouse";
    SIGN_TEXT = "Welcome to the magic store, 'help shop' for assistance";
    set_name("sylv");
    set_alias("dragonkin");
    set_short(GREEN_F+"decloaked dragonkin"+NORM);
    set_long(GREEN_F+
"For years it was believed that Sylv was of elven decent for with her\n"+
"slim and always cloaked face it gave that impression. When the city was\n"+
"liberated by her people, She removed her hooded cloak as well as doubt.\n"+
"Although her form is slim, her body is complete with metallic green\n"+
"scales which cover her from head to toe.\n");
    set_race("dragonkin");
    add_spell_immunity("charm");
    set_gender(2);
    set_level(50);
    set_half_exp(1);
    set_magic_resist(50);

}
