#include <ansi.h>
#include <mudlib.h>
#define TPQN this_player()->query_alt_name()

#define QTT this_player()->query_attrib("QTT")
#define STT this_player()->set_tmp_attrib("QTT",time()+2)

string *query_cmd_aliases() { return
     ({ "stance","casting",
        "cas","relax","relaxed","offense","stealth",
        "ste","defense","off","def" }); }

int main(string what);
int second(object who, string what);
int help();
int result();

int main(string what){
    STT;
   if(!what) what = query_verb();
    call_out("second",3,this_player(),what);
    write("You try to focus your concentration.\n");
    return 1;
}


int second(object who, string what){
    string poss; 
   if(QTT > time()){
      write("Unable gain tactical advatage!\n");
      return 1;
    }

    poss = who->query_possessive();
//   write((string)what+(string)who+(string)poss);
     switch(what){
      case "relax":
      case "relaxed":
        write("You concentrate on being relaxed.\n");
          who->set_tactic("relaxed");
         return 1;
         break;
      case "stealth":
      case "stea":
      case "ste":
        write("You concentrate on stealth.\n");
          if(!who->query_skill("stealth tactics")){
              write("You stop concentrating.\n"); 
              return 1;
           }
          who->set_tactic("stealth");
         return 1;
         break;
      case "defense":
      case "def":
        write("You concentrate on defensive tactics.\n");
          if(!who->query_skill("defensive tactics")){
              write("You stop concentrating.\n"); 
              return 1;
           }
          who->set_tactic("defensive");
         return 1;
         break;
      case "offense":
      case "off":
        write("You concentrate on offensive tactics.\n");
          if(!who->query_skill("offensive tactics")){
              write("You stop concentrating.\n"); 
              return 1;
           } 
          who->set_tactic("offensive");
         return 1;
        break;
      case "casting":
      case "cast":
      case "cas":
        write("You concentrate on casting.\n");
          if(!who->query_skill("casting tactics")){
              write("You stop concentrating.\n"); 
              return 1;
           } 
          who->set_tactic("casting");
         return 1;
        break;
        default:
        return help();
        break;
       return 1;
    }
}

int help(){
     if(this_player()->query_tactic() != "0")
     write("You are currently using "
           +this_player()->query_tactic()+" tactics.\n");
     write("<offense/defense/casting/stealth>\n");
     write("See 'help tactics'.\n");
     return 1;
}

// You arch into a tiger stance.
// You swing into a dragon stance.
// You squat into a mantis stance.
// You twist into a viper stance.
// You hunch into a monkey stance.


