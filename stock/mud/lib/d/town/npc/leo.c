#include <mudlib.h>
inherit BARKEEP;
#include <ansi.h>

reset(arg) {
    ::reset(arg);
    if(arg) return;
    add_object("NOBLE_OB","d/town/obj/NOBLE_OB");
    set_name("leo");
    set_short(YELLOW_F+"dragonkin barkeep"+NORM);
    set_alias("barkeep");
    set_long(
      "The huge bartender of the Old Konkaria Pub, now established here in\n"+
      "Twilite City, is clearly a Dragonkin. However, his friendly smile, and\n"+
      "twinkling brown eyes show him to be of kind disposition. He loves to\n"+
      "stand and chat with patrons as they slowly inebriate themselves.\n");
    set_race("dragonkin");
    set_gender(1);
    load_chat(1,({
        "Leo smiles with the grace of a noble.\n",
        "Leo says: Commoners eat at Grogs near the south road.\n"
        "Leo says: The king is a proud warrior.\n"
        "Leo says: The king is a master of combat.\n"
    }));
    add_drink("mixture","bottle of creamy mixture","It tastes like sugar.",51);
    add_food("sandwich", "steak sandwich", "That was excellent.",51);
    add_sdrink("tea", "glass of sun tea", "That was brisk.", 51);
    set_level(50);
    set_half_exp(1);
    set_magic_resist(50);

}
