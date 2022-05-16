// Ash

string *query_cmd_aliases() { return ({ "compare",  }); }

#include <valid.cfg>
#include <ansi.h>
#include <mudlib.h>
#include <bar.h>

#define TP this_player()

status main(string arg) {
      object what1;
      string str;
      int x;

    if(!arg){ write("lore <what>\n"); return 1; }
    if(!(what1=present(arg,this_player()))) {
        write("There is no " + arg + " here.\n");
        return 1;
    }
      x = what1->query_ac()+what1->query_wc()+what1->query_bonus();
if(what1->query_type() == "shield" || 
   what1->query_type() == "armour" ||        
   what1->query_wc()) 
       x = (x/2+1);
      if(x > 15)
        str =(B_GREEN_F"*GODLY");
      if(x > 14)
        str =(B_GREEN_F"*DIVINE*");
      else if(x > 12)
        str =(GREEN_F"superb");
      else if(x > 10)
        str =(GREEN_F"amazing");
      else if(x > 8)
        str =(B_MAGENTA_F"incredible");
      else if(x > 6)
        str =(MAGENTA_F"excellent");
      else if(x > 5)
        str =(B_BLUE_F"great");
      else if(x > 4)
        str =(CYAN_F"good");
      else if(x > 3)
        str =(RED_F"average");
      else if(x > 2)
        str =(BROWN_F"decent");
      else if(x > 1)
        str =(YELLOW_F"poor");
      else
       str =(WHITE_F"bad");
        str = NORM+str+NORM;

   printf("    %s\n","You look over the obvious characteristics of");
   printf("    %s\n",what1->query_short());
   printf("    %s", "You decide the item basically rates as : "+str+".\n");
  return 1;
}

status help(){
    write("This command tells you aproximate usefullness of an item.\n");
    write("Command: lore <item in inventory>\n");
    return 1;
}

