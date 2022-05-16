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

   set_short(GREEN_F+"snowy evergreen forest"+NORM);
   set_long(GREEN_F+
    "Sturdy evergreen trees of various kinds decorate this cold landscape\n"+
    "of low, snow-covered hills. Patches of new grass show through the\n"+
    "snow in open areas where sunlight has been able to warm the frosty\n"+
    "ground.\n"+NORM);
 set_items(({
   "up#u#sky",
   "A clear, bright sky above the trees.\n",
   "ground#soil#mound#mounds#drifts#drift#banks#bank#snow",
   "Areas of melting snow alternate with patches of new grass. Deeper\n"+
   "drifts can be seen beneath the trees.\n",
   "splashes#splash#wildflowers#wildflower",
   "Some brightly colored wildflowers grow along a grassy hillside.\n",
   "trees#tree#grove#spruce#pine#firs#fir",
   "Spruce, pine and fir trees grow in this cold region.\n",
   "branches#branch#needles#needle#clumps#clump#cones#cone",
   "The evergreen branches are thickly covered with needles and lightly\n"+
   "dusted with snow. Clumps of dried needles and pine cones are scattered\n"+
   "beneath the trees.\n",
   "landscape#hills#hill#base#top#hillside#slopes#slope",
   "Low hills and evergreen trees fill this region.\n",
   "furrows#furrow#rivulets#rivulet#run-off",
   "Water from melting snow runs down the hillsides and carves shallow\n"+
   "furrows in the soil.\n",
   "grass#patches#patch#layer#blades#blade",
   "New grass is springing up in areas where the snow has melted.\n",
   "flurry#crystals#crystal",
   "The wind blows snow off the tree branches and sends it swirling through\n"+
   "the air.\n",
   "gully#ice#chunks#chunk",
   "A low area at the base of a hill is filled with slushy snow and chunks\n"+
   "of ice.\n",
  }));
 set_smell("The tangy scent of pine fills the air.\n");
 set_listen("The cold wind whistles through the trees.\n");

/////// ROOM ONE
   if(x == 1){
  set_long(GREEN_F+
    "Snow covers the frozen ground within this grove of tall, slender\n"+
    "spruce trees. The drooping, blue-green branches of the trees stir in\n"+
    "the cold wind, causing a scattering of needles to fall on the ground\n"+
    "below.\n"+NORM);
              }

/////// ROOM TWO
   if(x == 2){
set_long(GREEN_F+
    "A cold wind swirls through this open area between the trees, rippling\n"+
    "the small blades of new grass that are poking up through the thawing\n"+
    "ground. Shallow furrows are carved in the soil by tiny rivulets that\n"+
    "flow from mounds of melting snow.\n"+NORM);
             }
/////// ROOM THREE
   if(x == 3){
set_long(GREEN_F+
    "Giant pine trees spread their long-needled branches wide against the\n"+
    "clear, bright sky. Dried clumps of needles and pine cones dot the\n"+
    "snowy ground under the trees, and the tangy scent of pine floats on\n"+
    "the cold air.\n"+NORM);
             }
/////// ROOM FOUR
   if(x == 4){
set_long(GREEN_F+
    "A gust of wind blows through some short-needled firs, swirling snow\n"+
    "off their branches and causing a brief flurry of icy crystals through\n"+
    "the air. The snow among the trees has piled into drifts, which make\n"+
    "travel slow and tiring.\n"+NORM);
             }
/////// ROOM FIVE
   if(x == 5){
set_long(GREEN_F+
    "The refreshing scent of pine from the surrounding trees fills the\n"+
    "cold, crisp air. The thawing ground in this open area is covered with\n"+
    "soft new grass, its light green color contrasting with the remaining\n"+
    "mounds of snow near the trees.\n"+NORM);

             }
/////// ROOM SIX
   if(x == 6){
set_long(GREEN_F+
    "Dark green fir trees with delicately tapered branches are widely\n"+
    "scattered along the slopes of a low hill. Areas of melting snow are\n"+
    "interspersed with clear patches where new grass is sprouting. Deeper\n"+
    "banks of snow remain in the shaded areas beneath the trees.\n"+NORM);
             }
/////// ROOM SEVEN
   if(x == 7){
set_long(GREEN_F+
    "Tall spruce trees rise high into the sky, shading this shallow gully\n"+
    "that runs along the base of a lightly wooded hill. The run-off from\n"+
    "the melting snow on the hillside, and the alternating conditions of\n"+
    "freezing and thawing, have made the ground here a mixture of slushy\n"+
    "snow and jagged chunks of ice.\n"+NORM);
             }
/////// ROOM EIGHT
   if(x == 8){
set_long(GREEN_F+
    "Only a few pine trees crown the top of a small hill here, leaving an\n"+
    "open hillside that has completely shed its covering of snow and is\n"+
    "carpeted with a thick layer of soft green grass. Bright splashes of\n"+
    "pink, yellow and purple mark the presence of wildflowers that spring\n"+
    "up suddenly in the brief periods of warmer weather.\n"+NORM);

             }
/////// ROOM NINE
   if(x == 9){
set_long(GREEN_F+
    "The fir trees in this small grove sparkle with the light dusting of\n"+
    "powdery snow that clings to their branches. The trees grow closely\n"+
    "together here, and their thick, densely needled branches help to block\n"+
    "the chilly winds that blow through this region.\n"+NORM);
             }
/////// ROOM TEN
   if(x ==10){ // USE DEFAULT
             }

}
