/* a bottle of beer */

#include <mudlib.h>
inherit DRINK;

void reset(status arg) {
  set_name("bottle");
  set_short("an empty bottle");
  set_empty_container("bottle");
  set_full(0);
  set_strength(0);
}
