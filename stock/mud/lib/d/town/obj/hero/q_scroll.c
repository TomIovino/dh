#include <ansi.h>
#include <mudlib.h>
inherit TREASURE;
#define TP this_player()
#define QN query_name()

int si, tu, gr, sy, am;

reset (status arg){
    if (arg) return;
    set_name("scroll");
    set_alias("q_scroll");
    set_short(B_RED_F"rune etched scroll"+NORM);
    set_long("This scroll is made of the finest paper.\n"+
             "It also bears obvious magical enhancement.\n"+
             "It has some writing on it.\n");
    set_weight(1);
    set_no_save(1);
    set_value(0);
    set_sell_destruct(1);
}

  int drop(){ write("It burns into ashes.\n"); rto(); return 1; }

  init(){
     ::init();
     add_action("read","read");
     add_action("hand","hand");
}

string read(string what){
   if(what != "scroll") return "";
   write("Most of the words you cannot comprehend.\n");
   write("Take this scroll to the following people:\n");
   write("Silanios, Tuji, Amber, Grog, Sylv.\n");
   write("'hand <name>' to all these people to read and sign.\n");
   write("When you are done return to the hero and bow.\n");
  if(si)
   write("Silanios has signed.\n");
  if(tu)
   write("Tuji has signed.\n");
  if(am)
   write("Amber has signed\n");
  if(gr)
   write("Grog has signed.\n");
  if(sy)
   write("Sylv has signed.\n");
return "";
}

string hand(string str){
          object who;
      if(!str){ write("hand <who>\n"); return ""; }
      if(!(who=present(str,environment(TP)))) {
        write("There is no " + str + " here.\n");
        return "";
      }
       who = present(str, environment(TP));
   
   if(who->query_name() == "Silanios"){
      write("You hand the scroll to "+who->query_name()+"\n");
      write("Silanios looks over the scroll carefully.\n");
      write("Silanios cackles, signs the scroll and hands it to you.\n");
      si = 1;  
      return "";
   }

   if(who->query_name() == "Tuji"){
      write("You hand the scroll to "+who->query_name()+"\n");
      write("Tuji looks over the scroll carefully.\n");
      write("Tuji grins, signs the scroll and hands it to you.\n");
      tu = 1;
      return "";
   }
   if(who->query_name() == "Amber"){
      write("You hand the scroll to "+who->query_name()+"\n");
      write("Amber looks over the scroll carefully.\n");
      write("Amber signs the scroll and hands it to you.\n");
      am = 1;
      return "";
   }
   if(who->query_name() == "Sylv"){
      write("You hand the scroll to "+who->query_name()+"\n");
      write("Sylv looks over the scroll carefully.\n");
      write("Sylv smiles, signs the scroll and hands it to you.\n");
      sy = 1;
      return "";
   }
   if(who->query_name() == "Grog"){
      write("You hand the scroll to "+who->query_name()+"\n");
      write("Grog puts an X on the scroll and gives it to you.\n");
      gr = 1;
      return "";
   }
  
  write("That is not one of the five.\n");
  return "";
}

  
int complete(){ if(si && gr && am && sy && tu) return 1; return 0; }
