/** id (ash)  **/
string *query_cmd_aliases() { return
({"id", }); }



status identify(string targ, mixed alt_type) {
   int level, i,x;
   string file;



   if(!targ) {
     notify_fail("identify what?\n");
     return 0;
   }    
   file = file_name(this_object());
   sscanf(file,"%s#%d",file,i);

   this_player()->load_spell(({
   "target",       targ,
   "name",         "identify",
   "cost",         50,
   "level",        50,
   "type",              "focus",
   "stat",              "intellect",
   "immune",       "identify",
   "damage",       level,
   "cast time",    2,
   "spell object", file,
   "passive",
   }));
   return 1;
}


status cast_spell(object caster,object ob,object prev,int level) {
  int ac, wc, mon_level, p_level, level_dif,weight;
  int accuracy, value,enchanted,failed;
   string crr;

  failed = caster->query_stat("intellect");


  crr = creator(ob);


  if(crr == "town"
  || crr == "mudlib"
  || crr == "secure"
  || crr == "MUDLIB"
  || crr == "NONAME"
  || crr == "inherit")
  crr = "ash";
  if(crr) crr = capitalize(crr);
  crr += " Crimsonesti";
  enchanted = (int)ob->query_enchant();
  ac = (int)ob->query_ac() -enchanted;
  wc = (int)ob->query_wc() -enchanted;
  if(ac < 0) ac = 0;
  if(wc < 0) wc = 0;
  value = (int)ob->query_value();
  write("\n");
  if(living(ob)) 
     {
      write("You place your hands on "+ ob->query_name() 
           +" and attempt to identify "+ ob->query_objective() +".\n");
      say(this_player()->query_name() +" touches "+ ob->query_name()
         +" and seems enlightened.\n");
      mon_level = (int)ob->query_level();
       p_level = (int)this_player()->query_level();
       level_dif = mon_level - p_level;
       write("Living being of level "+mon_level+"\n");
     }
else {
      if(ac)
      write("This is a class  "+ac+" armour.\n");
      if(wc)
      write("This is a class "+wc+" weapon.\n");
      if(enchanted)
      write("This has also been enchanted for "+enchanted+" points.\n");
      if(ob->query_bless())
      write("This item has been blessed.\n");
      if(value)
      write("This object has a value of "+value+" with most shops.\n");  
   ob->query_bonus(1);
   if(ob->query_wc() && !ob->query_ac() || !ob->query_wc() &&ob->query_ac())
   {
   write("This item is type "+ob->query_type()+".\n");
   if(ob->query_length())
   write("This item is "+ob->query_length()+" in length.\n");
   }
   if(ob->query_no_break())
    write("This has special protection against breakage.\n");
   if(ob->query_weight())
   write("This item has a weight of "+ob->query_weight()+".\n");
   write("\n");
  }
  destruct(this_object());
  return 1;
}

  query_no_save(){ return 1; }

