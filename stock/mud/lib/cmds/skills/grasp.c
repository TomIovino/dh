// This is one of the four basic elemental attack spells.
// Ash 7.99


#define NAME "@@query_name:$this_player()$@@"
#define TARG "@@query_name:$this_object()$@@"  
#include <ansi.h>
#define COLOR this_player()->query_color()   
#define ELEMENT this_player()->query_magic()   
#define SPELLNAME "grasp"


status main(string targ, mixed alt_type, mixed spell_type){
    int stat, level, skill, dam, mod;  
    string desc; 
    level = this_player()->query_level();
    stat = this_player()->query_stat("intellect"); 
    skill = this_player()->query_skill(SPELLNAME); 
    desc = ""; 
    mod = (stat+skill)/50;
    dam = ( random(stat+skill+level)+stat+skill+level ) / 6;
    if(mod >= 4) desc = "an anguishing"; 
    if(mod == 3) desc = "a tormenting"; 
    if(mod == 2) desc = "an agonizing"; 
    if(mod <= 1) desc = "a painful"; 
    this_player()->load_spell(({
	"target", targ,
	"name",   SPELLNAME,
	"stat",   "intellect",
	"type",   "primal",
	"cost",   20,
	"damage", dam,
	"immune", SPELLNAME,
	"level",  30,
	"cast time", 1,
	"msg target", 
        COLOR+NAME+" grasps you with "+desc+" "+ELEMENT+" hold!\n"+NORM,
	"msg room", 
        COLOR+NAME+" "+ELEMENT+" grasps "+TARG+" with "+desc+" hold!\n"+NORM,
	"msg caster", 
        COLOR+"Your grasp "+TARG+" with "+desc+" "+ELEMENT+" hold!\n"+NORM,
      }));
    return 1;
}
