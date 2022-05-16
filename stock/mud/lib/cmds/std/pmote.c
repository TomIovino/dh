#include <ansi.h>
#include <mudlib.h>

int main(string str);
int help();
string *query_cmd_aliases() { return
({";", }); }


int main(string str){
  string verb;
  verb = query_verb();
  if(!str) str = "";
  if(str == "" || str == " ") {
    write("Usage: pmote <msg>\n");
    return 1;
  }
  write(this_player()->do_cc("")+ this_player()->query_alt_name() 
       +"'s "+ str+"\n"+NORM);
  if(this_player()->query_security_level())
      say(this_player()->do_cc("")+this_player()->query_alt_name() 
       +"'s "+ str+"\n"+NORM);
  else
      say(this_player()->do_cc("")+":"+this_player()->query_alt_name() 
       +"'s "+str +"\n"+NORM);
  return 1;

}

help(){
  write("\npmote <what>\n");
  write("example:\npmote sword was stolen.\n");
  write("people will see:\nPlayer's sword was stolen.\n");
  write("default alias is ;\n\n");
  return 1;
}

