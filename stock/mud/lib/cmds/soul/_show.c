#include <soul.h>
 
status emote(string str) {
    string ansi;
    object ob;

    if(!(ob = present(str, this_player()))) {
        write(NORM+"You show everyone your " + str + ".\n");
        say(NORM+TPNAME + " shows everyone " + POSS + " " +str + ".\n");
        return 1;
    }

    write("You show everyone your " + (string) ob->short() + ".\n");
    say(TPNAME+" shows everyone "+POSS+" "+(string) ob->short()+".\n");
    return 1;
}

string channel_msg(string str) {
    string ansi;
    object ob;
 
//       return (TPNAME + " shows everyone everything "+PRO+" has.");
    
    if(!(ob = present(str, this_player()))) {
        return (TPNAME + " shows everyone " + POSS + " " +str);
    }
    return (TPNAME + " shows everyone " + POSS + " " + ob->short());
}

