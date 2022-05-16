#include <ansi.h>
#define CAST (string)caster->query_name()
#define TARG (string)target->query_name()

status main(string targ){
    string file;
    int i, cl;

    file = file_name(this_object());
    sscanf(file,"%s#%d",file,i);

    this_player()->load_spell(({
	"name",              "tame",
	"cost",              40,
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

    if(target->query_level()+20 >
        caster->query_level()+caster->query_security_level()){
	write("You lose concentration!\n");
	destruct(this_object());
	return 1;
    }
    if(target->is_npc()){
	write("You lose concentration!\n");
	destruct(this_object());
	return 1;
    }
    write("You have tamed "+target->query_alt_name()+"'s beast.\n");
    say(caster->query_alt_name()+" has sent "+
        target->query_alt_name()+"'s beast away.\n");
    target->set_var("dragonkin",0);
    target->set_var("fangs",0);
    target->set_var("wings",0);
    target->unset_var("sight");
    target->set_var("claws",0);
    target->set_var("sight",0);
    target->set_var("scales",0);

    return 1;
}




query_no_save(){ return 1; }

