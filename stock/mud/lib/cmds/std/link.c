#include <ansi.h>
#include <mudlib.h>

int main(string what);
int help();
int result();

#define SOULD "/secure/soul/emote_d"

int main(string what){
    object wouldbe;
    string wbname,myname;
    object colink;
    string tpqn;
    wbname = myname = 0;
    tpqn = this_player()->query_alt_name();
 /*****************************************************************/
 if(this_player()->query_link() != "" && this_player()->query_link() != 0)
  if(find_player(this_player()->query_link()))
   colink = find_player(this_player()->query_link());
  if(this_player()->query_link() != ""){
      if(what == "remove" || what == "off" || !colink){
                   tell_object(this_player(), 
                   "[ link removed ]\n");
                   this_player()->set_link("");
            if(colink){
                   tell_object(colink, 
                   "[ link removed ]\n");
                   colink->set_link("");
            }
            return 1;
      }

 /*****************************************************************/
    if (what == "-h"){
       if(this_player()->query_attrib("LH3"))
        write("[3]"+this_player()->query_attrib("LH3")+"\n");
       if(this_player()->query_attrib("LH2"))
        write("[2]"+this_player()->query_attrib("LH2")+"\n");
       if(this_player()->query_attrib("LH1"))
        write("[1]"+this_player()->query_attrib("LH1")+"\n");
       return 1;
    } 
 /*****************************************************************/

   if (what[0] == ';') {
      catch(what = (string)SOULD->get_feeling(what[1..],"x"));
      if(what)
       what = sprintf(tpqn+" %s", what);
        else
       return 0;
      }
    else if (what[0] == '*') {
      catch(what = (string)SOULD->get_feeling(what[1..],"x"));
      if(what)
       what = sprintf(tpqn+" %s", what);
      }
     else what = sprintf(tpqn+": "+what+" ");
  
 /*****************************************************************/
    colink->set_tmp_attrib("LH3",colink->query_attrib("LH2"));
    colink->set_tmp_attrib("LH2",colink->query_attrib("LH1"));
    colink->set_tmp_attrib("LH1",what);
     what = this_player()->do_cc("["+what+"]\n")+NORM;
     tell_object(this_player(),what); 
     tell_object(colink,what);

     return 1;
  }
 /*****************************************************************/
    if(!what){ 
     write("link who?\n");
     return 1;
    }
    wouldbe = find_player(what);

    if(!wouldbe){
     write("That person cannot be located.\n");
     return 1;
    }
   if(!this_player()->query_security_level())
    if(wouldbe->query_invis()){
     write("That person cannot be located.\n");
     return 1;
    }
    myname = this_player()->query_real_name();
    wbname = wouldbe->query_real_name();
     if(this_player()->query_link() != 0 && 
      this_player()->query_link() != ""){
      write("Your are already linked.\n");
      return 1;
    } 
    if(wouldbe->query_link() != 0 && 
       wouldbe->query_link() != ""){
      write("They are already linked.\n");
      return 1;
    } 
    tell_object(wouldbe,myname+
        " has requested a link to you to accept type: link "+myname+".\n");
    tell_object(this_player(),
       "You requested a link to "+wouldbe->query_name()+"\n");
 
    wouldbe->set_tmp_attrib("wannalink",myname);

    if( this_player()->query_attrib("wannalink") == wbname){
      write("You are now linked.\n");
      tell_object(wouldbe,"You are now linked.\n");
      this_player()->set_link(wbname);
      wouldbe->set_link(myname);
      wouldbe->set_tmp_attrib("wannalink",0);
      this_player()->set_tmp_attrib("wannalink",0);
    }
  
       return 1;
}


