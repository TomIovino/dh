// ash (duh)

main(str) {
  int i;
  object *invent;
  object item;
   if(!str)
   { 
    return 0; 
   }
  if(str != "all")
  if(!item = present(str, this_player()) ) 
   {
    printf("You dont have that.\n");
    return 1;
   }
  if(str != "all"){
  if(item->query_owner() != "")
   if(item->query_owner() != 
     this_player()->query_real_name()){
     return 1;
     }

  call_other(item,"wear",str);
    return 1;
   }

  if(str == "all")
   {
  invent = all_inventory(this_player());
    for(i=0; i<sizeof(invent); i++)
    {
    if(invent[i]->query_wc() && !invent[i]->query_wielded()) 
      command("wield "+(string)invent[i]->query_name(), this_player());
    else if(invent[i]->query_ac() && !invent[i]->query_worn())
      command("wear "+(string)invent[i]->query_name(), this_player());
    }
  return 1;
 }
} 


help(){
   write("wear <what>\n");
   return 1;
}

