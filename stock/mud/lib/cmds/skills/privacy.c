#include <ansi.h>


status main(string targ) {
  int i, x;
  string file;

    log_file("SPELL",this_player()->query_real_name()+" privacy rune "+
       this_player()->query_current_room()+" @ "+ctime()+"\n");
  if(targ != "seal"){
     write("Privacy Seal?\n");
    return 1;
  }

  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);

  this_player()->load_spell(({
  "target",            environment(this_player()),
  "name",              "privacy seal",
        "stat",        "intellect",
        "type",        "focus",
  "cost",              50,
  "level",             50,
  "cast time",         5,
  "spell object",      file,
  "damage",            -1,
  "casting msg",       "You begin to create a new seal.\n",
  "passive",
  }));
  return 1;
}


/*******************************************************************/


status cast_spell(object caster,object target,object prev,int dmg) {
  tell_room(target, "The Seal of Privacy is here.\n");
  target->set_no_track(5);
  return 1;

}  
status id(string str) {
  return str == "spell"
      || str == "magic"
      || str == "seal";
}

void dispel_magic() {
   write("The seal begins to fade..\n");
   destruct(this_object());
}

void extra_long() {
  write(GREY_F+"The Seal of Privacy is here.\n"+NORM);
}


