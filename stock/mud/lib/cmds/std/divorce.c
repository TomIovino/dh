#include <ansi.h>
#include <mudlib.h>

int main(string what);
int help();
int result();


int main(string what){
    object wouldbe;
    string wbname,myname;
    if(!what){ 
     write("divorce who?\n");
     return 1;
    }
    wouldbe = find_player(what);

/*
    if(this_player()->query_real_name() == "nilgo" ||
       this_player()->query_real_name() == "hydrix"){
         write("Not granted.\n");
         return 1;
    }
*/
    if(!wouldbe){
     write("That person cannot be located.\n");
     return 1;
    }
    if(wouldbe->query_invis()){
     write("That person cannot be located.\n");
     return 1;
    }
    myname = this_player()->query_real_name();
    wbname = wouldbe->query_real_name();
    if(myname == wbname){ 
      write("You're stuck with yourself.\n");
      return 1;
    }

    if(this_player()->query_spouse() != wbname){
      write("You are not married to each other.\n");
      return 1;
    } 
    if(myname != wouldbe->query_spouse()){
      write("You are not married to each other.\n");
      return 1;
    } 

      tell_object(wouldbe,"You are no longer married.\n");
      write("You are no longer married.\n");
      this_player()->set_spouse("remove");
      wouldbe->set_spouse("remove");
  
  return 1;
}


