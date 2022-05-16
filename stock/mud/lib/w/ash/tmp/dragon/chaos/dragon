// God on Drugs, this is Ash's code. June 1, 1996

#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;
inherit "inherit/tell_all";

#define PATH   "/w/ash/dragon/redsteel/"
#define CLONE    ({ "helm","boots","gloves","sleeves","pants",\
                  "shield","armour","belt","amulet","ring",\
                  "cloak","floating","ring", })
int c;
int c             = random(sizeof(CLONE));
reset (arg){
   ::reset(arg);
   if(arg) return;
   set_name("SPAWN");
   set_dead_ob(this_object());
   set_short(B_RED_F+"Ancient Fire Dragon");
   set_long(B_RED_F+
   "The words of a poem about death race through your mind.\n"+
   "A spectrum of red scales shine brightly on the ancient\n"+
   "skin of Cinder, You feel sick with fear.\n"NORM);
  set_race("dragon");
  set_aggressive(1);
  set_gender(1);
  load_spells(10, ({
   "heal","stoneskin","holy ground","blast",
   "storm","heal","curse","dispel",
  }));
  load_a_chat(10, ({ 
    B_RED_F+"Cinder whips his tail around nearly hitting you!\n" }));
   set_level(150);
   add_object(CLONE[c],PATH+CLONE[c]);
   add_object("dagger",PATH+"artifact");
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
                dam = -100;            // ..between
                dam += random(50);     //    100-50 points damage
                a[i]->adj_hp(dam);
           tell_object(a[i],
    B_RED_F"Cinder breaths FIRE breath all over you!\n"+NORM);
                if(a[i]->query_hp()==0){
                   a[i]->death();
                }
                 primary_attack = a[i];
              return;
            }
        }
    }
}


monster_died() {
  say(NORM+MAGENTA_F+"The dragon screams a death cry.\n");
  tell_all(NORM+MAGENTA_F+"A dragon screams a death cry in the distance.\n");
  return 0;
}

