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
     x=random(6);
     set_light(0);
/////// ROOM DEFAULT /////////
   set_short(GREY_F"underground caverns"NORM);
   set_long(GREY_F+
"Huge dark Mushrooms grow in a nearby rock formation on the wall.\n"+
"The underground cavern seems to nothing but endless darkness.\n"+
"Darkness calls from the shadows of every wall and tunnel.\n"+
"Fungus grows on the rocks giving the illusion of a grass.\n"+
  NORM);
  
 set_items(({
 "minerals","Speckles of minerals in the other rocks.\n",
 "rocks","The rocks are of different colors and sizes.\n",
 "stalacties","Rocks that 'grow' from the ground towards the ground.\n",
 "stalagmites","Rocks that 'grow' from the floor towards the ceiling.\n",
 "mushrooms","The mushrooms are dark and leatherlike.\n",
 "fungus",GREEN_F+"Green fungus carpets the floor.\n"+NORM,
 "down#ground","The cavern floor.\n",
 "up#sky","The cavern ceiling.\n",
  }));

 set_smell(
   GREY_F+"A moist mildew order lightly scents the air.\n"+NORM);
 set_listen(
   GREY_F+"An echo of a waterdrop can be heard in the distance.\n"+NORM);

/////// ROOM ONE
   if(x == 1){
     set_long(GREY_F+
"Large dark Mushrooms grow in a nearby rock formation on the wall.\n"+
"The underground cavern seems to nothing but endless darkness.\n"+
"Darkness calls from the shadows of every wall and tunnel.\n"+
  NORM);
           }
/////// ROOM TWO
   if(x == 2){
   set_long(GREY_F+
"Huge dark Mushrooms grow in a nearby rock formation on the wall.\n"+
"The underground cavern seems to nothing but endless darkness.\n"+
"Shining mineral specks are bright against the dark rock formations.\n"+
  NORM);
             }
/////// ROOM THREE
   if(x == 3){
   set_long(GREY_F+
"Darkness calls from the shadows of every wall and tunnel.\n"+
"Shining mineral specks are bright against the dark rock formations.\n"+
"Huge stalagmites grow from the floor towards the ceiling.\n"+
  NORM);
             }
/////// ROOM FOUR
   if(x == 4){
   set_long(GREY_F+
"Huge stalacties grow from the ground towards the ground.\n"+
"Darkness calls from the shadows of every wall and tunnel.\n"+
"Shining mineral specks are bright against the dark rock formations.\n"+
  NORM);
             }
/////// ROOM FIVE
   if(x == 5){
   set_long(GREY_F+
"Large dark Mushrooms grow in a nearby rock formation on the wall.\n"+
"Shining mineral specks are bright against the dark rock formations.\n"+
"Fungus grows on the rocks giving the illusion of a grass.\n"+
  NORM);
             }

}
