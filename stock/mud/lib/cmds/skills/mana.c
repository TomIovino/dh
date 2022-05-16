#include <ansi.h>


status main(string targ) {
  int i, x,y;
  string file;

  if(targ != "rune"){
     write("Mana Rune?\n");
    return 1;
  }
  if(sscanf(this_player()->query_current_room(), "%s/arena/%s", x,y)== 2){
      return 1;
      }


  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);

  this_player()->load_spell(({
  "target",            environment(this_player()),
  "name",              "mana rune",
  "stat",              "wisdom",
  "type",              "focus",
  "cost",              75,
  "level",             75,
  "cast time",         5,
  "spell object",      file,
  "damage",            -1,
  "casting msg",       "You begin to create a new rune.\n",
  "passive",
  }));
  return 1;
}


/*******************************************************************/


status cast_spell(object caster,object target,object prev,int dmg) {
  tell_room(target, "The rune of mana is here.\n");
  return 1;

}  
status id(string str) {
  return str == "spell"
      || str == "magic"
      || str == "rune"
      || str == "mana rune";
}

void dispel_magic() {
   write("The rune begins to fade..\n");
   destruct(this_object());
}

void extra_long() {
  if(!present("health rune") && present("movement rune"))
  write(NORM+MAGENTA_F+"The Rune of Movement and Mana is here.\n"+NORM);
  else if(!present("health rune"))
  write(NORM+MAGENTA_F+"The Rune of Mana is here.\n"+NORM);
  if(present("mana rune 2")) call_out("dispel_magic",1);
   
}


