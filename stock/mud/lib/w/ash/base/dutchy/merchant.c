// Ash

#include <mudlib.h>
#include <ansi.h>
#include "path.h"
inherit SHOPKEEP;
int               c,r,a,w,x,level,gender;
int a             = random(sizeof(ARM));
int w             = random(sizeof(WEAP));
int gender        = random(2)+1;
int level         = 60;
reset (arg){
  ::reset(arg);
   if(arg) return;
    STOREROOM = PATH+"warehouse";
    SIGN_TEXT = capitalize(query_name())+" says, what do you want?";
  set_name("merchant");
  set_alias(RACE);
  set_race(RACE);
  set_short(COLOR+RACE" merchant"+NORM);
  set_long(COLOR+
  "Before you stands the "+RACE+" merchant.\n");
  set_gender(gender);
  set_level(50);
  add_money(1500+random(1500));
  add_object(ARM[a],PATH+"obj/shield");
  if(!random(4)) add_object(WEAP[w],PATH+"obj/"+WEAP[w]);
  if(!random(4))  add_object(ARM[a],PATH+"obj/"+ARM[a]);
  load_chat(5,({
  COLOR"Merchant asks, Got any trinkets I could purchase from you?\n"NORM,
  COLOR"Merchant smiles at your belongings.\n"NORM,
  }));
}
