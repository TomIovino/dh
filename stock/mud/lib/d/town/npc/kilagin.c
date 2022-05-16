#include <mudlib.h>
#include <ansi.h>

inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if (arg) return;
    set_name("kilagin");
    set_alt_name("hunter");
    set_alias("bounty");
    set_short(BLUE_F+"retired bounty hunter"+NORM);
    set_long(
  "Kilagin was once a great bounty hunter, but following the injury that left\n"+
  "him blinded in the right eye he settled down to establish the Twilite City\n"+
  "Bounty Agency. He still looks like a formidable opponent despite his age\n"+
  "and lack of depth perception.\n");
    set_gender(1);
    add_object("NOBLE_OB","d/town/obj/NOBLE_OB");
    set_level(50);
    set_half_exp(1);
    set_magic_resist(50);

	
}
void catch_tell(string str) {
    string a,b,mesg;
    if(sscanf(str,"%sjob%s",a,b)==2 || sscanf(str,"%squest%s",a,b)==2) {
	mesg="\nKilagin looks you over carefully for a moment.\n\n"+
	"Kilagin says: Yes, you look like you'll do.\n\n"+
	"Kilagin says: I got a special job you might be interested in...\n\n"+
	"Kilagin says: Some... people, want to see the Steward of the Konkarian Silver\n"+
	"Mines... taken care of, and will pay 10 gold for it.\n\n"+
	"Kilagin leans back in his chair and grins at you.\n\n"+
	"Kilagin says: Interested?\n"+NORM;
	write(CYAN_F+mesg+NORM);
	this_player()->set_quest("bounty hunter");
    this_player()->set_quest_hint("Assassinate Kassandra, Steward of the Konkarian Silver Mines,\n"+
	  "and bring her head to Kilagin for payment.\n");
	return; }
    if(sscanf(str,"%syes%s",a,b)==2 || sscanf(str,"%syeah%s",a,b)==2 || sscanf(str,"%ssure%s",a,b)==2) {
	if(this_player()->query_current_quest()=="bounty hunter") {
	    mesg="\nKilagin grins.\n"+
	    "\nKilagin says: Then get going, you know where to find me when you're done.\n";
	    write(CYAN_F+mesg+NORM);
	    return; } }
    return;
}
