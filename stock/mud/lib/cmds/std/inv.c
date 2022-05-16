#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
#define TP this_player()
string *query_cmd_aliases() { return 
({"i","inventory", }); }

status main(string str) {
string temp;
if(TP->test_dark()){ return 1; }
printf("\n\
%s\n",
PRINTBAR,
NORM);
TP->show_inventory();
printf("\
%s\n\
%s\n",
PRINTBAR,
NORM);
  return 1;
}
