
#include <ansi.h>

object portal;
status main(string str, mixed alt_type) {
    string file;
    int i, cl;
    int x;
    if(!str) {
	notify_fail("visit who?\n");
	return 0;
    }

    file = file_name(this_object());
    sscanf(file,"%s#%d",file,i);

    this_player()->load_spell(({
	"target",            this_player(),
	"name",              "visit",
	"cost",              60,
	"level",             75,
	"cast time",         3,
	"spell object",      file,
  "stat",              "wisdom",
  "type",              "focus",
	"argument",          str,
	"passive",
        "casting msg", ({
                    "You play with inter-dimensional forces.\n",
                    "You try and focus your mind on your target.\n",
                    "A portal starts to form.\n",
                     })
      }));
    return 1;
}

status cast_spell(object caster,object target,object prev,int dmg) {
    object ob;
    string rname;
    object env;
    rname = (string)caster->query_spell_argument();
    ob=find_player(rname);
    if(!ob) {
	write("You cannot locate "+ capitalize(rname) +".\n");
	destruct(this_object());
	return 1;
    }
    if(ob->query_security_level() > 0) {
	write("You cannot locate "+ capitalize(rname) +".\n");
	destruct(this_object());
   	return 1; 
    }
    
    if(ob->query_invis() && ob->query_security_level() <= 0) {
	write("You cannot locate "+ capitalize(rname) +".\n");
	destruct(this_object());
	return 1;
    }
    if(environment(this_player())->query_no_summon()) {
      write("Something in your area prevents you from summoning.\n");
      return 1;
    }
    if(environment(ob)) {
	if(environment(ob)->query_no_teleport()
	  || environment(ob)->query_no_summon()) {
	    write("Something in their area prevents you from summoning.\n");
	    destruct(this_object());
	    return 1;
	}
    }
   
    if(ob->query_primary_attack()) {
	write("You lose your concentration!\n");
	destruct(this_object());
	return 1;
    }
/*
    if(this_player()->query_alignment() != ob->query_alignment()){
     write("You can only use this magic with players of your realm.\n");
       return 1;
    }
*/
    if(ob->query_env_var("VISIT") != 1){
	write("You cannot visit a player without permission.\n");
	destruct(this_object());
   	return 1; 
    }
    env = environment(ob);
    this_player()->move_player("X#"+(string)env);

    log_file("MOVE",sprintf(
             "[%s] Room:  %s\n\tCaster:  %O\n\tVictim:  %O\n\n",
             ctime(), file_name(environment(caster)), caster, ob));
 
    destruct(this_object());
    return 1;
}

query_no_save(){ return 1; }

