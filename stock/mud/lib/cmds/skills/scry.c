// scry

status scry(string str, mixed alt_type) {
    int i;
    string file;
    if(!str) {
	notify_fail("Scry who?\n");
	return 0;
    }
   if(!this_player()->query_wp())
   {
    write("You must have a wp to use this skill.\n");
    return 1;
   }
    if(!alt_type) alt_type = "divination";
    file = file_name(this_object());
    sscanf(file, "%s#%d", file, i);
    this_player()->load_spell(({
	"target",            this_player(),
	"name",              "scry",
  "type",              "focus",
  "stat",              "intellect",
	"cost",              90,
	"level",             90,
	"cast time",         3,
	"spell object",      file,
	"argument",          str,
	"passive",
      }));
    return 1;
}

/*******************************************************************/
/* called when this is cloned to target */
status cast_spell(object caster,object target,object prev,int dmg) {
    object ob;
    string rname;
    rname = (string)caster->query_spell_argument();
    ob=find_player(rname);
    if(!ob) {
	write("You cannot locate "+ capitalize(rname) +".\n");
        destruct(this_object());
	return 1;
    }

    if(ob->query_var("awareness")){
          tell_object(ob, "Someone has attempted to scry you.\n");
    }
    if(ob->query_var("fortitude")){
	    destruct(this_object());
         return 1;
    }
     log_file("SPELL",
      this_player()->query_real_name()+
      " scried "+ob->query_alt_name()+"  "+
         "on "+ctime()+"  "+
      ob->query_current_room()+"\n");
    if(ob->query_security_level() > 0) {
	write("You cannot locate "+ capitalize(rname) +".\n");
	    destruct(this_object());
	return 1;
    }
    if(ob->query_invis() && ob->query_security_level() <= 0) {
	write(capitalize(rname) +" is somewhere about, but their "+
	  "invisibility disrupts the locating ability of the spell.\n");
	    destruct(this_object());
	return 1;
    }
    if(ob->query_npc()) {
	write("You can only locate players.\n");
	    destruct(this_object());
	return 1;
    }
    if(environment(ob)) {
	if(environment(ob)->query_no_track()) {
	    write("Your mind cannot seem to get a firm grip upon "+
	      capitalize(rname) + ".\n");
	    destruct(this_object());
	    return 1;
	
    }
    write("Your mind peers across the domains, you get a picture of\n"+
	  "the area where "+ capitalize(rname) +" is located:\n\n");
	environment(ob)->long();
    }
    destruct(this_object());
    return 1;
}

status id(string str) { return str == "spell"; }
int query_no_save(){ return 1; }

