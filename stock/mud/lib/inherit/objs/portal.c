#include <ansi.h>
#include <mudlib.h>
object dest;
string name;
inherit TREASURE;

reset(arg){
    ::reset(arg);
    call_out("dest",60);
    if(arg)return;
    set_name("portal");
    set_short("");
    set_extra_long(RED_F+"a "+BOLD+"shimmering "+NORM+RED_F+"portal hovers in the air.\n"+NORM);
    set_long("a portal");
    set_alias("portal");
    set_weight(100000);
}
init(){
    ::init();
    if(!dest) return;
    add_action("trans","enter");
    add_action("long","look");
    add_action("long","l");
}
long(str){
    if(str!="portal"){
	return 0;
    }
    write("\
"+BOLD+RED_F+"A mystical portal used to summon people.\n\
"+YELLOW_F+"You gaze into the shimmering portal and see:\n\
"+GREY_F+"---------------------------------------------------------\n"+NORM);
    write(dest->query_long());
    write("\
"+GREY_F+"---------------------------------------------------------\n"+NORM);
    return 1;
}
trans(str){
    if(str!="portal"){
	write("Enter what?\n");
	return 1;
    }
    if(!present("pkey",this_player())){
	write(BOLD+BLUE_F+"You try to enter the portal but fail.\n"+NORM);
	return 1;
    }
    write(BOLD+BLUE_F+"You enter the "+CYAN_F+"shimmering "+BLUE_F+"portal!\n"+NORM);
    say(BOLD+BLUE_F+this_player()->query_name()+" enters the portal.\n"+NORM);
    move_object(this_player(),dest);
    command("look",this_player());
    tell_room(dest,BOLD+BLUE_F+this_player()->query_name()+" arrives through a shimmering portal!\n"+NORM);
    destruct(this_object());
    return 1;
}
void set_dest(object location) {dest = location; }
dest() {
    destruct(this_object());
}
