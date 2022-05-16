#include <ansi.h>
#define CAST (string)caster->query_name()
#define TARG (string)target->query_name()

status main(string targ, mixed alt_type) {
    string file;
    int i, cl;

    file = file_name(this_object());
    sscanf(file,"%s#%d",file,i);

    if(environment(this_player())->query_exit_filename("castle")){
          return 0;
    }
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
   string myhouse;
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
    
    myhouse = this_player()->query_house();
    if(file_size("/d/castle/"+myhouse+"/b.c") > 0){
    this_player()->move_player("X#/d/castle/"+myhouse+"/b");
    destruct(this_object());
    return 1;
    }
        write("Cannot find your house castle!\n");
       destruct(this_object());
        return 1;

}




query_no_save(){ return 1; }

