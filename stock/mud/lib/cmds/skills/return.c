#include <ansi.h>
#define CAST (string)caster->query_name()
#define TARG (string)target->query_name()

status main(string targ, mixed alt_type) {
    string file;
    int i, cl;

    file = file_name(this_object());
    sscanf(file,"%s#%d",file,i);

    this_player()->load_spell(({
	"name",              "return",
	"cost",              25,
        "level",             25,
        "stat",              "intellect",
        "type",              "primal",
	"cast time",         5,
	"spell object",      file,
        "damage",           -1,
	"passive",
      }));
    return 1;
}


/*********************************************************************/
/* when cloned to player */



status cast_spell(object caster,object target,object prev,int dmg) {
   int dam; 
   if(environment(this_player())->query_no_teleport()
      || environment(this_player())->query_no_summon()) {
	write("Something in the area prevents you from teleporting.\n");
	destruct(this_object());
	return 1;
    }


    if(this_player()->query_primary_attack()){
	write("You lose concentration!\n");
	destruct(this_object());
	return 1;
    }
    this_player()->church();
   log_file("MOVE",
      this_player()->query_real_name()+" Returned "+
        " on "+ctime()+"\n");

    destruct(this_object());
    return 1;
}




query_no_save(){ return 1; }

