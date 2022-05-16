#include <ansi.h>

#define ELEMENT this_player()->query_alignment()
#define MAGIC this_player()->query_magic()
#define SPELL "jolt"
#define TPQN this_player()->query_alt_name()
#define COLOR  ({ BROWN_F,MAGENTA_F,BLUE_F,RED_F,GREEN_F,YELLOW_F,CYAN_F, })

inherit "/inherit/objs/charmed_npc";

int c;
string color;

string short(string x){
  c             = random(sizeof(COLOR));
  color = COLOR[c];
  return color+"ethereal defender shield"+NORM;
}


void reset(status arg) {
    ::reset(arg);
    if (arg) return;
}


status cast_spell(object caster,object target,object prev,int level) {
  int type, number;
  object summoned;
  int i,lev;
  lev = caster->query_stat("charisma");
  lev += caster->query_skill("defender");
  lev = (lev+1)/4;
  if(present("defender")){
    write("Something prevents the summoning.\n");
    destruct(this_object());
    return 1;
  }
  if(environment(caster)->query_no_summon()) {
    write("Something prevents the summoning.\n");
    destruct(this_object());
    return 1;
  }
      set_level(lev);
      set_name("defender");
      set_race(MAGIC);
      set_no_exp(1);
      set_alias("summoned");
      set_gender(0);
      set_magic_resist(lev);
      load_spells(10, ({ SPELL }));
      add_spell_immune(SPELL);
      load_chat(5, ({
      }));

  clone_object("obj/shadows/follow")->follow(caster,this_object());
  return ::cast_spell(caster,target,prev,level);
}
  int query_carry_percent() { return 50; }

