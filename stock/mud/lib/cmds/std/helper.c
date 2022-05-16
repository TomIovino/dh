// ash

string *query_cmd_aliases() { return ({ "nh" }); }

#include <valid.cfg>
#include <ansi.h>
#include <mudlib.h>
#include <bar.h>

#define TP this_player()

status main(string arg) {
  if(this_player()->query_level() < 50){
       write("Must be level 50 to be a newbie helper.\n");
      this_player()->unset_attrib("helper");
   return 1;
    }
  if(!this_player()->query_attrib("helper")){
   write("Your [NH] flag is now ON.\n");
      this_player()->set_attrib("helper",1);
      return 1;
  }
  if(this_player()->query_attrib("helper")){
   write("Your [NH] flag is now OFF.\n");
      this_player()->unset_attrib("helper");
      return 1;
  }  
return 1; //shouldnt happen
}

