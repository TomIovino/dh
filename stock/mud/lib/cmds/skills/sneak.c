

status sneakinc(){
      if((string)this_player()->query_tactic() == "stealth"){
      this_player()->checkskill("stealth tactics","dexterity",50);
      }    
      this_player()->checkskill("sneak","dexterity",5);
   }

status main(string str) {
      string x,y;
      int bonus;
      if(sscanf(this_player()->query_current_room(), "%s/arena/%s", x,y)== 2) {
      return 1;
      }

      if(this_player()->query_sneaking()) {
        write("You stop moving silently.\n");
         this_player()->set_sneaking(0);
        return 1;
      }

         remove_call_out("sneakinc");
         call_out("sneakinc",3);

	if(!this_player()->query_skill("sneak")){
           write("You attempt to sneak.\n");  return 1; }

	if(this_player()->query_primary_attack()) {
	   write("Not in combat.\n");
	   return 1;
         }

  bonus = this_player()->query_skill("stealth tactics")+1/10;
  if(this_player()->query_skill("sneak")+bonus > random(100)){
        remove_call_out("sneakinc");
         call_out("sneakinc",3);
	 write("You begin to sneak.\n");
         this_player()->set_sneaking(1);
         this_player()->set_position("sneaking");
	 return 1;

}
      write("You begin to sneak.\n");
      return 1;
}


