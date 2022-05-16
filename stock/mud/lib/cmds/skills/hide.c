string *query_cmd_aliases() { return
({"h", }); }


status hideinc(){
      if((string)this_player()->query_tactic() == "stealth"){
      this_player()->checkskill("stealth tactics","dexterity",25);
      this_player()->checkskill("hide","dexterity",5);
      }    
   }

status main(string str) {
      string x,y;
      int bonus;
      if(sscanf(this_player()->query_current_room(), "%s/arena/%s", x,y)== 2) {
      return 1;
      }
      if(this_player()->query_hiding()) {
        write("You step out of the shadows.\n");
         this_player()->set_hiding(0);
         say(this_player()->query_name() + " steps out of the shadows.\n");
        return 1;
      }

        if((string)this_player()->query_tactic() != "stealth"){
           write("You need to use stealth tactics for this skill.\n");
           return 1; 
         }
   
         remove_call_out("hideinc");
         call_out("hideinc",3);

	if(!this_player()->query_skill("hide")){
           write("You attempt to hide.\n");  
         return 1; }

	if(this_player()->query_primary_attack()) {
	   write("Not in combat.\n");
	   return 1;
         }

  bonus = this_player()->query_sbp();
  if(this_player()->query_skill("hide")+bonus > random(100)){
         remove_call_out("hideinc");
         call_out("hideinc",3);
	 write("You hide in the shadows.\n");
         this_player()->set_hiding(1);
	 say(this_player()->query_name() + " vanishes into the shadows.\n");
	 return 1;
  }
      write("You hide in the shadows.\n");
      return 1;
}


