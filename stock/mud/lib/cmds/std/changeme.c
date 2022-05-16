#define TP this_player()
#define VALID "secure/valid_d"

int set(string what, string how,int level)
{
   int mod;
     log_file("CME",
      TP->query_name()+":"+what+":"+how+":"+level+":"+time()+"\n");
     mod = level % 10;
     level = (level/10)*9;
     TP->set_level(1);
     if(what == "resetallstats")
      TP->set_all_stats(25);
     if(what == "race"){
      TP->set_race(how);
      TP->set_class(TP->query_profession());
     }
     if(what == "class"){
      TP->set_profession(how);
      TP->set_class(how);
     }
     TP->init_stats(TP->query_race());
     TP->set_level(level+mod);
     TP->clear_who_string();
     TP->init_static_link();
 return 1;
}

status main(string arg){
   string type,choice; 
   int level;
   level = this_player()->query_level();
   type = choice = "";
   if(!arg)
    { write("changeme <resetallstats|race|class> <selection>\n");
      return 1;
    }
//   if(!(status)environment(this_player())->query_no_fight() == 1)
//    { write("Goto a church\n"); return 1; }

    if(arg == "resetallstats")
        return set("resetallstats",choice,level);
    if(sscanf(arg,"%s %s",type,choice) == 2) 
    {
    if(type == "race")
     if(VALID->is_valid_race(choice))
        return set("race",choice,level);
    if(type == "class");
     if(VALID->is_valid_profession(choice))
        return set("class",choice,level);
    }
  
  write("invalid syntax\n");
  return 1;
     
}
