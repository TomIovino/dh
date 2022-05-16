#include <ansi.h>
inherit "inherit/tell_all";
string *query_cmd_aliases() { return ({"nuke", }); }

int main(string who) {
    string *spells;
    mapping ss;
    int s, i;
    object favored;
    if(who)
    favored = find_player(who);
    if(!favored){ write("cant find target.\n"); return 1; }

    tell_all(B_RED_F+"Ash throws a meteor across the sky.\n"+
 NORM+MAGENTA_F+"A mushroom cloud appears where "+capitalize(who)+" was standing.\n"
    +B_BLUE_F+"All is quiet.. for now.\n");
/*
    tell_all(B_WHITE_F+"Ash flies dangerously close over"  
    +" where "+capitalize(who)+" is standing.\n"
 +GREY_F"Ash bombs "+capitalize(who)+" with a special gift from heaven.\n"
    +B_WHITE_F+"All is quiet.. for now.\n");
*/
    favored->save_me(1);
    favored->quit_2();
    return 1;
}

