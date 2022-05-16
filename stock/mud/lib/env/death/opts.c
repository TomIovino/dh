// Ash

#include <ansi.h>
#include <mudlib.h>
#include "opts.h"
#include <randomnpc.h>
// dummy room functions 

int set_light(int x);
string set_long(string s);
string set_short(string s);
string set_items(string *a);
string set_smell(string s);
string set_listen(string s);
void set_up_room();


void set_up_room(){
     int x;
     x= random(11);
     set_light(1);
     randomnpc();



/////// ROOM DEFAULT /////////

   set_short(GREEN_F+"dying, decayed forest"+NORM);
   set_long(GREEN_F+
    "Decaying vegetation covers the ground between the trees of this\n"+
    "old, dying forest. The branches overhead are dry and brittle,\n"+
    "sparsely covered with withered leaves. Some thorny vines winding up\n"+
    "the trees seem to be the only things thriving here.\n"+NORM);
 set_items(({
    "up#u#sky",
    "A gloomy, overcast sky above the trees.\n",
    "down#d#vegetation#ground",
    "Rotting vegetation and broken tree branches cover the ground.\n",
    "forest#tree#trees#trunk#trunks#sapling#saplings#bark",
    "The trees in this old growth forest are being killed by the\n"+
    "parasitic vines climbing up their trunks.\n",
    "branch#branches#leaf#leaves#limb#limbs",
    "Dead, dry branches with a few withered leaves.\n",
    "vine#vines",
    "Thorny vines are taking over the forest and sucking the life out\n"+
    "of the trees.\n",
  }));
 set_smell("The sickly, rotten odor of death and decay.\n");
 set_listen(
  "The mournful sound of the wind blowing through the dying trees.\n");

/////// ROOM ONE
   if(x == 1){
  set_long(GREEN_F+
    "The ground is spongy with decaying leaves and chunks of broken bark\n"+
    "that have rotted off of the dying trees. A few wilted leaves fall to\n"+
    "the ground as the barren branches crackle overhead. Some of the trees\n"+
    "lean drunkenly, their supporting roots rotting away.\n"+NORM);
              }

/////// ROOM TWO
   if(x == 2){
set_long(GREEN_F+
    "The tilting trunks of diseased trees make travel difficult through\n"+
    "this dying forest. Branches snap dangerously overhead in a light\n"+
    "wind, occasionally breaking off and crashing to the ground. The air\n"+
    "is thick and heavy with the smell of rotting vegetation.\n"+NORM);
             }
/////// ROOM THREE
   if(x == 3){
set_long(GREEN_F+
    "The ghostly limbs of dying trees reach toward the overcast sky, a few\n"+
    "withered leaves hanging limply from the dry branches. The cracked\n"+
    "trunks are wrapped with thorny vines, their roots deeply imbedded in\n"+
    "the bark, sucking the life from this old forest.\n"+NORM);
             }
/////// ROOM FOUR
   if(x == 4){
set_long(GREEN_F+
    "The twisted limbs of dying trees make ghostly patterns against the\n"+
    "gloomy sky. The rotting vegetation underfoot makes the ground soft\n"+
    "and spongy, and the occasional broken branch threatens to trip the\n"+
    "unwary traveler.\n"+NORM);
             }
/////// ROOM FIVE
   if(x == 5){
set_long(GREEN_F+
    "A faint warmth rises from the decomposing vegetation that covers the\n"+
    "ground, carrying the sickly sweet scent of death and decay. The only\n"+
    "signs of healthy life are the thorny vines that have strangled the\n"+
    "life out of the old trees.\n"+NORM);

             }
/////// ROOM SIX
   if(x == 6){
set_long(GREEN_F+
    "The surrounding forest is dark and gloomy, with dying trees tilting\n"+
    "at dangerous angles as their roots rot in the spongy ground. Only a\n"+
    "few withered, yellowish leaves cling to the twisted branches, soon\n"+
    "to be overrun by the thorny vines twining up the trunks.\n"+NORM);
             }
/////// ROOM SEVEN
   if(x == 7){
set_long(GREEN_F+
    "An atmosphere of death pervades this forest of old trees. A few\n"+
    "young saplings have struggled to grow from the the soft, rotten\n"+
    "ground, only to be strangled by the same thorny vines that have\n"+
    "taken over the rest of the forest.\n"+NORM);
             }
/////// ROOM EIGHT
   if(x == 8){
set_long(GREEN_F+
    "The dying trees seem to shiver sadly in the mournful wind that blows\n"+
    "through this gloomy forest. The thick, thorny vines that crawl up\n"+
    "the trunks seem to pulse with the life that is being sucked out of\n"+
    "the trees.\n"+NORM);

             }
/////// ROOM NINE
   if(x == 9){
set_long(GREEN_F+
    "Thorny vines wind tightly around the trunks of the dying trees in\n"+
    "this old growth forest. The spongy ground is covered by a thick\n"+
    "layer of rotting vegetation and broken tree branches, which make\n"+
    "walking hazardous.\n"+NORM);
             }
/////// ROOM TEN
   if(x ==10){ // USE DEFAULT
             }

}
