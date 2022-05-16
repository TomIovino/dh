/* send function. -Belial. 4-4-96. Allows one to send living or non living */
/* objects to players or wizards */

int main(string arg) {
    object player,thing;
    string what,who;
    int result;
    if(sscanf(arg,"%s to %s", what, who) !=2) return
	notify_fail("Syntax: send <what> to <who>\n");
    if(!who || !what || who=="" || what=="") return 1;
    player=find_player(who);
    if(!player) {
	write("Can't find that person.\n");
	return 1;
    }
    thing=present(what,this_player());
    if(!thing) thing=present(what,environment(this_player()));
    if(!living(thing)) result=transfer(thing,player);
    else {
	move_object(thing,environment(player));
	write(thing->query_real_name()+" moved to "+
	  capitalize(player->query_real_name())+".\n");
	if(thing->query_name()!="Someone")
	    tell_room(environment(player),thing->query_name()+" suddenly "+
	      "materializes.\n");
	return 1;
    }
    if(!result) {
	write("Object transfered.\n");
	tell_room(environment(player),this_player()->query_name()+
	  " teleports "+thing->query_name()+" to "+player->query_name()+
	  "'s pocket.\n");
    }
    else if(result==1) {
	write("Too heavy for "+player->query_real_name()+".");
	if(player->query_security_level() > 0) {
	    write("Used move_object instead.\n");
	    if(environment(thing)==this_player())
		this_player()->add_weight(-(thing->query_weight()));
	    tell_object(player,this_player()->query_name()+" teleports "+
	      thing->query_name()+" to you.\n");
	    move_object(thing,player);
	}
	write("\n");
    }
    else {
	write("Impossible!\n");
	return 1;
    }
    return 1;
}
