#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
#define TP this_player()
status main(string str) {
  if(this_player()->query_primary_attack())
  {
  this_player()->random_move();
  return 1;
  }
  write("Your not in combat!\n");
  return 1;
}
