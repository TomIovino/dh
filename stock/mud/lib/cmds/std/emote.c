#include <ansi.h>
#include <mudlib.h>

int main(string str);
int help();
string *query_cmd_aliases() { return
({":", }); }


int main(string str){
  string verb;
  verb = query_verb();
  if(!str) str = "";
  if(str == "" || str == " ") {
    write("Usage: emote <msg>\n");
    return 1;
  }
  write(this_player()->do_cc("")+ this_player()->query_alt_name() 
       +" "+ str+"\n"+NORM);
  if(this_player()->query_security_level())
      say(this_player()->do_cc("")+this_player()->query_alt_name() 
       +" "+ str+"\n"+NORM);
  else
      say(this_player()->do_cc("")+":"+this_player()->query_alt_name() 
       +" "+str +"\n"+NORM);
  return 1;

}

help(){
  write("\nemote <what>\n");
  write("example:\nemote dances the dance of fire.\n");
  write("people will see:\nPlayer dances the dance of fire.\n");
  write("default alias is :\n\n");
  return 1;
}

