#include <ansi.h>

string *query_cmd_aliases() { return ({"blah","'","speak", }); }

status main(string str) {
    string prefix;
               string temp;
    if(str == "" || str == " ") {
        write(this_player()->do_cc("You mumble incoherently.\n"));
        say(this_player()->do_cc(this_player()->query_alt_name() + " mumbles incoherently.\n"));
        return 1;
    }

    prefix  = (sscanf(str,"%s!",temp))   ? " exclaim"
    : (sscanf(str,"%s?",temp))           ? " ask" : " say";
        
    write(this_player()->do_cc("You"+prefix+" : "+str+NORM+"\n"));
    say(this_player()->do_cc(this_player()->query_alt_name() + 
    prefix+"s : "+str+NORM+"\n"));
 
    return 1;
}



/* something to reintergrate when i have more time.
switch (speak_language) {
 case "elf" : conf = "musical language"; break;
 case "human" : conf = "rolling tounge"; break;
 case "dwarf" : conf = "rough langauge"; break;
 case "giant" : conf = "thunderous language"; break;
 case "halfling" : conf = "cheerful langauge"; break;
 case "goblin" :
 case "orc" : conf = "primative language"; break;
 case "drow" : conf = "menacing language"; break;
 case "ogre" : conf = "harsh language"; break;
 case "troll" : conf = "ancient language"; break;
 default : conf = "language"; break;
 }
conf = sprintf("%s speaks in a %s which you cannot understand.\n",
query_name(), conf); tell_object(env[i], conf);
*/

