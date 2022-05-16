#include <mudlib.h>
#include <ansi.h>


string *query_cmd_aliases() { return ({ "pp", "pep" }); }
#include <filters.h>
/************************************************************************/
/* list_peoples */
  
void page(string s) {
    object pager;
    move_object((pager=clone_object(PAGER)),this_player());
    pager->page(s);
    return;
}
main(arg) { 
  string temp, domain_str;
  object *who;
  string str, outp, space;
  int i;
  int a; 
  int idle_count;
  string location;
     
  if(!this_player()->query_security_level()) return 1; 

  who = filter_array(users(),"filter_users",this_object());  

    who = sort_array(who, "by_sec_level", this_object());
    str =  "Name ---\n";
    write(str);
    for(i = 0; i < sizeof(who); i++) {
      str = capitalize((string)who[i]->query_real_name());
      str += (string)who[i]->query_security_level()+"  --"+
             (string)who[i]->query_total_remorts()+"-"+
             (string)who[i]->query_level()+"  --"+
             (string)who[i]->query_exp()+"  --"+
             (string)who[i]->query_master_exp()+"  --\n";
    write(str);
    }
    return 1;  
}
