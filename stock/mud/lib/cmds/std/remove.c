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
  if(str != "all")
   {
    call_other(item,"remove",str);
    call_other(item,"unwield",str);
    return 1;
   }
  if(str == "all")
   {
  invent = all_inventory(this_player());
  for(i=0; i<sizeof(invent); i++) 
  {
    if(invent[i]->query_wielded())
      command("remove "+(string)invent[i]->query_name(), this_player());
    if(invent[i]->query_worn())
      command("remove "+(string)invent[i]->query_name(), this_player());
   }
  return 1;
 }
} 


