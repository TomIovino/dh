// Ash 7.99


#define NAME "@@query_name:$this_player()$@@"
#define TARG "@@query_name:$this_object()$@@"  
#include <ansi.h>
#define COLOR this_player()->query_color()   
#define ELEMENT this_player()->query_magic()   
#define SPELLNAME "spike"


status main(string targ, mixed alt_type, mixed spell_type){
    int stat, level, skill, dam, mod;  
    string desc; 
    level = this_player()->query_level();
    stat = this_player()->query_stat("intellect"); 
    skill = this_player()->query_skill(SPELLNAME); 
    desc = ""; 
    mod = (stat+skill)/50;
    dam = ( random(stat+skill+level)+stat+skill+level ) / 3;

    if(mod >= 4) desc = "A huge"; 
    if(mod == 3) desc = "A large"; 
    if(mod == 2) desc = "A medium"; 
    if(mod <= 1) desc = "A small"; 
    this_player()->load_spell(({
	"target", targ,
	"name",   SPELLNAME,
        "stat",   "intellect",
        "type",   "primal",
	"cost",   45,
	"damage", dam,
	"immune", SPELLNAME,
	"level",  50,
	"cast time", 2,
	"msg target", COLOR+desc+" "+ELEMENT+" spike springs from "+NAME
	+ "'s hand, striking you!\n"+NORM,
	"msg room", COLOR+desc+" "+ELEMENT+" spike springs from "+ NAME
	+ "'s hand, striking "  +TARG+ "!\n"+NORM,
	"msg caster", COLOR+desc+" "+ELEMENT+" spike springs from your hand " 
	+ "striking "+ TARG + NORM +"!\n",
      }));
    return 1;
}
