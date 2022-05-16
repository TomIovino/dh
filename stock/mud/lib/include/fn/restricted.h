
static string how;    // function to check 
static string ok;     // return this on "how" to be ok

int set_restricted(string x, string y){
     if(how == "race" || how == "alignment" || how == "remort")
        how = x;
         ok = y;
    return 1;
    }

int query_restricted(){
      // 0 = OK
      // 1 = NOT_OK
      if(!how) return 0;
      if(!ok) return 0;
      if(how == "race")
         if(this_player()->query_race() != ok) return 1;      
      if(how == "remort")
         if(!this_player()->query_remort(ok)) return 1;      
      if(how == "alignment")
          if(this_player()->query_alignment() != ok) return 1;      
      // unknown restrictions..
      return 0;
}

string query_restriction_string(){
       string s;
       if(!how) return "";
       if(!ok) return "";
       s = "This object is restricted to the "+how+" of "+ok+".\n";
    return s;
}

