#define TP this_player()
/* Enchant  */

#include <ansi.h>
#define NAME "@@query_name:$this_player()$@@"
#define COLOR B_CYAN_F
int wc,ac;
status enchant(string targ, mixed alt_type) {
  int level, i,x;
  string file;
  if(!targ) {
    notify_fail("enchant <item>?\n");
    return 0;
  }
  file = file_name(this_object());
  sscanf(file,"%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "enchant",
  "cost",              50,
  "level",             50,
  "type",              "primal",
  "stat",              "intellect",
  "damage",            level,
  "cast time",         2,
  "spell object",      file,
  "casting msg",       "You chant very slowly...\n",
  "casting msg room",  NAME +" chants slowly...\n", 
  "passive",
  }));
  return 1;
}


status cast_spell(object caster,object target,object prev,int level) {
  string sh,lo, prep;
  int en;
  if(living(target)) {
    write(target->query_name() +" is not a weapon!\n");
    destruct(this_object());
    return 1;
  }
  if(target->query_wc()) wc = target->query_wc();
  if(target->query_ac()) ac = target->query_ac();

  if(!wc){
    write(target->query_name() +" is not a weapon!\n");
    destruct(this_object());
    return 1;
  }
  if(target->query_enchant()) {
    write(target->query_name() +" is already enchanted!\n");
    destruct(this_object());
    return 1;
  }
  write(MAGENTA_F"You hold "+ target->query_name() +
  " firmly in your hands and enchant it.\n"NORM);
    say(caster->query_name()+" grasps "+ target->query_name() +" in "+ 
      caster->query_possessive() +" hands and enchants it.\n");


   prep = this_player()->query_skill("enchant");
   en = 1;
   if(prep > 19) en = 2;
   if(prep > 39) en = 3;
   if(prep > 59) en = 4;
   if(prep > 79) en = 5;

   if(en < 1)     en = 1;
   if(en > 5)     en = 5;

   target->set_data("enchanter",TP->query_name());
   if(wc) target->adj_enchant(en);
   if(wc) target->set_light(5);
   if(target->query_weight() > en+1)
   target->set_weight(target->query_weight()-en);
   caster->recalc_wc();
   caster->recalc_ac();
   caster->recalc_carry();
   destruct(this_object());
  return 1;
}

