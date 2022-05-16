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
  set_name("guard");
  set_alias(RACE);
  set_race(RACE);
  set_short(COLOR+RACE+" guard"+NORM);
  set_long(COLOR+
  "Before you stands a vigilant "+RACE+" sentry.\n"); 
  set_gender(gender);
  set_level(50);
  load_chat(5, 
  ({ COLOR+capitalize(RACE)+" examines you closely.\n"NORM }));
  add_money(((random(100)+200)*2)+1);
  if(!random(3)){ add_object(ARM[a],PATH+"obj/"+ARM[a]);     }
  if(!random(3)){ add_object(WEAP[w],PATH+"obj/"+WEAP[w]);   }
}
