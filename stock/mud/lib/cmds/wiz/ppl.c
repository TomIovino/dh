#include <ansi.h>
#include <mudlib.h>
#include <bar.h>

status main(string arg) {
  object *i; 
  string s;
  int j,k,inv;
  string t;
  string x;
  s="\n";
  i= users();
  i= sort_array(i, lambda( ({'x, 'y}), ({#'<,
             ({#'call_other, 'x, "query_alt_level"}),
             ({#'call_other, 'y, "query_alt_level"}) }) ) );
  for(j=0,k=sizeof(i);j<k;j++) {
    inv=0;
    if(i[j]->query_invis()){
      if(this_player()->query_alt_level() < i[j]->query_alt_level())
        continue;  
      else inv=1;
  }


   s+=sprintf("%s\n",
   sprintf("%11s %3d %3d-%2-d %2s %12d %6d %2d/%2d-(%2d) %3d-(%2d)",
                          (i[j]->query_real_name()),
                          (i[j]->query_alt_level()),
                          (i[j]->query_level()),
                          (i[j]->query_extrastats_total()),
                          extract(""+i[j]->query_tactic(),0,2),
                          (i[j]->query_funds()),
                          (i[j]->query_master_exp()),
                          (i[j]->query_right_wc()),
                          (i[j]->query_left_wc()),
                          (i[j]->query_opb()),
                          (i[j]->query_ac()),
                          (i[j]->query_dpb())));
  }
  write(PRINTBAR+"\n");
  write(B_WHITE_F+
"-----name----"+B_BLUE_F+
"-|--Lv-+---St---"+B_RED_F+
"--$$------Rate---"+B_GREEN_F+
"-rwc/lwc(+)---ac-(+)--"+NORM);
  write(s);
  write(PRINTBAR+"\n");
  return 1;
}
 
 

