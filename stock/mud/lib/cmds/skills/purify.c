
#include <ansi.h>
#define CAST (string)caster->query_name()
#define TARG (string)target->query_name()

status purify(string targ, mixed alt_type) {
  string file;
  int i,x;


  file = file_name(this_object());
  sscanf(file,"%s#%d",file,i);

  this_player()->load_spell(({
  "target",            targ,
  "name",              "purify",
  "stat",              "wisdom",
  "type",              "focus",
  "level",             25,
  "cost",              50,
  "cast time",         2,
  "spell object",      file,
  "passive",
  "casting msg", YELLOW_F"You begin to cast purify\n",
  }));

  return 1;
 }


status cast_spell(object caster,object target,object prev,int heal) {
  object ob;
  int wis_mod;
  wis_mod = caster->query_stat("wisdom");
   target->unset_var("blind");
   target->unset_var("curse");
   target->unset_var("poison");
   target->unset_var("weaken");
   target->unset_var("deafness");
   target->unset_var("slow");
   target->unset_var("disease");
   target->remove_scar();
   target->dispel_magic();
   write("Your spell was successfull.\n"NORM);
  return 1;
}
int query_no_save(){ return 1;}

