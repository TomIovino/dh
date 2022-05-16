#include <ansi.h>
#include <mudlib.h>
inherit FOOD;
object item,player;
int splat;
int smash;
int enchant;
string pname;

int query_enchant(){ return 1; }
reset(arg){
    ::reset(arg);
    set_name("mango");
    set_alias("fruit");
    set_short(B_GREEN_F+"juicy mango"+NORM);
    set_long("A throwable mango...wow\n\
"+BOLD+RED_F+"IF YOU ARE CAUGHT SPAMMING WITH THIS I WILL PERSONALLY KILL YOU"+NORM+"\n\
"+BOLD+BLUE_F+"The mango is now LOGGED for my enjoyment, so dont spam."+NORM+"\n\
Thank you, and have fun.\n");
    set_alt_name("throwmango");
    set_light(1);
    set_weight(3);
    set_value(2000);
}
init(){
    ::init();
    add_action("throw","throw");
    add_action("eat","eat");
    add_action("drop","drop");
    add_action("quit","quit");
}
eat(str){
    if(str!="mango"){
	return 0;
    }
    write(YELLOW_F+"You slowly bring the massive mango to your mouth...\n"+NORM);
    write(RED_F+"The mango comes to life!\n"+NORM);
    write(BOLD+RED_F+"The mango "+BLINK+"SMACKS "+NORM+RED_F+BOLD+"you silly.\n"+NORM);
    write(BLUE_F+"Mango says : I don't think so...go eat an apple.\n"+NORM);
    say(RED_F+this_player()->query_name()+" gets smacked really hard by the mango!\n"+NORM);
    return 1;
}
quit(str){
    write(RED_F+"The mango screams : "+YELLOW_F+"You may not keep me! hahaha\n"+NORM);
    write(BOLD+RED_F+"The mango self-destructs!...splat\n");
    say(GREEN_F+"The mango from "+this_player()->query_name()+" self-destructs!...SPLAT!\n"+NORM);
    log_drop();
    destruct(this_object());
    return 0;
}
drop(str){
    if(str!="mango"){
	return 0;
    }
    write(BROWN_F+"You drop the mango and it splatters all over the ground!\n"+NORM);
    destruct(this_object());
    return 1;
}
throw(str){
    if(!str) return;

    if(sscanf(str,"%s to %s",item,player)!=2){
	write("Try: throw mango to <player>\n");
	return 1;
    }
    smash = 0;
    player=find_player(player);
    if (!player) {
	write ("Throw mango at who?\n");
	return 1;
    }
    if(player->query_security_level() > this_player()->query_security_level()){
	write("Throw mango at who?\n");
	return 1;
    }
    if(player->query_invis()){
        write ("Throw mango at who?\n");
    return 1;
    }
    pname=player->query_name();
    if(present("throwmango",player)){
	write(YELLOW_F+"That player already has a mango! try someone else.\n"+NORM);
	return 1;
    }
    say(BOLD+BLUE_F+this_player()->query_name()+" throws a mango at "+pname+"!\n"+NORM);
    write(BLUE_F+"You throw the mango at "+capitalize(player->query_name())+"..WHAP\n");
    tell_room(environment(player),GREEN_F+pname+" gets hit by a Mango!\n\
"+BOLD+RED_F+"It was thrown by "+this_player()->query_name()+"!\n"+NORM,({player}));
    tell_object(player,YELLOW_F+this_player()->query_name()+" just threw a mango at you...WHAP!\n"+NORM);
    splat=random(100);
    if(splat < 15){
	write("The mango goes splat and gets smashed.\n");
	log_splat();
	tell_object(player,BROWN_F+"the mango splatters everywhere as it hits you.\n"+NORM);
	destruct(this_object());
	return 1;
    }
    move_object(clone_object("/inherit/objs/mango"),player);
    player->recalc_carry();
    log_throw();
    destruct(this_object());
    return 1;
}
log_throw(){
    log_file("MANGO",
      this_player()->query_name()+" "+
      "threw mango at "+pname+" ..time: "+ctime()+"\n");
}
log_drop(){
    log_file("MANGO",
      this_player()->query_name()+" "+
      "dropped the mango ..time: "+ctime()+"\n");
}
log_splat(){
    log_file("MANGO",
      this_player()->query_name()+" "+
      "threw mango at "+pname+" and it DESTED time: "+ctime()+"\n");
}
