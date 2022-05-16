#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
#include "hdefs.h"     
inherit ROOM;

void reset(status arg) {
    add_object("knight1",PATH+HOUSE+"/knight");
    add_object("knight2",PATH+HOUSE+"/knight");
    add_object("knight3",PATH+HOUSE+"/knight");
    add_object("cook",PATH+HOUSE+"/cook");
    if(arg) return;
    set_short(CASTLE+" great hall");
    set_long(GREY_F+
    "The great hall is a large communal room at the center of the castle.\n"+
    "A fire burns brightly in the immense fireplace, which is fitted with\n"+
    "roasting spits and cooking racks. Scrubbed wooden tables flanked by\n"+
    "long benches provide a place for the castle residents to rest and eat.\n"+
    "Straw filled pallets along the walls serve as beds for house members.\n"+
    "The large silk banners that hang from the ceiling identify the castle\n"+
    "as belonging to House "CASTLE".\n"+
    NORM);
    set_items(({
    "up#u#ceiling",
    "The high stone ceiling is supported by thick wooden crossbeams.\n",
    "down#d#floor",
    "The stone floor is cushioned by a layer of freshly cut rushes.\n",
    "walls#wall#torches#torch",
    "Torches are mounted on the thick stone walls to provide light.\n",
    "castle#keep",
    "This sturdy stone fortress belongs to House "CASTLE".\n",
    "fireplace#spits#spit#racks#rack",
    "The fireplace contains several types of implements for food\n"+
    "preparation.\n",
    "fire",
    "The fire provides both warmth and a place to cook.\n",
    "tables#table#benches#bench",
    "Long benches lie alongside the wooden dining tables.\n",
    "pallets#pallet#beds#bed",
    "Linen covers filled with straw are used as sleeping pallets.\n",
    "banners#banner",
    "Large silk banners bearing the emblem of House "CASTLE" hang from\n"+
    "the ceiling crossbeams.\n",
    "emblem",
    "YOUR DESCRIPTION GOES HERE.\n",
    }));
    set_pk_only(1);    
    set_smell("The odor of cooking food drifts from the fireplace.\n");
    set_listen("The large fire crackles and hisses.\n");
    set_exits(({
    PATH+HOUSE+"/b","north",
    PATH+HOUSE+"/d","west",
    PATH+HOUSE+"/f","east",
    PATH+HOUSE+"/g","south",
      }));
   set_light(5);

}
