// sentry has low chance for a weapon+armour, no spells, low cash

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
  set_name("sentry");
  set_alias(RACE);
  set_race(RACE);
  set_short(COLOR+RACE" sentry"+NORM);
  set_long(COLOR+
  "Before you stands the "+RACE+" sentry.\n"); 
  set_gender(gender);
  set_level(50);
  add_money(1200+random(600));
  load_chat(5, 
  ({ COLOR+"Sentry scans the area for intruders.\n"NORM }));
  if(!random(4)) add_object(WEAP[w],PATH+"obj/"+WEAP[w]);
  if(!random(4))  add_object(ARM[a],PATH+"obj/"+ARM[a]);     
}
