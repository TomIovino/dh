// Circle - Modified version //
// Based off of Geewhiz's Backstab code -Ash
// This is essentially a weak backstab in combat..
 
 
////////////////////////////////////////////////////////////////////
/////////////// Server Code ////////////////////////////////////////
////////////////////////////////////////////////////////////////////
mapping circleing = ([ ]);
mapping repeat_offender = ([ ]);

int in_church() {
    return environment()->query_no_fight();
}
 
status circle(string str) {
    object circle_ob;
    int flag;

    if(this_player()->query_tactic() != "stealth"){
         write("You must be using stealth to circle an opponent correctly.\n");
           return 1;
     }
    if(!this_player()->query_primary_attack()){
          write("You must be in combat to circle quickly enough!\n");
          return 1;
    }
    if(circleing[this_player()]) {
        repeat_offender[this_player()] += 1;
        if(this_player()->is_npc())
            return 1;
        circleing[this_player()]->dest(1);
        flag = 1;
    }
    circle_ob = clone_object(__FILE__);            // Spawn client
    circleing[this_player()] = circle_ob;       // Create 'lockfile'
    return circle_ob->Do_circle(str,flag);       // Pass off to client
}



 
status valid_call(object which) { // Verify 'which' is a client
    return circleing[this_player()] == which;// *we* spawned
}
 
void clear_repeat() { m_delete(repeat_offender,this_player()); }
 
////////////////////////////////////////////////////////////////////
/////////////// Client Code ////////////////////////////////////////
////////////////////////////////////////////////////////////////////
 
#include <ansi.h>
#include <ansi_filter.h>
#include <mudlib.h>

object obj, primary, weapon, env;
string type, name;
int your_dex;
int dam;
int my_circle;
object prey;
string cco;
#define NAME      (string)this_player()->query_name()
 
varargs void dest(int keeprepeat) { 
    remove_call_out("circle_2");
    remove_call_out("circle_3");
    if(!keeprepeat)
        __FILE__->clear_repeat();
    if(this_object())
        destruct(this_object());
}
 
void CleanUp() { if(this_object()) dest(); }
    
varargs status Do_circle(string str, int mult) {
    // The only thing allowed to call Do_circle are clients
    // made through a call to circle()
    if(!__FILE__->valid_call(this_object())) {
        raise_error("Invalid call to Do_circle!\n");
    }
    move_object(this_object(),this_player());
    call_out("CleanUp",10);   // The client will always kill itself after
                              // 10 seconds.
 
    env = environment(this_player());
 
    this_player()->set_tmp_attrib("PlayerStationary",1);

    cco = str;
    if(!str) {
        notify_fail("circle who?\n");
        dest();
        return 0;
    }
    obj = present(lower_case(str), environment(this_player()));
    if(!obj) {
        notify_fail((name = capitalize(str))+" isn't here to circle!\n");
        dest();
        return 0;
    }
/*
    if(obj->query_hp()*2 < (obj)->query_max_hp()){
        notify_fail("That creature is too aware to be circled!\n");
        dest();
        return 0;
    }
*/
    if(this_player()->query_dead()) {
        notify_fail("You cannot circle in your immaterial form.\n");
        dest();
        return 0;
    }
    if(!living(obj)) {
        notify_fail("Well, it doesn't move. It hardly knows you are here.\n");
        dest();
        return 0;
    }
    if((primary = (object)this_player()->query_primary_attack())) {
        if(!present(primary, environment(this_player()))) {
            notify_fail(B_RED_F BLINK
               "You can't circle someone you're not fighting!\n" NORM);
            dest();
            return 0;
        }
    }
    if(this_player()->query_skill("circle") < 1)
   {
       notify_fail("You just don't have the skill to do that.\n"+NORM);
       dest();
    return 0;
  }
    weapon = (object)this_player()->query_right_weapon();
    if((status)environment(this_player())->query_no_fight() == 1) {
       write("You would displease the Gods.\n");
       dest();
       return 1;
    }

  if((status)environment(this_player())->query_no_fight() == 1){
    write("No.\n");
  dest();
   return 1;
  }
       call_out("circle_2",2);
    return 1;
}
 
void circle_2() {
    if(!this_player())  {
        dest();
        return;
    }
    if(!obj) {
        printf("Your prey is gone!\n");
        dest();
        return;
    }
    if(env != environment(this_player())
    || !this_player()->query_attrib("PlayerStationary")) {
        printf("You moved!  How do you expect to kill %O if you move\nto a different room?\n",name);
        dest();
        return;
    }
    if(!present(obj, environment(this_player()))) {
        printf("Your target is no longer present.\n");
        dest();
        return;
    }
    if((primary = (object)this_player()->query_primary_attack())) {
        if(!present(primary, environment(this_player()))) {
            printf(B_RED_F BLINK
               "You can't circle, you're not fighting!\n" NORM);
            dest();
            return;
        }
    }
    printf("You begin to circle on your prey.\n");
    call_out("circle_3",1);
    return;
}
 
void circle_3() {
    string str;
    int bonus;
    if(!this_player())  {
        dest();
        return;
    }
    if(!obj) {
        printf("Your prey is gone!\n");
        dest();
        return;
    }
    if(env != environment(this_player())
    || !this_player()->query_attrib("PlayerStationary")) {
        printf("You moved!  How do you expect to kill %O if you move\nto a different room?\n",name);
        dest();
        return;
    }
    if(!present(obj, environment(this_player()))) {
        printf("Your target is no longer present.\n");
        dest();
        return;
    }
    if((primary = (object)this_player()->query_primary_attack())) {
        if(!present(primary, environment(this_player()))) {
            printf(B_RED_F BLINK
               "You can't circle someone while not fighting!\n" NORM);
            dest();
            return;
        }
    }
    if(this_player() == obj){
        printf(B_RED_F
                      "Geez!  What are you?  Suicidal?  Get some help.\n"
                      "(Even though suicide is morally neutral and is\n"
                      " neither right, nor wrong... still, you should\n"
                      " try to find somebody you can talk to, it really\n"
                      " does help:)\n"
               NORM);
        dest();
        return;
    }
    str = cco;
    prey = obj;
    your_dex = prey->query_level();
    this_player()->checkskill("circle","dexterity",5);
    
   if((string)this_player()->query_tactic() == "stealth"){
      bonus = (this_player()->query_skill("stealth tactics")+1/10);
      bonus += this_player()->query_spb();
      this_player()->checkskill("stealth tactics","dexterity",5);
    }
    /* for surprising them...must beat their level */
    my_circle = (int)this_player()->query_skill("circle");
    if(random(10)+your_dex <= my_circle){
        printf("%sYou sneak up on %s...%s\n",
          GREY_F, filter_ansi(obj->short()), NORM);
        printf("%sYou surprise %s!%s\n",
          RED_F, filter_ansi(obj->short()), NORM);
        tell_room(environment(this_player()),
          sprintf("%s%s sneaks up on %s...%s\n",
            GREY_F, NAME, filter_ansi(obj->short()), NORM),
          ({ this_player() }));
        tell_room(environment(this_player()),
          sprintf("%s%s surprises %s%s!\n",
            RED_F, NAME, filter_ansi(obj->short()), NORM),
          ({ this_player() }));
        dam = random(my_circle);
        if(dam >= 40) dam = 40;
        dam += bonus;
        if(dam >= 80) dam = 80;
        dam += this_player()->query_level()/2;
        (int)obj->hit_player(dam);
        weapon = (object)this_player()->query_right_weapon();
        if(weapon) type = (string)weapon->query_type();
        this_player()->attack_msg(dam,type,obj,"right");
        this_player()->add_secondary_attacker(obj); // get ready to attack 
        this_player()->set_primary_attack(obj);     // make sure focus of attack
    }
    else {
        printf("%sYou sneak up on %s...%s\n",
          GREY_F, filter_ansi(obj->short()), NORM);
        printf("%s%s sees you coming and dodges the attack!%s\n",
          B_BLUE_F, filter_ansi(obj->short()), NORM);
        tell_object(obj,
          sprintf("%sYou see %s sneaking up on you.%s\n",
            GREY_F, this_player()->query_name(), NORM));
        tell_room(environment(this_player()),
          sprintf("%sYou see %s sneaking up on %s.%s\n",
            GREY_F, this_player()->query_name(), obj->query_name(), NORM),
          ({ this_player(), obj }));
        this_player()->add_secondary_attacker(obj); /* get ready to attack */
        this_player()->set_primary_attack(obj);     /* make sure focus of attack */
    }
    dest();
    return;
}
 
status id(string str) { return str == "Circle"; }
 
