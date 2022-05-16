#define TP this_player()
#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;
#define RACE   ({ "dragonkin", })
#define SHORT  ({ "dragonkin", })
#define PATH      "/d/town/obj/hero/"
#define CLONE  ({ "helm","boots","gloves","sleeves","leggings","floating",\
                  "shield","armour","sword","belt","amulet","ring",\
                  "cloak","wrists","ankles",})
#define COLOR  ({  WHITE_F,MAGENTA_F,BLUE_F,RED_F,GREEN_F,YELLOW_F,CYAN_F, })
#define AD     ({ "strong", "mighty", "powerful", "wise", "famous", \
                  "legendary", "honorable",}) 
int               sk,sp,c,a,x,r,o,f,m,l,level,gender,notrace,train,res;
int o             = random(sizeof(CLONE));
int c             = random(sizeof(COLOR));
int res           = random(100);
int train         = random(30)+10;
int gender = 1;
int a             = random(sizeof(AD));
reset (arg){
   ::reset(arg);
   if(arg) return;
if(x == r) x      = x + 1;
if(x>sizeof(RACE)) x = x -2;
  set_name("daniel");
  set_alignment("divinity");
  set_alias("hero");
  set_short(COLOR[c]+"dragonkin hero"+NORM);
  set_long(COLOR[c]+
     "Clad in rune armour, here stands before you the "+AD[a]+"\n"+
     "hero of the dragonkin race, Daniel Runemaster. Chosen\n"
     "protector of the virtues of the Gods themselves.\n"+NORM);
  set_race("dragonkin");
  set_gender(gender);
  set_half_exp(1);
  set_tactic("offensive");
  set_special_chance(10);
  set_special_dam(10);
  load_spells(15, ({ "lance","heal","stun","flurry","circle", }));
  load_chat(5, ({
  "*smile","*bow","*greet","*heal","*exoskin","*bless","*barrier",
  "*heal","*reflect","*exoskin","*bless","*avenger","*defender",
  COLOR[c]+capitalize(query_name())+" whispers to you, The dragonkin "+
  "race is much more famous then your race.\n"+NORM,
  COLOR[c]+capitalize(query_name())+" says: "+
   "I've seen Dragons flying in the far reaches of the realms.\n",
  COLOR[c]+capitalize(query_name())+" says: "+
  "Have you heard of eight dragons of dawn?\n",
  COLOR[c]+capitalize(query_name())+" says: "+
  "Have you heard of eight dragons of dusk?\n", }));
   set_level(100);
   set_magic_resist(50);
  if(!random(10))
   add_object(CLONE[o],PATH+CLONE[o]);
   add_money(random(2626)+3000);
   set_env_var("NO_GIVE",0);
   set_env_var("AUTOLOOT",0);

}

init(){
    ::init();
    add_action("no","no");
    add_action("bow","bow");
    add_action("nod","nod");
    if(TP->query_quests("scroll runner")) return;
    write(COLOR[c]+"Hero bows.\n");
    write(COLOR[c]+"Hero says would you like a quest?\n");
    write(COLOR[c]+"Hero says, nod to me if you do.\n");
}

void attack() {
    mixed *a;
    int i,dam;
    object victim;

    ::attack();
    ::attack();
    if(!(victim = query_primary_attack()) ||
    !present(victim, environment())) return;
    if(!random(10)) {
        a=all_inventory(environment());
        for(i = 0; i < sizeof(a); i++) {
            if(living(a[i]) && (!a[i]->query_invis()) &&
            a[i]!=this_object()) {
                dam = -10;             // ..between
                dam += random(10);     //    10-20 points damage
                a[i]->adj_hp(dam);
           tell_object(a[i],
       YELLOW_F"Hero charges into you!\n"+NORM);
                if(a[i]->query_hp()==0){
                   a[i]->death();
                }
                 primary_attack = a[i];
              return;
            }
        }
      }
    }







// QUEST
int no(string who){
     if(who != "hero") return 0;
     write(
     COLOR[c]+capitalize(query_name())+" says:\n"+
     "The dark dragons have lived for eons in the corners of this realm.\n"+
     "Unlike the noble dragons, these rogues kill all who go near them only\n"+
    "to collect treasure and magic... Stay away from them at all costs.\n"+
     COLOR[c]+capitalize(query_name())+" ponders a thought.\n"+
    "It is said that their combined horde of treasure would make\n"+
     "a nearly invincible warrior.\n"+
     COLOR[c]+capitalize(query_name())+" chuckles.\n"+
    "It's better not to think about such things though...\n");
     return 1;
}

int bow(string str){
    object qitem;
    int flag;
    if(!qitem = present("q_scroll", TP)) return 0;   
        qitem = present("q_scroll", TP);
    if(!qitem->complete()){
        write(COLOR[c]+"Hero says: You don't have all the signatures!\n");
        command("look at q_scroll",TP);
        return 1;
    }
    
    write(COLOR[c]+"Hero says: Nice job!\n");
    write(COLOR[c]+"Hero says: Here is a gift for your trouble.\n");
    write(COLOR[c]+"Hero gives "+TP->query_name()+" a sword.\n"+NORM);
    call_other("/d/town/obj/hero/q_sword","xx");
    move_object(clone_object("/d/town/obj/hero/q_sword"),TP);
    qitem->rto();
    TP->finish_quest("scroll runner",1);
    return 1;
}

int nod(string str){
   object qitem;
   if(qitem = present("q_scroll", TP)) return 0;   
   if(str == "hero"){ 
   write(COLOR[c]+"Hero says: Ah! Help at last.\n");
   write(COLOR[c]+"Hero gives "+TP->query_name()+" a scroll.\n"); 
   write(COLOR[c]+"Hero says: Look at the scroll and do as instructed.\n");
   write(COLOR[c]+"Hero says: When you're done come back to me.\n"+NORM);
     call_other("/d/town/obj/hero/q_scroll","xx");
   move_object(clone_object("/d/town/obj/hero/q_scroll"),TP);
    TP->set_quest("scroll runner");
    TP->set_quest_hint("read the scroll and follow the instructions.\n");
   return 1;
  }
}


death_2() {
  say(NORM+RED_F+"The hero screams a death cry.\n");
  write(NORM+RED_F+"You have killed a true hero, You bastard!\n");
  return 0;
}

