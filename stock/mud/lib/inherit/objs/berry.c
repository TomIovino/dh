/* goodberry */

#include <ansi.h>
inherit "inherit/food";

reset(arg) {
    ::reset(arg);
    if(arg) return;
    set_name("goodberry");
    set_alias("berry");
    set_short(RED_F+"goodberry"+NORM);
    set_long("This small berry is glowing red. It looks really juicy.\n"+ 
      "You can't wait to eat it.\n");
    set_weight(1);
    set_strength(3 * (random(4)+1));
    set_value(25);
    /* 3 - 12 hp of healing */
}

init(){
    ::init();
    add_action("put", "put");
}

status put(string str){
    object ob;
    string bottle;
    int strength;

    if(!str) return 0;
    if(sscanf(str, "goodberry in %s", bottle) == 1) {
	if((ob = present(bottle, this_player()))) {
	    if(ob->query_alco_drink() || ob->query_soft_drink()){
		write("The berry starts to ferment!\n");
		if(ob->query_full()){
		    write("You fortify the "+(string)ob->query_name()+"!\n");
		    say((string)this_player()->query_name()+" adds a berry to the "+
		      (string)ob->query_name()+"!\n");
		    strength = (int)ob->query_strength();
		}
		destruct(ob);
		ob = clone_object("/inherit/drink");
		ob->set_name("goodberry liquor");
		ob->set_alias("liquor");
		ob->set_alt_name("spell");
		ob->set_short("a bottle of goodberry liquor");
		ob->set_long("\
This is a bottle of goodberry liquor, an obvious attempt at moonshining.\n");
		ob->set_strength(strength+8+random(8));
		ob->set_room_msg("\
You see @@query_name:$this_player()$@@ slam down a bottle of \
goodberry liquor.\n");
		ob->set_consumer_msg("\
You slam down a bottle of goodberry liquor.\n\
The world spins a little as it quenches your thirst.\n");
		move_object(ob,this_player());
		destruct(this_object());
		return 1;
	    }        
	}
    }
    return 0;
}


status cast_spell(object caster,object target,object prev,int dmg) {
    printf("You change the fruit into a goodberry.\n");
    say("The berry "+ caster->query_name() 
      +" is holding begins to glow softly.\n");   
    return 1;  
}  


