string *query_cmd_aliases() { return
({"p", }); }


#include <ansi.h>
#include <mudlib.h>

status main(string arg) {
    object who;
    if(!this_player()->query_skill("peek")){
        write("You dont seem to have that talent..\n");
        return 1;
      }
    if(!arg) {
	write("peek who?\n");
	return 1; 
      }
    who=present(arg,environment(this_player()));
    if(!who) {
	printf("peek who?\n");
	return 1; }
    if(who->query_invis() || who->query_security_level() || !who->query_hp()){
        printf("peek who?\n");
        return 1; }
    this_player()->checkskill("peek","dexterity",5);
    if(this_player()->query_skill("peek") < 
              who->query_level())
    tell_object(who,this_player()->query_cap_name()+
              " just peeked at your inventory!\n");
    write("You peek at "+arg+"'s inventory.\n");
    if(who->show_inventory()==1) {
        return 1; }
    write(who->show_inventory()+".\n");
    return 1;
}

