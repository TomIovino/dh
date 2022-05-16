#include <ansi.h>
#include <mudlib.h>
#define TPQN this_player()->query_alt_name()
#define TP this_player()
string *query_cmd_aliases() { return   
  ({ "sit","land","wake","sleep","rest","stand","kneel", "fly"}); }

int main(string what);
int second(string what);
int result();

int main(string what){
    string poss; 
    what = query_verb();
    poss = this_player()->query_position();

     switch(what){
      case "fly":
        if(this_player()->query_var("wings")){
            write("You spread your wings and begin to fly.\n");
            return 1;
        }
            write("You dont seem to have any wings.\n");
            return 1;
        break;
      case "stand":
      case "land":
      case "wake":
         if(this_player()->query_position() == "standing"){
             write("You are already standing.\n");
             return 1;
         }
        write("You stand up.\n");
        say(TPQN+" stands up.\n");
          this_player()->set_position("standing");
         return 1;
         break;
      case "kneel":
         if(this_player()->query_position() == "kneeling"){
             write("You are already kneeling.\n");
             return 1;
         }
        write("You kneel on the floor.\n");
        say(TPQN+" kneels on the floor.\n");
          this_player()->set_position("kneeling");
         return 1;
         break;
      case "sleep":
         if(this_player()->query_position() == "sleeping"){
             write("You are already sleeping.\n");
             return 1;
         }
        write("You lie down and sleep.\n");
        say(TPQN+" lays down and sleeps.\n");
          this_player()->set_position("sleeping");
              return 1;
      case "rest":
      case "sit":
         if(this_player()->query_position() == "resting"){
             write("You are already resting.\n");
             return 1;
         }
        write("You sit down to rest.\n");
        say(TPQN+" sits down to rest.\n");
          this_player()->set_position("resting");
              return 1;
        return 1;
        break;
       return 1;
    }
       return this_object()->help();
}

