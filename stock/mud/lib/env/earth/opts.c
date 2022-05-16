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

   set_short(GREEN_F+"gloomy, swampy forest"+NORM);
   set_long(GREEN_F+
    "The ground is soft and muddy in this warm, swampy area. The knobby\n"+
    "trunks of cypress trees rise out of the black mud, their branches\n"+
    "draped with long strands of grey moss. The slight breeze that blows\n"+
    "through the trees barely manages to stir the heavy humid air.\n"+NORM);
 set_items(({
   "up#u#sky",
   "A grey, gloomy sky above the trees.\n",
   "down#d#ground#mud#soil#earth",
   "The ground is soft and muddy.\n",
   "branches#branch",
   "Green, leafy branches, some of them draped with grey moss.\n",
   "water#pools#pool",
   "A shallow layer of water covers much of the ground, with deeper\n"+
   "pools in some areas.\n",
   "moss#strand#strands#carpet",
   "Strands of grey moss hang from tree branches, and a carpet of green\n"+
   "moss grows on the ground beneath some of the trees.\n",
   "lilies#lily#patches#patch#group#blossoms#blossom",
   "Water lilies with white blossoms grow in some of the deeper pools.\n",
   "swamp#landscape",
   "A warm, humid area with muddy ground supporting a variety of plants.\n",
   "algae",
   "A thin layer of algae coats the shallow water.\n",
   "mosquitoes#mosquito#clouds#cloud",
   "Swarms of mosquitoes rise from the swampy water.\n",
   "hillocks#hillock#grass#clumps#clump#top#tops",
   "Clumps of tall swamp grass grow on areas of higher ground.\n",
   "trees#tree#trunks#trunk#cypress#sycamore#grove",
   "Cypress trees grow out of the swampy water, and groves of sycamores\n"+
   "grow on small hills.\n",
   "hill#base#island",
   "Small mossy hills rise out of the dark swampy water.\n",
  }));
 set_smell("The musty smell of wet earth.\n");
 set_listen("The buzzing of mosquitoes.\n");

/////// ROOM ONE
   if(x == 1){
  set_long(GREEN_F+
    "The thick, sucking mud shifts and gurgles underfoot, and the air is\n"+
    "filled with the buzzing of mosquitoes. The long curtains of grey\n"+
    "moss that hang from the trees barely move in the sluggish breeze\n"+
    "that blows through the swamp.\n"+NORM);
              }

/////// ROOM TWO
   if(x == 2){
set_long(GREEN_F+
    "A layer of greenish, algae-coated water covers the thick black mud\n"+
    "here, a perfect breeding ground for the clouds of mosquitoes swirling\n"+
    "into the warm air. A few small hillocks rise above the watery surface,\n"+
    "their tops covered with thick clumps of coarse swamp grass.\n"+NORM);
             }
/////// ROOM THREE
   if(x == 3){
set_long(GREEN_F+
    "Giant cypress trees stretch their delicate branches high into the\n"+
    "gloomy sky. Ghostly strands of grey moss hang down from the trees,\n"+
    "some of them long enough to brush the muddy ground below. The rich,\n"+
    "musty smell of wet earth fills the air.\n"+NORM);
             }
/////// ROOM FOUR
   if(x == 4){
set_long(GREEN_F+
    "Areas of higher ground between the trees are covered with dense\n"+
    "clumps of tall swamp grass that sway gently in the light breeze. The\n"+
    "ground drops sharply away in some spots, leaving deeper pools that\n"+
    "are dotted with water lilies.\n"+NORM);
             }
/////// ROOM FIVE
   if(x == 5){
set_long(GREEN_F+
    "A dense grove of sycamore trees grows on a gently sloping hill that\n"+
    "rises above the dark, swampy water that laps at its base. A carpet\n"+
    "of thick green moss covers the moist ground beneath the trees.\n"+NORM);

             }
/////// ROOM SIX
   if(x == 6){
set_long(GREEN_F+
    "The dense, leafy branches of the sycamore trees in this area allow\n"+
    "only small glimpses of the gloomy sky overhead. The soft moss underfoot\n"+
    "is a welcome relief from the thick, sucking mud in other parts of the\n"+
    "swamp.\n"+NORM);
             }
/////// ROOM SEVEN
   if(x == 7){
set_long(GREEN_F+
    "The thin layer of algae that covers the muddy water here makes it\n"+
    "difficult to judge its depth. Patches of water lilies with delicate\n"+
    "white blossoms float atop the deeper pools. One group of lilies\n"+
    "suddenly bobs in the water, as if disturbed by something moving\n"+
    "beneath the surface.\n"+NORM);
             }
/////// ROOM EIGHT
   if(x == 8){
set_long(GREEN_F+
    "The long, swaying strands of grey moss hanging down from the giant\n"+
    "cypress trees cast ghostly shadows on the muddy ground, and sometimes\n"+
    "become entangled with the tall clumps of swamp grass that grow on\n"+
    "higher ground.\n"+NORM);

             }
/////// ROOM NINE
   if(x == 9){
set_long(GREEN_F+
    "Shallow, muddy water surrounds this small hill on all sides, creating\n"+
    "an inviting island in this swampy landscape. The mossy ground here\n"+
    "is sheltered by the overhanging branches of the sycamore trees which\n"+
    "thrive in the rich, black soil.\n"+NORM);
             }
/////// ROOM TEN
   if(x ==10){ // USE DEFAULT
             }

}
