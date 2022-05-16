#include <ansi.h>
#define TP this_player()
#include <mudlib.h>
#include <bar.h>
string *query_cmd_aliases() { return ({"mp","sp", }); } 
status main() {
TP->print_prompt();
  return 1;
  }
