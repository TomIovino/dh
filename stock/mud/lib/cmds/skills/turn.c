#include <ansi.h>
#define CAST (string)caster->query_name()
#define TARG (string)target->query_name()

status main(string targ){
    string file;
    int i, cl;

    file = file_name(this_object());
    sscanf(file,"%s#%d",file,i);

    this_player()->load_spell(({
	"name",              "turn",
	"cost",              25,
        "level",             50,
        "type",              "focus",
        "stat",              "wisdom",
        "target",            targ,
	"cast time",         3,
	"spell object",      file,
        "damage",           -1,
        "passive", 1,
      }));
    return 1;
}


/*********************************************************************/
/* when cloned to player */



status cast_spell(object caster,object target,object prev,int dmg) {
   int dam; 

  if(!caster->query_security_level()){
   
   if(environment(this_player())->query_no_teleport()
      || environment(this_player())->query_no_fight()
      || environment(this_player())->query_no_summon()) {
	write("Something in the area prevents it.\n");
	destruct(this_object());
	return 1;
    }
    if(target->query_level() > 50 &&
        target->query_level()+25 >= this_player()->query_level())
    if(target->query_level() >= this_player()->query_level() ||
        target->query_security_level()){
	write("You lose concentration!\n");
	destruct(this_object());
	return 1;
     }
  }

  if(target->is_npc()){
	write("You lose concentration!\n");
	destruct(this_object());
	return 1;
  }
   log_file("MOVE",
      this_player()->query_real_name()+
      " turned "+target->query_alt_name()+"  "+
         "on "+ctime()+"  "+
      target->query_current_room()+"\n");
    write("You have sent "+target->query_alt_name()+" away.\n");
    say(caster->query_alt_name()+" has sent "+
        target->query_alt_name()+" away.\n");
    target->stop_fight();
    target->church();
    target->set_mp(10);
    destruct(this_object());
    return 1;
}




query_no_save(){ return 1; }

