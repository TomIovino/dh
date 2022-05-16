// steal


#include <mudlib.h>

#define NAME        (string)this_player()->query_name()
#define CLOSE_ENUF  3
#define TP this_player()
#define QN query_name()
#define QA query_house() 
#define MY_STAT (int)this_player()->query_skill("steal")

string tmp1, tmp2;
string cco;
object victim, obj, *env;
int mystat, yourstat, i, money;

void check_env(object player);

mapping stealing = ([ ]);
mapping repeat_offender = ([ ]);

status steal(string str) {
    object steal_ob;
    int flag;
     if((status)environment(this_player())->query_no_steal() == 1){
           write("You cannot steal here.\n");
           return 1;
     }
    if(stealing[this_player()]) {
        repeat_offender[this_player()] += 1;
        stealing[this_player()]->dest();
        flag = 1;
    } 
    steal_ob = clone_object(__FILE__);       // Spawn client
    stealing[this_player()] = steal_ob;  // Create 'lockfile'
    return steal_ob->Do_steal(str,flag);
}

void clear_repeat(object ob) {  m_delete(repeat_offender,ob); }

varargs void dest(int keeprepeat) { 
    remove_call_out("CleanUp");
    remove_call_out("steal2");
    if(!keeprepeat)
       __FILE__->clear_repeat(this_player());
    destruct(this_object());
}

void CleanUp() { dest(); }

varargs status Do_steal(string str, int mult) {
    move_object(this_object(),this_player());
    call_out("CleanUp",6);

    money = 0;

    this_player()->set_tmp_attrib("PlayerStationary",1);
    if(!MY_STAT){ write("You should learn how to steal.\n"); return 1; }
    if(this_player()->query_position() != "sneaking"){
        write("Stealing is awful hard to do with out sneaking.\n");
        return 1;
    }
    if(!str) {
    notify_fail("Steal from who?\n");
    dest();
    return 0;
    }
    if(this_player()->query_dead()) {
    printf("You cannot steal in your immaterial form.\n");
    dest();
    return 0;
    }
    if(this_player()->query_attack()) {
    notify_fail("You cannot steal from someone while engaged in combat!\n");
    dest();
    return 0;
    }
    if((status)environment(this_player())->query_no_steal() == 1){
    notify_fail("You cannot steal here.\n");
    dest();
    return 0;
    }
    if(!sscanf(str, "%s from %s", tmp1, tmp2)) {
    notify_fail("Steal what from who?\n");
    dest();
    return 0;
    }

    env = all_inventory(environment(this_player()));
    victim = present(lower_case(tmp2), environment(this_player()));
    if(!victim) {
    notify_fail("That person is not here.\n");
    dest();
    return 0;
    }


  if(!victim->is_living()){
    write("Steal what from who?\n");
    return 1;
   }
 
    if(query_once_interactive(victim) && !query_ip_name(victim)) {
    notify_fail("You cannot steal from someone that is not logged on!\n");
    dest();
    return 0;
    }

    if(mult && victim)
        tell_object(victim,sprintf("\n\n%-=75s\n\n",
   sprintf("You have a strange feeling that %s is trying to steal %O!!!",
          (string)this_player()->query_name(),tmp1)));
    printf("You begin sneaking up on %s.\n", victim->query_name());
    remove_call_out("steal2");
    call_out("steal2",3,str);
    return 1;
}

void steal2(string str) {
    status added_weight;
    int bonus;
    added_weight = 0;
    if(!this_player()) return;
    if(!victim) {
    write("Your victim is no longer around.\n");
    dest();
    return;
    }
    if(!present(victim,environment(this_player()))
    || !this_player()->query_attrib("PlayerStationary")) {
    write("You are no longer in your victim's room.\n");
    dest();
    return;
    }
    if(this_player()->query_attack()) {
    printf("You cannot steal while engaged in combat!\n");
    dest();
    return;
    }
    if(victim->query_attack()) {
    write("Your victim seems to be using the item right now..\n");
    dest();
    return;
    }

    if(sscanf(tmp1, "%d coins", money) || sscanf(tmp1, "%d coin", money)) {
    if(money > 500) money = 500;
    if(money < 0) {
        log_file("/STEALBUG",sprintf("%s\n\tTP: %O\n\tTyped = %O\n\tmoney = %O\n\n",
                    timestamp(),this_player(),tmp1,money));
        printf("\nTrying to abuse bugs isnt nice.\n");
        printf("Oh, and by the way, you're busted cause it just got logged.\n");
        printf("Might wanna send mail 'gods' way and try to explain yourself.\n\n");
        printf("\nJust wait til Mom or Dad gets home....\n");
        money = 0;
        dest();
        return;
    }
        
    if(victim -> query_money() < money) {
        write("You don't think "+ victim -> query_name() +" has that "+
          "many coins.\n");
        money = 0;
        dest();
        return;
    }
    }

  if(!money) { 
    obj = present(lower_case(tmp1), victim);
    if(!obj) {
        write(victim -> query_name() +" has no "+ tmp1 +" on "+ 
          victim->query_objective() + ".\n");
        dest();
        return;
    }
    if(!obj->get() || obj->query_auto_load() || obj->query_no_steal_flag()) {
        printf("There is no way you could steal that from %s.\n",
          victim->query_name());
        dest();
        return;
    }
    if(obj->query_worn() || obj->query_wielded()){
         printf("Somehow you think that item would be noticed missing..\n");
         dest();
         return;
    }
  }
    if(victim->query_security_level()) {
    printf("%s grabs you by the neck and glares at you.\n",
      victim->query_name());
    tell_room(environment(victim),
      sprintf("%s grabs %s by the neck and glares at %s.\n",
        victim->query_name(), this_player()->query_name(),
        this_player()->query_objective()), ({ victim, this_player() }));
    tell_object(victim,
      sprintf("%s foolishly tried to steal %s from you.\n",
        this_player()->query_name(),
        (obj->short() ? obj->short() : "something")));
    dest();
    return;
    }

    if(money) {
    obj = clone_object("obj/money");
    obj -> set_money(money);
    }

    if(this_player()->query_invis()) {
    this_player()->toggle_invis();
    }

   yourstat = ((int)victim->query_level());
   yourstat += ((int)victim->query_stat("intellect"));
   yourstat += random(50);
   mystat = ((int)this_player()->query_level());
   mystat += ((int)this_player()->query_skill("steal"));
   mystat += ((int)this_player()->query_skill("steal")/10);

   if(this_player() == victim){
       write("Sure you steal that from yourself.. good job loser.\n");
       dest();
       return;
   }
    this_player()->checkskill("steal","dexterity",10);
    if((string)this_player()->query_tactic() == "stealth"){
      bonus = this_player()->query_skill("stealth tactics")+1/10;
      this_player()->checkskill("stealth tactics","dexterity",25);
    }

    /* IF I WIN !! */
    if(mystat+bonus > yourstat) {   
    if(this_player()->query_attack() == 1){
        write("You cannot steal while engaged in combat!\n");
      dest();
      return; 
    }
    if(victim->query_attack() == 1){
         write("You cannot steal in combat!\n");
     dest();
      return; 
    }
    if(!this_player()->add_weight(obj->query_weight())) {
        write(obj -> query_name() +" is too heavy for you to carry!\n");
        dest();
        return;
    } else {
        added_weight = 1;
    }

    if(obj -> query_worn()) {
        write("You manage to remove the "+ obj -> query_name() +" that "+
         victim -> query_name() +" is wearing, without "+victim -> 
              query_objective()+" noticing.\n");
        obj -> drop(1);  /* silently */
        if(added_weight)
             this_player()->add_weight(negate((int)obj->query_weight()));
        this_player()->recalc_carry();
        dest();
        return;
    }

    if(obj->query_wielded()) {
        write("You manage to remove the "+ obj -> query_name() +" that "+
            victim -> query_name() +" is wielding without "+victim ->
            query_objective()+" noticing.\n");
        obj -> drop(1);  /* silently */
        if(added_weight) this_player()->add_weight(negate((int)obj->query_weight()));
        this_player()->recalc_carry();
        dest();
        return;
    }

    check_env(this_player());
    

      if(money != 1)  // dont log stealing 1 coin.
      log_file("/STEAL",
      "T:"+TP->QN+"("+TP->QA+"),O: "+obj->query_name()+
      ",V:"+victim->QN+"("+victim->QA+")"+time()+"\n");
       write("You steal "+ obj -> short() +" from "+ 
       victim -> query_name() +"'s pockets.\n");
    if(money) {
        this_player()->add_money(money);
        victim -> add_money(-money);
        money = 0;
        destruct(obj);
    }
    else {
        move_object(obj, this_player());    
        this_player()->recalc_carry();
        victim->recalc_carry();
        victim -> steal();
    }
    dest();
    return;
    }


    /* a close attempt, but not quite */
    if(MY_STAT > 19 && ((yourstat - mystat) < CLOSE_ENUF)) {  
    if(!this_player()->add_weight(obj->query_weight())) {
        write(obj -> query_name() +" is too heavy. You cannot carry any more!\n");
    } else {
        added_weight = 1;
    }
    if(obj -> query_worn()) {
        write("You manage to remove the "+ obj -> query_name() +" that "+
          victim -> query_name() +" is wearing.\n");
        obj -> drop(1);  /* silently */
        if(added_weight) this_player()->add_weight(negate((int)obj->query_weight()));
        this_player()->recalc_carry();
        dest();
        return;
    } else {
        if(obj -> query_wielded()) {
        write("You manage to remove the "+ obj -> query_name() +" that "+
          victim -> query_name() +" is wielding.\n");
        obj -> drop(1);  /* silently */
        if(added_weight) 
        this_player()->add_weight(negate((int)obj->query_weight()));
        this_player()->recalc_carry();
        dest();
        return;
        } else {
        write("You manage to steal "+ obj -> short() +" from "+
          victim -> query_name() +"!!\n");
        }
    }

    if(money) {
        this_player()->add_money(money);
        victim->add_money(-money);
        money = 0;
        destruct(obj);
    } else {
        move_object(obj, this_player());
        this_player()->recalc_carry();
        victim->recalc_carry();
        victim -> steal();
    write("However, "+ victim -> query_name() +" catches you in the act!\n");
    }
    }

    /* here's where we fail totally !!! */
    else {
    write("You fail to steal "+ obj -> short() +" from "+ 
      victim -> query_name() +"\n");
    }
    check_env(this_player());
    if(money) destruct(obj);  /* gets rid of tmp money object cloned */
    if(victim -> query_npc()) {
    write(victim -> query_name() +" becomes enraged and begins to attack!\n");
    victim->hit_player(0);
    }
    else {
    write(victim -> query_name() +" gets angry with you!\n");
    tell_object(victim, this_player()->query_name()+" attempted to steal "+
      "from you!\n");
    }
    dest();
    return;
}


/* To see if anyone else notices this dastardly feat! */
void check_env(object player) {
    if(!player) return;
    for(i=0; i<sizeof(env); i++) {
    if(!env[i]) return;
    if((env[i] != victim) && (env[i] != player)) {
        if(mystat < (env[i]->query_level())) {
        if(!present(victim,environment(player))) return;
        tell_object(env[i], "You notice "+player -> query_name()+" stealing "+
          obj -> query_short() +" from "+ victim -> query_name() +"!!\n");
        }
    }
    }
}

