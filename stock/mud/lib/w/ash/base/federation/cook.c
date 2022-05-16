// Ash

#include <mudlib.h>
#include <ansi.h>
#include "path.h"
inherit BARKEEP;
int               c,r,a,w,x,level,gender;
int a             = random(sizeof(ARM));
int w             = random(sizeof(WEAP));
int gender        = random(2)+1;
int level         = 60;
reset (arg){
   if(arg) return;
   ::reset(arg);
  set_name("cook");
  set_alias(RACE);
  set_short(COLOR+RACE+" cook"+NORM);
  set_long(COLOR+"Before you stands the "+RACE+" cook.\n"); 
  set_race(RACE);
  set_gender(gender);
  set_level(level);
  load_chat(5,({
  COLOR"Cook says, I got ale, jerky and water, whats ya want?\n"NORM,
  COLOR"Cook says, You don't look much like the others.\n"NORM,
  COLOR"Cook says, We don't have a mealplan, what do you want?\n"NORM,
  }));
  add_drink("ale", "glass of ale", "AH.. imported.",25);
  add_food("jerky", "beef jerky", "It's chewy.",25);
  add_sdrink("water", "cup of water", "AH.. that was good.",25);
  add_money(1500+random(1000));
  add_object(WEAP[w],PATH+"obj/shield");
  if(!random(6)){ add_object(ARM[a],PATH+"obj/"+ARM[a]);     }
  if(!random(6)){ add_object(WEAP[w],PATH+"obj/"+WEAP[w]);   }
}
