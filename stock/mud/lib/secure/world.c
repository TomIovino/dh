// Ash

#include <ansi.h>
#include <mudlib.h>

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
     x=random(10);
     set_light(1);

/////// ROOM DEFAULT /////////

   set_short(GREEN_F"dense evergreen forest"NORM);
   set_long(GREEN_F+
   "The sturdy trunks of mighty evergreens rise high above the pine"+
  " needle\n"+
  "carpeted forest floor. Growing densely between the trees are thorny\n"+
  "bushes that make travel rather difficult, although the brightly"+
  " coloured\n"+
  "flowers growing amongst their branches are quite pleasing to the"+
  " eye.\n"+NORM);
 set_items(({
    "trunk#trunks#tree#trees#evergreen#evergreens#oak#oaks#pine#pines",
    "Sturdy trunks of mighty trees rise high above the forest floor.\n",
    "bush#bushes#thorn#thorns",
    "Thorny bushes grow densely together around the thick tree trunks.\n",
    "flower#flowers","Many colourful flowers bloom brightly amongst.\n"+
    " the otherwise thorny bushes.\n",
    "d#down#ground#trail#pine needles#needle#needles",
    "The ground is dark, hard packed earth covered with soft pine needles.\n"+
    "A thin trail seems to wind it's way through the forest, though it is\n"+
    "hard to tell if it is a real trail or one of the many false ones that\n"+
    "naturally form in forests.\n",
    "moss","Soft green moss grows on the tree trunks.\n",
    "u#up#sky#clouds#cloud",
    "High above the tree tops wispy clouds drift on the breeze of an\n"+
    "otherwise beautiful blue sky.\n"
  }));
 set_smell(
   CYAN_F"The scent of pine needles hangs in the cool moist air.\n"+NORM);
 set_listen(
   CYAN_F"All you hear is the gentle breeze blowing through the trees.\n"NORM);

/////// ROOM ONE
   if(x == 1){
  set_long(GREEN_F+
  "The sturdy trunks of mighty evergreens rise high above the pine"+
  "needle\n"+
  "carpeted forest floor. Growing densely between the trees are thorny\n"+
  "bushes that make travel rather difficult, although the brightly"+
  " coloured\n"+
  "flowers growing amongst their branches are quite pleasing to the"
  " eye.\n"+NORM); }

/////// ROOM TWO
   if(x == 2){
set_short("dense evergreen forest");
set_long(BOLD+GREEN_F+
 "The forest is still and the quiet is nearly surreal. The great"
 " evergreens\n"+
 "grow so thick that their heavy branches droop from their own weight, a few\n"+
 "of them touch the ground forming tent like areas under the trees. Moss "+
 "grows\n"+
 "thick on their trunks due to the dampness of the air under the dense"+
 " canopy\n"+
 "of branches.\n"+NORM);
             }
/////// ROOM THREE
   if(x == 3){
set_short(GREY_F+"misty forest");
set_long(GREY_F+
  "Tall brown capped mushrooms grow densely together in the damp air under\n"+
  "the thick canopy of branches above. The sturdy trunks of mighty oaks rise\n"+
  "high above the forest floor which is blanketed in a thin mist that floats\n"+
  "gently on a barely noticeable breeze.\n"+
  NORM);
set_items(({
    "mist","A thin mist floats just above the ground.\n",
    "mushroom#mushrooms",
"Large brown mushrooms grow thickly in the damp air under the canopy of trees.\n",
    "canopy#trunk#trunks#tree#trees#oak#oaks#pine#pines",
    "Sturdy trunks of mighty trees rise high above the forest floor.\n",
    "d#down#ground#trail#pine needles#needle#needles",
    "The ground is dark, hard packed earth covered with dead brown leaves.\n"+
    "A thin trail seems to wind it's way through the forest, though it is\n"+
    "hard to tell if it is a real trail or one of the many false ones that\n"+
    "naturally form in forests.\n",
    "u#up#sky#clouds#cloud",
  "High above the tree tops wispy clouds drift on the breeze of an otherwise\n"+
    "beautiful blue sky.\n"
  }));
             }
/////// ROOM FOUR
   if(x == 4){
set_long(GREEN_F+
  "Shadows cast by the massive branches flicker as the trees sway slightly from\n"+
  "an otherwise unnoticable breeze. The forest seems empty, although movement\n"+
  "flickers on the edge of vision in the deep shadows. The forest floor rustles\n"+
  "as small creatures hurry about their business to the security of their homes.\n"+
  NORM);
set_items(({
    "creature#creatures","They dart just out of your vision and into the dark cover of the shadows.\n",
    "shadows","Deep shadows are cast beneath the mighty branches of the forest.\n",
    "trunk#trunks#tree#trees#branches#branch#oak#oaks#pine#pines",
    "Heavy branches lush with broad green leaves form the boughs of the\n"+
    "mighty trees of this forest.\n",
    "d#down#ground#trail#pine needles#needle#needles",
    "The ground is dark, hard packed earth covered with soft pine needles.\n"+
    "A thin trail seems to wind it's way through the forest, though it is\n"+
    "hard to tell if it is a real trail or one of the many false ones that\n"+
    "naturally form in forests.\n",
    "u#up#sky#clouds#cloud",
    "High above the tree tops wispy clouds drift on the breeze of an otherwise\n"+
    "beautiful blue sky.\n"
  }));
             }
/////// ROOM FIVE
   if(x == 5){
set_long(GREEN_F+
  "Shadows cast by the massive branches flicker as the trees sway slightly from\n"+
  "an otherwise unnoticable breeze. The forest seems empty, although movement\n"+
  "flickers on the edge of vision in the deep shadows. The forest floor rustles\n"+
  "as small creatures hurry about their business to the security of their homes.\n"+
  NORM);
set_items(({
    "creature#creatures","They dart just out of your vision and into the dark cover of the shadows.\n",
    "shadows","Deep shadows are cast beneath the mighty branches of the forest.\n",
    "trunk#trunks#tree#trees#branches#branch#oak#oaks#pine#pines",
    "Heavy branches lush with broad green leaves form the boughs of the\n"+
    "mighty trees of this forest.\n",
    "d#down#ground#trail#pine needles#needle#needles",
    "The ground is dark, hard packed earth covered with soft pine needles.\n"+
    "A thin trail seems to wind it's way through the forest, though it is\n"+
    "hard to tell if it is a real trail or one of the many false ones that\n"+
    "naturally form in forests.\n",
    "u#up#sky#clouds#cloud",
    "High above the tree tops wispy clouds drift on the breeze of an otherwise\n"+
    "beautiful blue sky.\n"
   }));

             }
/////// ROOM SIX
   if(x == 6){
set_long(GREEN_F+
 "A few ancient oak trees rise up in the midst of the evergreens, contrasting\n"+
 "their leafy green foliage with the dark sharpness of the pines. The air is\n"+
 "lighter here as the oak trees allow in more light to the forest floor, and\n"+
 "a faint breeze blows through the trees making them quiver slightly.\n"NORM);

             }
/////// ROOM SEVEN
   if(x == 7){
set_long(GREEN_F+
  "Large oak trees rise high above the evergreens and crowd out the smaller\n"+
  "pine trees from the much needed light and space. The small evergreens stand\n"+
  "in isolated patches amongst the towering leafy giants. Thick thorny bushes\n"+
  "dotted with colourful flowers grow densely about the bases of the trees.\n"+
  NORM);
             }
/////// ROOM EIGHT
   if(x == 8){
set_long(GREEN_F+
  "Broad oak trees stand in majestic solitude rising high above the forest\n"+
  "floor and casting shimmering shadows on the ground below as sunlight\n"+
  "drifts through their leafy branches. An occasional squirrel scurries\n"+
  "down the trees and dashes across the intervening ground before reaching\n"+
  "the safety of another tree. Dense patches of nettles and thorny brush\n"+
  "cling to the bases of the thick trunks.\n"+
  NORM);
set_items(({
    "trunk#trunks#tree#trees#oak#oaks",
    "Sturdy trunks of mighty trees rise high above the forest floor.\n",
    "bush#bushes#thorn#thorns#nettle#nettles#patch#patches#brush",
    "Thorny bushes grow densely together around the thick tree trunks.\n",
    "squirrel","The occasional squirrel you spot seems rather timid, and doesn't wait.\n",
    "to see what you want before running for a safe place to hide.\n",
    "d#down#ground#trail#pine leaves#leaf#leaves",
    "The ground is dark, hard packed earth covered with dark dead leaves.\n"+
    "A thin trail seems to wind it's way through the forest, though it is\n"+
    "hard to tell if it is a real trail or one of the many false ones that\n"+
    "naturally form in forests.\n",
    "u#up#sky#clouds#cloud",
    "High above the tree tops wispy clouds drift on the breeze of an otherwise\n"+
    "beautiful blue sky.\n"
  }));

             }
/////// ROOM NINE
   if(x == 9){
set_long(GREEN_F+
  "The forest is immersed in an enveloping cloak of silence. An occasional\n"+
  "bird call echoes through the trees harshly before dying out. The twisted\n"+
  "roots of the many trees entwine about each other, creating a web of wood\n"+
  "on the forest floor.\n"+
  NORM);
set_items(({
    "bird","It is somewhere above the branches out of your sight.\n",
    "root#roots#web",
"An intricate web of entwined roots extends between the closely packed trees.\n",
    "trunk#trunks#tree#trees#oak#oaks",
    "Sturdy trunks of mighty trees rise high above the forest floor.\n",
    "d#down#ground#trail#pine leaves#leaf#leaves",
    "The ground is dark, hard packed earth covered with dark dead leaves.\n"+
    "A thin trail seems to wind it's way through the forest, though it is\n"+
    "hard to tell if it is a real trail or one of the many false ones that\n"+
    "naturally form in forests.\n",
    "u#up#sky#clouds#cloud",
 "High above the tree tops wispy clouds drift on the breeze of an otherwise\n"+
    "beautiful blue sky.\n"
  }));
set_listen(
  "The forest is so incredibly silent that even your breathing seems to\n"+
  "be disturbingly loud.\n");

             }
/////// ROOM TEN
   if(x ==10){ // USE DEFAULT
             }

}
