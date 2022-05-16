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

   set_short(GREEN_F+"hot, dry forest"+NORM);
   set_long(GREEN_F+
    "A hot, dry wind blows through the widely spaced trees in this\n"+
    "desert-like area, causing dust devils to dance along the sandy\n"+
    "soil. The flat land is broken by the occasional small sand dune\n"+
    "formed by the swirling wind.\n"+NORM);
 set_items(({
    "up#u#sky",
    "A cloudless sky overhead.\n",
    "down#d#ground#devils#devil#sand#sands#cloud#clouds",
    "The dry soil is loose and sandy, with occasional dust devils\n"+
    "dancing across the surface.\n",
    "dune#dunes",
    "The swirling winds cause small hills of sand to form and shift\n"+
    "between the trees.\n",
    "tree#trees#oak#mesquite#joshua#cottonwood#trunks#trunk#forest",
    "A mixture of cottonwood, oak, mesquite and joshua trees grow in\n"+
    "this region.\n",
    "vegetation#cacti#clumps#patches#grass#plant#plants",
    "Among the widely spaced trees can be found small cacti and patches\n"+
    "of grass.\n",
  }));
 set_smell("Windblown sand tickles your nose.\n");
 set_listen("The faint hissing sound of the shifting sands.\n");

/////// ROOM ONE
   if(x == 1){
  set_long(GREEN_F+
    "The vegetation in this sparsely forested region is a mixture of\n"+
    "plants native to both desert and temperate areas. Oak and cottonwood\n"+
    "trees, somewhat stunted by the dry conditions, mingle with mesquite\n"+
    "and joshua trees.\n"+NORM);
              }

/////// ROOM TWO
   if(x == 2){
set_long(GREEN_F+
    "The ground here is loose and sandy, dotted with small clumps of\n"+
    "grass that spring up after the infrequent rainfalls, but quickly\n"+
    "turn brown in the dry heat here. A few green clumps can be seen in\n"+
    "the shade of the larger trees.\n"+NORM);
             }
/////// ROOM THREE
   if(x == 3){
set_long(GREEN_F+
    "The heat and the constantly shifting sands underfoot make travel\n"+
    "here tiring. The areas of shade cast by the scattered trees offer\n"+
    "some protection from the burning sun during the daytime hours.\n"+NORM);
             }
/////// ROOM FOUR
   if(x == 4){
set_long(GREEN_F+
    "The swirling winds shift and reshape the small sand dunes here,\n"+
    "constantly changing the landscape. The trunks of the sturdy desert\n"+
    "trees will be half-buried in sand one moment, only to be uncovered\n"+
    "again with the next shifting wind.\n"+NORM);
             }
/////// ROOM FIVE
   if(x == 5){
set_long(GREEN_F+
    "Small cacti and clumps of brown grass dot the sandy ground between\n"+
    "the widely spaced trees. The dry conditions don't allow for the\n"+
    "growth of a full forest, but a few deeply rooted trees have adapted\n"+
    "to the reduced moisture in this region.\n"+NORM);

             }
/////// ROOM SIX
   if(x == 6){
set_long(GREEN_F+
    "A line of large, healthy oak trees skirts the edge of a small sand\n"+
    "dune, indicating the presence of a water source beneath the sandy\n"+
    "surface of the ground. The green grass under the trees offers a\n"+
    "welcome relief from the sparse desert vegetation.\n"+NORM);
             }
/////// ROOM SEVEN
   if(x == 7){
set_long(GREEN_F+
    "The mixture of different types of plant life in this area seems to\n"+
    "indicate a change from one climatic zone to another. Cottonwood and\n"+
    "oak trees that thrive in cooler conditions are somewhat stunted here,\n"+
    "but still manage to grow among desert mesquite and joshua trees.\n"+NORM);
             }
/////// ROOM EIGHT
   if(x == 8){
set_long(GREEN_F+
    "A soft, whispering sound fills the air as the hot wind shifts and\n"+
    "re-forms small sand dunes among the sturdy trees growing here. Sparse\n"+
    "patches of brown, dried grass rise from the loose, sandy soil.\n"+NORM);

             }
/////// ROOM NINE
   if(x == 9){
set_long(GREEN_F+
    "A hot, swirling wind sends stinging clouds of sand through the air,\n"+
    "briefly obscuring the landscape. The fine sand clings to the trunks\n"+
    "of the scattered trees, and piles around the small clumps of grass\n"+
    "that have tried to grow here.\n"+NORM);
             }
/////// ROOM TEN
   if(x ==10){ // USE DEFAULT
             }

}
