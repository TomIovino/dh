#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

string tree_location;

void reset(status arg){
  if(arg) return;
  set_alt_name("branch");
  set_name("club");
  set_type("crush");
  set_wc(6); /* ..its not a good club, but its better than nothing! */
  set_short(BROWN_F+"tree branch"+NORM);
  set_length(80);
  set_alias("wood");
  set_long("Its a lump of dead wood.\n");
  set_value(1);
  set_weight(1);
  set_sell_destruct(1);
}

string set_tree_location(string str) {  return tree_location = str; }
string query_tree_location()         {  return tree_location; }  


void info() {
  if(tree_location) {
    write("This is from the tree growing at "+ 
           call_other(tree_location,"short") +".\n");
  }
}

status query_domain_safe() { return 1; }

