// caster has spells, chance of weapon+armour, cash!

#include <mudlib.h>
#include <ansi.h>
#include "path.h"
inherit MONSTER;

int               a,w,x,level,gender;
int a             = random(sizeof(ARM));
int w             = random(sizeof(WEAP));
int gender        = random(2)+1;

reset (arg){
   if(arg) return;
   ::reset(arg);
  set_name("caster");
  set_alias(RACE);
  set_race(RACE);
  set_short(COLOR+RACE" caster"+NORM);
  set_long(COLOR+
  "Before you stands a practicing "+RACE+" caster.\n"); 
  set_gender(gender);
  set_level(50);
  add_money(1200+random(600));
  load_chat(5, 
  ({ COLOR+"Caster mumbles some incantations.\n"NORM }));
  load_spells(15, ({ "heal","grasp","flash","jolt" }));
  if(!random(3)){ add_object(ARM[a],PATH+"obj/"+ARM[a]);     }
  if(!random(3)){ add_object(WEAP[w],PATH+"obj/"+WEAP[w]);   }
}
