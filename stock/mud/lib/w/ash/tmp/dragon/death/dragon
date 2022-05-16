#include <ansi.h>
#include <mudlib.h>
inherit "inherit/monster";

reset(arg){
    ::reset(arg);
    set_name("dracolich");
    set_alias("dragon");
    set_aggressive(1);
    set_short(GREY_F+"massive "+WHITE_F+"dracolich"+NORM);
    set_long("\
Large bones make up the massive body of the dracolich. A putrid smell\n\
of death rises from its bones as it marks itself as one of the ruling\n\
creatures of the undead. It looks busy killing you!\n");
    set_level(100);
    set_race("dracolich");
    set_magic_resist(100);
    set_money(5000+random(5000));
    load_spells(40, ({
	"storm",
      }));
    load_a_chat(20, ({
	"\n"
	"The dracolich lifts its skeleton wings high into the sky!\n\
The dracolich tears your skin as it swings its wing through\n\
your chest causing you to scream in pain!\n\
\n",
	"\n"
	"The dracolich launches its huge body in your direction...\n\
It's huge jaws come clamping down on your torso causing\n\
you to growl in pain as magical darkness burns you!\n\
\n",
	"\n"
	"The dracolich grabs you with its massive skeleton\n\
claws ripping through your armour and skin causing\n\
blood to fly everywhere.\n\
\n",
	"\n"
	"The red glowing darkness in the eye sockets of the\n\
dracolich stare into your soul as it rips your\n\
body into a bloody mess!\n\
\n",
      }));
    add_object("skull","/w/monger/graveyard/obj/helm");
    add_object("bones","/w/monger/graveyard/obj/armplates");
}
void attack(){
    object target;
    int damage;
    int soul;
    int i;
    object *a;
    ::attack();
    target=query_primary_attack();
    if(target) {
	if(random(100) <= 15){
	    a = all_inventory(environment());
	    for(i=0;i<sizeof(a);i++){
		if(living(a[i])){
		    if(a[i] != this_object()){
			damage=50+random(50);
			a[i]->adj_hp(-damage);
			tell_room(environment(),"\n");
			tell_room(environment(),BOLD+RED_F+"Flames errupt from the dracolich's body searing everybody in the room!\n\n");
			if(a[i]->query_hp() <= 0){
			    a[i]->death();
			}
			primary_attack = a[i];
			return;
		    }
		}
	    }
	}
	if(random(100) <= 15){
	    soul = 50+random(50);
	    tell_object(target,GREEN_F+BOLD+"The dracolich's eyes glow a bright green as it stares into your soul!\n"+
	      NORM+GREEN_F+"\
You fall down in pain as the dracolich siphons the life force from your soul\n\
drawing it into itself! "+BOLD+"The dracolich looks STRONGER!\n"+NORM);
	    tell_room(environment(target),B_GREEN_F+"The dracolich's eyes glow a bright green at "+target->query_name()+"!\n",({target}));
	    target->adj_mana_points(-soul);
	    this_object()->adj_hp(soul/3);
	} }
}
