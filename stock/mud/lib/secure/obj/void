#include <mudlib.h>
#include <ansi.h>

inherit ROOM;

#define WRAP(COLOR,MSG) (COLOR + wrap(MSG,60) + NORM)

void reset(int arg) {
    set_short(RED_F "The Void" NORM);

    set_long(WRAP(B_BLACK_F,"The void. A place of utter nothingness. An onimous "
        "shrine of emptiness. Underwhelming blankness. Devoid of everything. "
        "Containing nothing.  You are in the void and you do not like it."
    ));


    set_light(10);

    set_exits( ({
        "/secure/void", "out"
    }) );
}


int clean_up(int arg) { return 0; }

void init() {
    ::init();
    add_action("cmd_out","out");
    call_out("welcome",0,this_player());
    if(!this_player() || !this_player()->query_env_var("STAY_IN_VOID"))
        call_out("auto_out",(this_player() && this_player()->query_security_level()) ? 20 : 2,this_player());
}


void welcome(object who) {
    if(who && present(who)) {
        tell_object(who,B_RED_F "\nNothingness embraces you and whispers, 'Welcome to the Void'" NORM "\n\n");
    }
}

void object_entered(object ob) {
    if(!ob)
        return;
        
    if(query_once_interactive(ob))
        return;
        
    if(ob->is_player())
        return;
        
    call_out("destroy_object",0,ob);
}

void destroy_object(object ob) {
    string s;
    object *obs;
    int i;
    
    if(!ob || !present(ob) || ob->query_no_destroy_object())
        return;
        
    i = sizeof(obs = deep_inventory(ob));
    if(i) while(i--) move_object(obs[i],this_object());

    if(s = (string) ob->short())
        tell_room(this_object(),"\nA bolt of lightning comes from Nothingness\n\tand utterly vaporizes " + s + "!\n");
        
    destruct(ob);
}



void auto_out(object who) {
    if(who && present(who)) {
        command("*out",who);
        write(B_RED_F "\nNothingness releases you and whispers, 'Nothingness Implies Emptiness'" NORM "\n\n");
    }
}        

int query_light(){ return 10; }

varargs
int cmd_out(string arg) {
    string name;
    object room;

    if(!this_player()->query_security()) {
        this_player()->church();
        return 1;
    }

    this_player()->move_player("Wizard Complex#/d/over/inner");
    return 1;
}

