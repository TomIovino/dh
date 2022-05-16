#include <mudlib.h>
#include <ansi.h>

string *query_cmd_aliases() { return ({ "dia","diagnose","diag", }); }

#include <valid.cfg>
#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
#define TP this_player()
status main(string arg) {
   mixed ansi; 
   ansi = TP->query_env_var("ANSI");
   TP->unset_env_var("ANSI");
   say(B_CYAN_F+TP->query_name()+" reports:"+TP->print_prompt()+".\n"+NORM);
  if(ansi)
   TP->set_env_var("ANSI",ansi);
   write(B_CYAN_F+"You report: "+TP->print_prompt()+".\n"+NORM);

 return 1;
}

