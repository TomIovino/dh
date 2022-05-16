/* a bottle of beer */

#include <mudlib.h>
inherit DRINK;

void reset(status arg) {
  set_name("wine");
  set_empty_container("bottle");
  set_short("glass of wine");
  set_long("The wine is help in an rose bottle.\n");
  set_weight(1);
  set_value(10);
  set_consumer_msg("It is really good wine!\n");  set_room_msg("@@query_name:$this_player()$@@ drinks a glass of wine.\n");  set_alco_drink(1);
  set_strength(2);
}
