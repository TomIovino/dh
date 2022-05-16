
#define TP this_player()
#include <ansi.h>
inherit "inherit/timer";

object owner;

status main(string spell_type, mixed alt_type) {
  int i, time;
  string file;

    log_file("SPELL",this_player()->query_real_name()+" psychic rune "+
       this_player()->query_current_room()+" @ "+ctime()+"\n");
  if(spell_type != "rune") {
    notify_fail("psychic rune?\n");
    return 1;
  }

  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);

  this_player()->load_spell(({
  "target",            environment(this_player()),
  "name",              "psychic rune",
  "stat",              "wisdom",
  "type",              "focus",
  "cost",              50,
  "level",             50,
  "cast time",         3,
  "spell object",      file,
  "damage",            time,
  "casting msg",       "You tune into the area..\n",
  "casting msg room",  "You feel a link to this area.\n",
  "passive",
  }));
  return 1;
}


/*******************************************************************/
/* called when this is cloned to target */


status cast_spell(object caster,object target,object prev,int dmg) {
  owner = caster;
  tell_room(target, "The air shimmers about you.\n");
  adj_time(50);
  return 1;
}  

void time_out() {
  tell_object(environment(),"The psychic rune fades.\n");
  ::time_out();
}

void extra_long() {
  write(BOLD+WHITE_F+"A psychic rune is here.\n");
}

status id(string str) {
  return str == "spell"
      || str == "rune"
      || str == "psychic rune";
}

warn(string name){
     if(owner){
     if(this_player())
//   if(!this_player()->query_security_level()){
     if(!this_player()->query_invisable()){
       name = this_player()->query_alt_name();
     if(!name) name = "Someone";
     tell_object(owner,name+" is near your psychic rune.\n");
     }
//   }
  }
}

init(){
 string name;
 name = this_player()->query_alt_name();
 call_out("warn",1, name);
 }







 
