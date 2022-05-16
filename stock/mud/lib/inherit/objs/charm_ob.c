/* charm object */

#include <humanoid.h>
#include <ansi.h>

#define TP this_player() 
#define NAME           (string)this_player()->query_name()
#define CHARMED_WISDOM (int)charmed->query_stat("wisdom")+1
#define CHARISMA       (int)this_player()->query_stat("charisma")+1
#define IMMUNE         query_spell_immune("charm")

object charmed;
object follow;
string name;
int no_get, no_kill, no_drop, no_follow;

void reset(status arg) {
    if(arg) return;
    no_kill   = -1;
    no_get    = -1;
    no_drop   = -1;
    no_follow = -1;
}

void set_no_kill(status arg)   { no_kill = arg; }
void set_no_get(status arg)    { no_get = arg; }
void set_no_drop(status arg)   { no_drop = arg; }
void set_no_follow(status arg) { no_follow = arg; }

int query_no_kill()   { return no_kill; }
int query_no_get()    { return no_get;  }
int query_no_drop()   { return no_drop; }
int query_no_follow() { return no_follow; }


status id(string str) {
    return str == "spell"
    || str == "charm";
}

status drop(status quit) {
    return 1;
}

string saved_short;
string saved_long;
object saved_dead_ob;
object saved_steal_ob;
object saved_heart_ob;
string short;

void set_charmed(object ob) {
    string tmp; 
    if(ob->IMMUNE) {
	write(ob->query_name() + " shakes off the spell.\n");
	ob = 0;
	return;
    }
    if(!(charmed = ob)) return;
    if(!(name = (string)ob->query_name())) {
	ob = 0;
	return;
    }
    name = capitalize(name);
    if(ob->query_charmed_ob()) {
	write(name + " is already charmed.\n");
	charmed = name = 0;
	call_out("time_out",2);
	return;
    }

    saved_dead_ob = (object) ob->query_dead_ob();
    saved_steal_ob = (object) ob->query_steal_ob();
    saved_heart_ob = (object) ob->query_heart_ob();
    saved_short = (string) ob->query_short();
    saved_long = (string) ob->query_long();
    tmp = (string) this_player()->query_name();

    ob->set_dead_ob(this_object());
    ob->set_steal_ob(this_object());
    ob->set_heart_ob(this_object());
    ob->set_charmed_ob(this_object());
    ob->set_short(tmp+"'s "+ob->query_short());
    if(saved_long && saved_long[<1] != '\n')
	saved_long += "\n";
    ob->set_long(sprintf("%s\t%sThis creature has been befriended by %s.%s\n",
	(saved_long ? saved_long : ""), B_GREEN_F, tmp, NORM));
    call_out("dispel",3600);
}

void init() {
    if(!name || !charmed) return;
    add_action("charm_commands", "order");
}


status charm_commands(string str) {
    string arg;
    object ob;
    short = (string) charmed->query_short();
    name = (string) charmed->query_name();
    if(!charmed) {
	destruct(this_object());
	return 1;
    }
    if(!present(charmed, environment(this_player()))) {
	notify_fail(name+" is not here.\n");
	return 0;
    }
    if(!str) {
	notify_fail(name +" what?\n");
	return 0;
    }
    sscanf(str, "%s %s", str, arg);
    switch(str) {
    case "kill":
	if(no_kill) {
	    if(no_kill == 1 || CHARMED_WISDOM > (CHARISMA)) {
		write(name +" refuses to do that!\n");
		no_kill = 1;
	    }
	    else {
		no_kill = 0;
	    }
	}
	if(!no_kill) {
	    if(!arg) {
		write(short +" looks aggressive.\n");
		say(short +" gets into an aggressive stance.\n");
	    }
	    else if(!(ob = present(arg, environment(this_player())))) {
		write(short +" looks around for "+arg+".\n");
		say(short +" starts looking for "+ arg +".\n");
	    }
	    else if(!living(ob)) {
		tell_room(environment(this_player()),short +" picks up the "+ arg
		  +" gives it a shake, then drops it.\n");
	    }
	    else if(charmed->query_secondary_attacker(ob)) {
		write(short +" gives an extra aggressive attack.\n");
	    }
	    else {

		tell_room(environment(charmed), short +" leaps upon "+ arg +".\n");
		charmed->add_secondary_attacker(ob);
	    }
	}

	break;

    case "say" :
	if(!humanoid(charmed)) {
	    printf("%s doesn't speak any languages you know.\n",
	      charmed->query_name());
	}
	break;

    case "wield" :
          command("wield "+arg,charmed);
          write(name + " follows orders.\n");
          break;
    case "wear" :
          command("wear "+arg,charmed);
          write(name + " follows orders.\n");
          break;
    case "get":
	if(no_get) {
	    if(no_get == 1 || (CHARMED_WISDOM) > (CHARISMA)) {
		write(name +" refuses to do that!\n");
		no_get = 1;
	    }
	    else {
		no_get = 0;
	    }
	}
	if(!no_get) {
	    if(!arg) {
		tell_room(environment(ob),
		  short +" drops to the ground.\n");
	    }
	    else if((ob = present(arg, environment(charmed)))) {
		if(ob->get()) {
		    if(!(charmed->add_weight((int)ob->query_weight()))) {
			write(name + " can't carry more!\n");
			tell_room(environment(ob), short + " fails to get the " + arg +".\n");
			return 1;               
		    } else {
			tell_room(environment(ob),
			  short +" gets the "+ arg +".\n");
			move_object(ob, charmed);
		    }
		}
		else {
		    write(name +" trys but can't get it.\n");
		}
	    }
	    else {
		tell_room(environment(charmed),name +" looks around for the "+
		  arg +".\n");
	    }
	}
	break;


    case "drop":
	if(no_drop) {
	    if(no_drop == 1 || CHARMED_WISDOM > (CHARISMA)) {
		write(name +" refuses to do that!\n");
		no_drop = 1;
	    }
	    else {
		no_drop = 0;
	    }
	}
	if(!no_drop) {
	    if(!arg) {
		tell_room(environment(ob),
		  name +" drops to the ground.\n");
	    }
	    else if((ob = present(arg, charmed))) {
		if(!ob->drop()) {
		    tell_room(environment(ob),
		      name +" drops the "+ arg +".\n");
		    move_object(ob, environment(charmed));
		    charmed->add_weight((int)ob->query_weight()*(-1));
		}
		else {
		    write(name +" refuses to drop it.\n");
		}
	    }
	    else {
		tell_room(environment(charmed),name +" looks around for the "+
		  arg +".\n");
	    }
	}
	break;

    case "follow":
	if(no_follow) {
	    if(no_follow == 1 || CHARMED_WISDOM > (CHARISMA)) {
		write(name +" refuses to do that!\n");
		no_follow = 1;
	    }
	    else {
		no_follow = 0;
	    }
	}
	if(!no_follow) {
	    follow = clone_object("obj/shadows/follow");
	    follow->follow(this_player(),charmed);
	    write(name +" waits for you to leave.\n");
	    say(name +" watches "+ NAME +" closely.\n");
	}
	break;

    case "stay":
	if(!follow) {
	    write(name +" is not following you.\n");
	}
	else {
	    destruct(follow);
	    if(this_player() && environment(this_player()))
		tell_room(environment(this_player()),name +" gives up the chase.\n"); 
	}
	break;

    case "dismiss":
	write("You release " + name + " from your charm spell.\n");
	destruct(this_object());
	return 1;
	break;

    default:
if(!command(sprintf("%s %s",str,arg?arg:""),charmed)) {
	    write(name +" looks confused.\n");
	    say(short +" looks confused.\n");
	}
	break;
    }
    return 1;
}


void monster_died(object dead) { 
    if(!environment() || !this_player()) return;
    if(dead != charmed) return;  // Dunno why this would happen but...
    if(saved_dead_ob) saved_dead_ob->monster_died(dead);
    tell_object(environment(),
      sprintf(">>>> %s was slain by %s! <<<\n",name,
	(string)this_player()->query_name()));
    destruct(this_object());
}

void monster_heart_beat() {
    object env;
    int left;

    if(!charmed || !(env = environment())) return;
    if(!present(env,environment(charmed))) {
	if(find_call_out("time_out") < 0) {
	    call_out("time_out",300);
	}
    } else {
	remove_call_out("time_out");
    }   
    if(saved_heart_ob) saved_heart_ob->monster_heart_beat();
}

void steal(object from) {
    if(!environment() || !this_player()) return;
    if(environment() == this_player()) return;
    if(from != charmed) return; 
    if(saved_steal_ob) saved_steal_ob->steal(from);
}

void notify_destruct() {
    if(environment()) {
        tell_object(environment(),"You feel as though you have lost a friend.\n");
    }
    if(charmed) {
        charmed->set_short(saved_short);
        charmed->set_long(saved_long);
        charmed->set_dead_ob(saved_dead_ob);
        charmed->set_steal_ob(saved_steal_ob);
        charmed->set_heart_ob(saved_heart_ob);
        charmed->set_charmed_ob(0);
    }
    if(follow) destruct(follow);
    remove_call_out("time_out");
    remove_call_out("dispel");
}

void time_out() { destruct(this_object()); }
void dispel() { time_out(); }
