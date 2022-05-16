/* teleport, teleport other */


status teleport(string str, mixed alt_type) {
  int level, i;
  string file, *destinations, dom1, dom2, tmp, who;
  string spell_name;
  int spell_level,x,y;
  status party;
  object tel;
   if(sscanf(this_player()->query_current_room(), "%s/castle/%s", x,y) == 2) {
    write("Not to castle rooms.\n");
    return 1;
   }


  file = file_name(this_object());
  sscanf(file,"%s#%d",i);

  if(!(tel = present("teleport", this_player()))) {
    tel = clone_object("inherit/objs/teleport");
    move_object(tel, this_player());
    notify_fail("You should memorize a 'destination' first.\n");
    return 0;    
  }

  if(!str) {
      notify_fail("teleport <destination>\n"); 
    return 0;
  }

  destinations = (string *)tel->query_destinations();
  if((i = member_array(str, destinations)) == -1) {
    write("You have not memorized \""+ str +"\" as a destination.\n"+
          "type 'destination <location>' to memorize this area as a "+
          "destination.\n");
    return 1;

  }
  sscanf((string)this_player()->query_current_room(),"d/%s/%s",dom1,tmp);
  sscanf(destinations[i+1],"d/%s/%s",dom2,tmp);
  if(dom2 == "inn"){
       write("Arcane magic protects returning to that area.\n");
       return 1;
                    }

  if(dom2 == "castle"){
        write("Not to castles.\n");
        return 1;
     }
  if(environment(this_player())->query_no_summon()) {
    write("Something in the area prevents you from teleporting.\n");
    return 1;
  }
  this_player()->load_spell(({
  "target",            who,
  "name",              "teleport",
   "cost",             60,
        "stat",        "intellect",
        "type",        "focus",
  "level",             50,
  "spell object",      file,
  "cast time",         3,
  "argument",          destinations[i+1],
  "passive",
   ((party) ? "area" : "dummy"),
  }));
  return 1;
}


status cast_spell(object caster,object target,object prev,int level) {
  string party_name, *party_members;
  object party;

  if(prev) { /* area effect (party teleport) */
    if((string)caster->query_party() != (string)target->query_party()) {

      destruct(this_object()); /* not party teleport */
      return 1;
    }
  }
    
  if(this_player()->query_security_level()) {
    write("ARG:"+(string)caster->query_spell_argument()+"\n");
  }
  
  if(living(target)) {
    if(target->query_npc()) {
      if((int)caster->query_spell_dmg() != level) {
        write(target->query_name() +" gets upset with you.\n");
        target->add_secondary_attacker(caster);
        target->set_primary_attack(caster);
        destruct(this_object());
        return 1;
      }
    }

    /* catch is so this won't stop heart beat on failed move */
    if(catch(target->move_player("X#"+(string)caster->query_spell_argument()))){
      write("Your destination currently does not exist!\n");
    }
  }
  else if(target->get() && !target->drop()) {
    if(catch(move_object(target,(string)caster->query_spell_argument()))) {
      write("Your destination currently does not exist!\n");
    }
    else {
      write("The "+ target->query_name() +" disappears in a puff of smoke.\n");
      say(caster->query_name() +" makes the "+ target->query_name() 
         +" disappear in a puff of smoke.\n");
      tell_room(find_object((string)caster->query_spell_argument()),
        "A "+ target->query_name() +" appears in a puff of smoke.\n");
    }
  }
  else {

    write("Something prevents the "+ target->query_name() +" from leaving.\n");
  }
  destruct(this_object());
  return 1;
}



