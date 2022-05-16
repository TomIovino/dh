#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
#define TP find_player(arg)
string *query_cmd_aliases() { return 
({"equip","equipment" }); }

status main(string arg) {

string temp;

  if(!arg) arg = this_player()->query_real_name();

  if(!this_player()->query_security_level())
      arg = this_player()->query_real_name();

  if(!find_player(arg)){ write("No.\n"); return 1; }

if(this_player()->test_dark()) return 1;

 printf("\n%s%s\n", PRINTBAR, NORM);

temp = TP->show_eq(arg);
if(temp) printf("%s",temp);
if(!temp || temp == "") write("You have no gear worn or wielded.\n");
printf("\
%s\n\
%s\n",
PRINTBAR,
NORM);
  return 1;
}
