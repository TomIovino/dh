#include <ansi.h>
#include <mudlib.h>

int nofight() {
    return environment()->query_no_fight();
}



status main(string arg) {
    object who;
    if(!this_player()->query_skill("taunt")){
        write("You dont seem to have that skill.\n");
        return 1;
      }
     if(!arg) {
	write("taunt who?\n");
	return 1; 
      }

    if((status)environment(this_player())->query_no_fight() == 1) {
       write("No.\n");
       return 1;
    }
    who=present(arg,environment(this_player()));
    if(!who) {
	printf("taunt who?\n");
	return 1; }
    if(who->query_invis()){
        printf("taunt who?\n");
        return 1; }
    if(who->query_primary_attack() == this_player()){
        write("No need!\n");
        return 1; }
    if(who == this_player()){
        log_file("TBUG", this_player()->query_real_name()+" "+ctime()+"\n");
        write("Try harder!\n");
        return 1;
    }
    this_player()->checkskill("taunt","primal",10);
    if(random(this_player()->query_skill("taunt")) <
       random(who->query_stat("focus"))){
      write("Try harder!\n");
      return 1;
    }
 
    tell_object(who,this_player()->query_cap_name()+
              " is really pissing you off!\n");
    write("You get "+who->query_alt_name()+"'s attention.\n");
      who->set_primary_attack(this_player());
      return 1;
}

