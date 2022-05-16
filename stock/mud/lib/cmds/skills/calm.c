#include <ansi.h>
#define CAST (string)caster->query_name()
#define TARG (string)target->query_name()

status main(string targ){
    string file;
    int i, cl;

    file = file_name(this_object());
    sscanf(file,"%s#%d",file,i);

    this_player()->load_spell(({
	"name",              "calm",
	"cost",              30,
        "level",             50,
        "type",              "focus",
        "stat",              "wisdom",
        "target",            targ,
	"cast time",         3,
	"spell object",      file,
            "passive", 1,
        "damage",           -1,
      }));
    return 1;
}


/*********************************************************************/
/* when cloned to player */



status cast_spell(object caster,object target,object prev,int dmg) {
   int dam; 

    if(target->query_level()+1 >
     random(caster->query_level())+caster->query_security_level()){
	write("You lose concentration!\n");
	destruct(this_object());
	return 1;
    }
    write("You calm "+target->query_alt_name()+"!\n");
    say(caster->query_alt_name()+" calms "+
        target->query_alt_name()+"!\n");
    target->stop_fight();
    destruct(this_object());
    return 1;
}




query_no_save(){ return 1; }

