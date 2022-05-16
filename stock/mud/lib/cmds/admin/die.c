#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
#define TP this_player()
status main(string str) {
  object obj;
        obj = this_player();
        this_player()->add_secondary_attacker(obj); /* get ready to attack*/
        this_player()->set_primary_attack(obj);     /* make sure attack
        this_player()->attack();                    /* attack */

  return 1;
}
