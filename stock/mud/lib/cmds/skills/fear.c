#include <ansi.h>
#define CAST (string)caster->query_name()
#define TARG (string)target->query_name()

status main(string targ){
    string file;
    string x,y;
    int i, cl;

    file = file_name(this_object());
    sscanf(file,"%s#%d",file,i);
   if(sscanf(this_player()->query_current_room(), "%s/castle/%s", x,y) == 2) {
    write("Not to castle rooms.\n");
    return 1;
   }

    this_player()->load_spell(({
	"name",              "fear",
	"cost",              50,
        "level",             50,
        "type",              "primal",
        "stat",              "wisdom",
        "target",            targ,
	"cast time",         3,
	"spell object",      file,
        "damage",           -1,
      }));
    return 1;
}


/*********************************************************************/
/* when cloned to player */



status cast_spell(object caster,object target,object prev,int dmg) {
   int dam; 

   if(environment(this_player())->query_no_fight()
    ){ write("Not here.\n"); return 1; }
   if(environment(this_player())->query_no_teleport()
      || environment(this_player())->query_no_summon()) {
	write("Something in the area prevents you from teleporting.\n");
	destruct(this_object());
	return 1;
    }
    if(target->query_level()+10 >
        caster->query_level()+caster->query_security_level()){
	write("You lose concentration!\n");
	destruct(this_object());
	return 1;
    }
    if(  target->query_name() == "guard" ||
         target->query_race() == "guard"  ||
         target->query_alias() == "guard" ){
             write("The guard has no fear of you!\n");
            return 1;
     }
    write("You scare "+target->query_alt_name()+"!\n");
    say(caster->query_alt_name()+" scares "+
        target->query_alt_name()+"!\n");
    target->set_position("standing");
    target->random_move();
    target->random_move();
    destruct(this_object());
    return 1;
}




query_no_save(){ return 1; }

