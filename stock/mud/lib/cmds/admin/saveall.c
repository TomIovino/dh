#include <bar.h>
#include <mudlib.h>
#include <ansi.h>
#include <ansi_filter.h>
#include <udp.h>
#include <page.h>
 

 
string do_all(object *i);
string do_save();
status main(string arg) {
  string mud, line;
    tell_object(this_player(), do_save()+"\n");
  return 1;
}
 
 
string do_save() {
  object *imm,*mort;
  string line;
 
  mort=users()-(imm=filter_objects(users(), "query_level"));
  line = PRINTBAR;
  line += do_all(imm);
  line += PRINTBAR;
  return line;
}
 
string do_all(object *i) {
  string s;
  int j,k,inv;
  string t;
  s="\n";
  i=sort_array(i, lambda( ({'x, 'y}), ({#'<,
             ({#'call_other, 'x, "query_security"}),
             ({#'call_other, 'y, "query_security_level"}) }) ) );
  for(j=0,k=sizeof(i);j<k;j++) {
    inv=0;
    if(i[j]->query_invis()){
      if(this_player()->query_security_level() < i[j]->query_security_level())
        continue;  
      else inv=1;
  }
s+=sprintf("%|s",
   sprintf("%s saved! [%d]\n",
                          (i[j]->query_name()),
                          (i[j]->save_character( ))));
    }
  s+=sprintf("\n");
  return s;
}
 
 

