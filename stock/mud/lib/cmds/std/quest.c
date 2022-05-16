#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
string *query_cmd_aliases() { return ({ "quests", }); }

#define TP find_player(arg)

status main(string arg){
  if(!arg) arg = this_player()->query_real_name();
  if(!this_player()->query_security_level())
      arg = this_player()->query_real_name();
  if(!find_player(arg)){ write("No.\n"); return 1; }

printf("\n\
%s\n",
PRINTBAR,
NORM);
write(YELLOW_F);
write("Quest Points : "+TP->query_quest_points()+"\n\n");
write("Current Quest : "+TP->query_current_quest()+"\n");
write("Quest Hint : "+TP->quest_hint()+"\n\n");
write("Completed Quests : "+implode(TP->query_done_quests(),","));
printf("\n\
%s\n\
%s\n",
PRINTBAR,
NORM);
  return 1;
}


