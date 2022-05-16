// God on Drugs, this is Ash's code. June 1, 1996

#include <mudlib.h>
#include <ansi.h>
inherit        MONSTER;
inherit        "inherit/tell_all";
#define KI     "chaos"
#define CC     B_YELLOW_F
reset (arg){
   ::reset(arg);
   if(arg) return;
   set_name("spawn");
   set_short(CC"Ancient Chaos Dragon");
/*
   set_dead_ob(this_object());
*/
   add_object("artifact","/w/ash/base/art/"+KI);
   set_long(CC+
   "The words of a poem about death race through your mind.\n"+
   "A spectrum of white scales shine brightly on the ancient\n"+
   "skin of "+capitalize(query_name())+". You feel sick with fear.\n"NORM);
  set_race("dragon");
  set_alignment(KI);
/*
  set_wander(50, 30, 0, "/w/ash");
*/
  set_aggressive(1);
  set_gender(1);
  load_spells(7, ({
  "heal","heal","grasp","storm","spike","absorb","stun",
  }));
  load_a_chat(10, ({ 
  CC+capitalize(query_name())+" whips his tail around nearly hitting you!\n"+NORM }));
  set_level(175);
  add_money(random(5000)+5000);
  set_magic_resist(50);
}

void attack() {
    mixed *a;
    int i,dam;
    object victim;
    ::attack();
    ::attack();
    if(!(victim = query_primary_attack()) || 
    !present(victim, environment())) return;
    if(random(100) <= 10) {
        a=all_inventory(environment());
        for(i = 0; i < sizeof(a); i++) {
            if(living(a[i]) && (!a[i]->query_invis()) &&
            a[i]!=this_object()) {
              dam = -200;            // ..between
              dam += random(50);     //    200-150 points damage
              a[i]->adj_hp(dam);
              tell_object(a[i],
            CC+capitalize(query_name())+" breathes poisonous gas!\n"+NORM);
              if(a[i]->query_hp()==0){
                   a[i]->death();
               }
                 primary_attack = a[i];
              return;
            }
        }
    }
}

death_2() {
  say(NORM+MAGENTA_F+"The dragon screams a death cry.\n");
  tell_all(NORM+MAGENTA_F+"A dragon screams a death cry in the distance.\n");
  return 0;
}

