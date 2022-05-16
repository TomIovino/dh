#include <ansi.h>
#include <mudlib.h>
#include <bar.h>

status main(string arg) {
  object *i; 
  string s;
  int j,k,inv;
  string t;
  s="\n";
  i= users();
  i= sort_array(i, lambda( ({'x, 'y}), ({#'<,
             ({#'call_other, 'x, "query_security"}),
             ({#'call_other, 'y, "query_security_level"}) }) ) );
  write(PRINTBAR+"\n");
  for(j=0,k=sizeof(i);j<k;j++) {
    inv=0;
    if(i[j]->query_invis()){
      if(this_player()->query_security_level() < i[j]->query_security_level())
        continue;  
      else inv=1;
   }
    if(i[j]->query_invis()){
    continue;
   }
    if(i[j]->defined_env_var("hide_who")){
    continue;
   }
               s=sprintf("%|-15.15s",
               "---"+(i[j]->query_name())+"--------------");
              write(s);
                 write(i[j]->query_address());
                 write("\n"+NORM);
  }
  write(PRINTBAR+"\n");
  return 1;
}
 
 

