#include <ansi.h>
#include <mudlib.h>

int main(string what);
int help();
int result();


string do_msg(string say){
    catch("/secure/channeld/channeld"->
    tell_channel("info",say,":"));
    return say;
}


int main(string what){
    object wouldbe;
    string wbname,myname;
    wbname = myname = 0;
    if(!what){ 
     write("marry who?\n");
     return 1;
    }
    wouldbe = find_player(what);

    if(!wouldbe){
     write("That person cannot be located.\n");
     return 1;
    }
    if(wouldbe->query_invis()){
     write("That person cannot be located.\n");
     return 1;
    }
    if(wouldbe == this_player()){
     write("hmm?\n");
     return 1;
    }

    myname = this_player()->query_real_name();
    wbname = wouldbe->query_real_name();

    if(!this_player()->query_days_old() ||
       !wouldbe->query_days_old()){
       write("You're not old enough kid!\n");
       return 1;
    }

    if(this_player()->query_spouse() != 0 && 
      this_player()->query_spouse() != ""){
      write("You are already married.\n");
      return 1;
    } 
    if(wouldbe->query_spouse() != 0 && 
       wouldbe->query_spouse() != ""){
      write("They are already married.\n");
      return 1;
    } 
    if(this_player()->query_gender() == wouldbe->query_gender()){
       write("Interesting, but not in theme.\n");
       return 1;
    }
   if(!this_player()->query_attrib("proposed"))
    tell_object(wouldbe,myname+
        " has proposed marriage to you to accept type: marry "+myname+".\n");
    tell_object(this_player(),
       "You proposed marriage to "+wouldbe->query_name()+"\n");
 
    wouldbe->set_tmp_attrib("proposed",myname);

    if( this_player()->query_attrib("proposed") == wbname){
      write("Congratulations, You are now married.\n");
      tell_object(wouldbe,"Congratulations, You are now married.\n");
      this_player()->set_spouse(wbname);
      wouldbe->set_spouse(myname);
      do_msg(this_player()->query_who()+ " has joined with "+
              wouldbe->query_who()+" in marriage.");
       wouldbe->set_tmp_attrib("proposed",0);
       this_player()->set_tmp_attrib("proposed",0);
    }
  
  return 1;
}


