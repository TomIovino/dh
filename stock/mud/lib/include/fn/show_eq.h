#include <ansi.h>
static string L,R,Y;
string show_eq(string x){
  object me;
  object left,right,held;
  object head,neck,floating,torso,back,face;
  object hands,fingers,waist,legs,feet,arms;
  object wrists,ankles;
  string temp;
   R = BOLD+WHITE_F+">"+NORM;
   Y = NORM+GREY_F;
   L = BOLD+WHITE_F+"<"+NORM+WHITE_F;
  me        = this_object();
  face      = me->query_armour_type("mask");
  floating  = me->query_armour_type("floating");
  head      = me->query_armour_type("helm");
  neck      = me->query_armour_type("amulet");
  torso     = me->query_armour_type("armour");
  back      = me->query_armour_type("cloak");
  right     = me->query_right_weapon();
  left      = me->query_left_weapon();
  held      = me->query_armour_type("shield");
  arms      = me->query_armour_type("sleeves");
  wrists    = me->query_armour_type("wrists");
  hands     = me->query_armour_type("gloves");
  fingers   = me->query_armour_type("ring");
  waist     = me->query_armour_type("belt");
  ankles    = me->query_armour_type("ankles");
  legs      = me->query_armour_type("pants");
  feet      = me->query_armour_type("boots");
temp = "";

if(floating)
  temp+=(L+"floating nearby"+R+"     "+floating->query_short()+"\n");  
if(!floating && x)
  temp+=(Y+"<floating nearby>\n");
if(head)
  temp+=(L+"worn on head"+R+"        "+head->query_short()+"\n");
if(!head && x)
  temp+=(Y+"<worn on head>\n");
if(face)
  temp+=(L+"worn on face"+R+"        "+face->query_short()+"\n");
if(!face && x)
  temp+=(Y+"<worn on face>\n");
if(neck)
  temp+=(L+"worn around neck"+R+"    "+neck->query_short()+"\n");
if(!neck && x)
  temp+=(Y+"<worn around neck>\n");
if(torso)
  temp+=(L+"worn on torso"+R+"       "+torso->query_short()+"\n"); 
if(!torso && x)
  temp+=(Y+"<worn on torso>\n");
if(arms)
  temp+=(L+"worn on arms"+R+"        "+arms->query_short()+"\n");
if(!arms && x)
  temp+=(Y+"<worn on arms>\n");
if(back)
  temp+=(L+"worn on back"+R+"        "+back->query_short()+"\n");
if(!back && x)
  temp+=(Y+"<worn on back>\n");
if(wrists)
  temp+=(L+"worn on wrists"+R+"      "+wrists->query_short()+"\n");
if(!wrists && x)
  temp+=(Y+"<worn on wrists>\n");
if(right)
  temp+=(L+"held in right hand"+R+"  "+right->query_short()+"\n");
if(!right && x)
  temp+=(Y+"<held in right hand>\n");
if(left)
  temp+=(L+"held in left hand"+R+"   "+left->query_short()+"\n");
if(held)
  temp+=(L+"held in left hand"+R+"   "+held->query_short()+"\n");
if(!left && !held && x)
  temp+=(Y+"<held in left hand>\n");
if(hands)
  temp+=(L+"worn on hands"+R+"       "+hands->query_short()+"\n");
if(!hands && x)
  temp+=(Y+"<worn on hands>\n");
if(fingers)
  temp+=(L+"worn on fingers"+R+"     "+fingers->query_short()+"\n");
if(!fingers && x)
  temp+=(Y+"<worn on fingers>\n");
if(waist)
  temp+=(L+"worn about waist"+R+"    "+waist->query_short()+"\n");
if(!waist && x)
  temp+=(Y+"<worn about waist>\n");
if(legs)
  temp+=(L+"worn on legs"+R+"        "+legs->query_short()+"\n");
if(!legs && x)
  temp+=(Y+"<worn on legs>\n");
if(ankles)
  temp+=(L+"worn on ankles"+R+"      "+ankles->query_short()+"\n");
if(!ankles && x)
  temp+=(Y+"<worn on ankles>\n");
if(feet)
  temp+=(L+"worn on feet"+R+"        "+feet->query_short()+"\n");
if(!feet && x)
  temp+=(Y+"<worn on feet>\n");
return temp;
}
