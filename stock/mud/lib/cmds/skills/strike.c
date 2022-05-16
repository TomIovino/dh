// Backstab - Modified version //
// Fixed.
 
 
////////////////////////////////////////////////////////////////////
/////////////// Server Code ////////////////////////////////////////
////////////////////////////////////////////////////////////////////

mapping backstabbing = ([ ]);
mapping repeat_offender = ([ ]);



 
status strike(string str) {
    object bstab_ob;
    int flag;
    if(this_player()->query_tactic() != "stealth"){
         write("You must be using stealth to strike an opponent correctly.\n");
           return 1;
     }


    if(backstabbing[this_player()]) {
        repeat_offender[this_player()] += 1;
        if(this_player()->is_npc())
            return 1;
        backstabbing[this_player()]->dest(1);
        flag = 1;
    } 
    bstab_ob = clone_object(__FILE__);            // Spawn client
    backstabbing[this_player()] = bstab_ob;       // Create 'lockfile'
    return bstab_ob->Do_backstab(str,flag);       // Pass off to client
}
 
status valid_call(object which) { // Verify 'which' is a client
    return backstabbing[this_player()] == which;// *we* spawned
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
int my_backstab;
object prey;
string cco;
#define NAME      (string)this_player()->query_name()
 
varargs void dest(int keeprepeat) { 
    remove_call_out("backstab_2");
    remove_call_out("backstab_3");
    if(!keeprepeat)
        __FILE__->clear_repeat();
    if(this_object())
        destruct(this_object());
}
 
void CleanUp() { if(this_object()) dest(); }
    
varargs status Do_backstab(string str, int mult) {
    // The only thing allowed to call Do_backstab are clients
    // made through a call to backstab()
    if(!__FILE__->valid_call(this_object())) {
        raise_error("Invalid call to Do_backstab!\n");
    }
    move_object(this_object(),this_player());
    call_out("CleanUp",10);   // The client will always kill itself after
                              // 10 seconds.
 
    env = environment(this_player());
 
    this_player()->set_tmp_attrib("PlayerStationary",1);

    cco = str;
    if(!str) {
        notify_fail("strike who?\n");
        dest();
        return 0;
    }
    obj = present(lower_case(str), environment(this_player()));
    if(!obj) {
        notify_fail((name = capitalize(str))+" isn't here to strike!\n");
        dest();
        return 0;
    }
/*
    if(obj->query_hp() < obj->query_max_hp()){
        notify_fail("That creature is too aware to be struck!\n");
        dest();
        return 0;
    }
*/
    if(this_player()->query_dead()) {
        notify_fail("You cannot strike in your immaterial form.\n");
        dest();
        return 0;
    }
    if(!living(obj)) {
        notify_fail("Well, it doesn't move. It hardly knows you are here.\n");
        dest();
        return 0;
    }
    if((primary = (object)this_player()->query_primary_attack())) {
        if(present(primary, environment(this_player()))) {
            notify_fail(B_RED_F BLINK
               "You can't strike someone while fighting!\n" NORM);
            dest();
            return 0;
        }
    }
    if(this_player()->query_skill("strike") < 1)
   {
       notify_fail("You just dont have the skill to do that.\n"+NORM);
       dest();
    return 0;
  }
    weapon = (object)this_player()->query_right_weapon();
    if((status)environment(this_player())->query_no_fight() == 1) {
       write("You would displease the Gods.\n");
       dest();
       return 1;
    }

/*  if(!weapon) {
        notify_fail("You need a weapon to strike with!\n");
        dest();
        return 0;
    }
    type = weapon->query_type();
    if(type != "thrust"){
        notify_fail("Your weapon is not suited for a strike.\n");
        dest();
        return 0;
    }
    if(weapon->query_length() > 25) {
        notify_fail("Your weapon is much too large for strike.\n");
        dest();
        return 0;
    }
*/
  if((status)environment(this_player())->query_no_fight() == 1){
    write("No.\n");
  dest();
   return 1;
  }
       call_out("backstab_2",2);
    return 1;
}
 
void backstab_2() {
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
        if(present(primary, environment(this_player()))) {
            printf(B_RED_F BLINK
               "You can't strike someone while fighting!\n" NORM);
            dest();
            return;
        }
    }
    if(obj->query_primary_combat()){
        printf("%s is already fighting!\n",to_string(obj->query_name()));
        dest();
        return;
    }
    if(primary == obj) {
        printf("You are already in combat with that monster.\n");
        dest();
        return;
    }
    printf("You begin to sneak up on your prey.\n");
    call_out("backstab_3",1);
    return;
}
 
void backstab_3() {
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
        if(present(primary, environment(this_player()))) {
            printf(B_RED_F BLINK
               "You can't strike someone while fighting!\n" NORM);
            dest();
            return;
        }
    }
    if(obj->query_primary_combat()){
        printf(str+" is already fighting!\n");
        dest();
        return;
    }
    if(primary == obj) {
        printf("You are already in combat with that monster.\n");
        dest();
        return;
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
    if((string)this_player()->query_tactic() != "stealth"){
      dest();
      return;
    }
      this_player()->checkskill("strike","dexterity",6);
      this_player()->checkskill("stealth tactics","dexterity",5);
      bonus = this_player()->query_skill("stealth tactics")+1/10;
      bonus = this_player()->query_stat("dexterity")+1/10;
      bonus = this_player()->query_spb();

    /* for surprising them...must beat their level */
    my_backstab = (int)this_player()->query_skill("strike");

    if(obj->query_hp()+obj->query_level() < obj->query_max_hp()){
           // +QL 2 ADJ 4 +STA FACTOR 
       notify_fail("That creature is too aware to be striked!\n");
        dest();
        return 0;
    }
    if(random(10)+your_dex <= my_backstab){
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
        dam = random(my_backstab);
        dam += bonus;
        if(dam >= 100) dam = 100;
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
        this_player()->attack();                    /* attack */
    }
    dest();
    return;
}
 
status id(string str) { return str == "BackStab"; }
 
