

int do_obj_log(){
   string type;
   int check;
   int doit;
   int blah;
   type =  this_object()->query_type();
   blah =  this_object()->query_bonus();
   blah += this_object()->query_wc(); 
   blah += this_object()->query_ac();
 if(this_object()->query_wc() && blah >= 25) 
     doit = 1;
 if(this_object()->query_ac() && blah >= 25) 
   if(type == "armour" || type == "shield")
     doit = 2;
 if(this_object()->query_ac() && blah >= 15) 
   if(type != "armour" && type != "shield") 
     doit = 3;
 if(!doit) return 0;
 
  log_file("TOKEN/"+type, 
    this_player()->query_name()+" : "+this_object()->query_name()+
    ":"+blah+":"+file_name(this_object())+" : "+time()+"\n");

 if(doit == 1){
    log_file("TOKEN/WEAPON", 
    this_player()->query_name()+" : "+this_object()->query_name()+
    ":"+blah+":"+file_name(this_object())+" : "+time()+"\n");
   return 1;
  }
 if(doit == 2){
    log_file("TOKEN/PRIMARY", 
    this_player()->query_name()+" : "+this_object()->query_name()+
    ":"+blah+":"+file_name(this_object())+" : "+time()+"\n");
   return 1;
  }
 if(doit == 3){
    log_file("TOKEN/SECONDARY", 
    this_player()->query_name()+" : "+this_object()->query_name()+
    ":"+blah+":"+file_name(this_object())+" : "+time()+"\n");
   return 1;
  }

return 0;
}

