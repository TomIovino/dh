
#include <mudlib.h>
#include <ansi.h>
#include <bar.h>
#include <house.h>

string *query_cmd_aliases() { return ({"yell","shout","gossip", }); }

status main(string str) {
    object who;
    string chan, name;

    chan = this_player()->query_alignment();
    name = (string) this_player()->query_real_name();

    if(!str || !stringp(str)) {
        command("*"+chan,this_player());
        return 1;
    }

    if(str == "-h") {
        command("*"+chan+" -h",this_player());
        return 1;
    }
    command(sprintf("*%s %s",chan,str),this_player());
    return 1;
}

status help() {
    printf("      Command:\n"
           "         chat -h     ->  will show the chat history\n"
           "         chat blah   ->  [common] Name : blah\n"
           "         chat *blah  ->  [common] Name blah\n"
           "         chat ;smile ->  [common] Name smiles happily.\n");
    return 1;
}

