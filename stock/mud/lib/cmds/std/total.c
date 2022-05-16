// Ash (duh)

#include <bar.h>
#include <mudlib.h>
#include <ansi.h>
string *query_cmd_aliases() { return ({"ts","totalscore", }); }


string FIX(int what){
           if(what > 9999) return     ""+what;
           if(what >  999) return    " "+what;
           if(what >   99) return   "  "+what;
           if(what >    9) return  "   "+what;
           return                 "    "+what;
          }

#define LM NORM+MAGENTA_F+"("
#define RM NORM+MAGENTA_F+")"
#define LG NORM+GREEN_F+"("
#define RG NORM+GREEN_F+")"
#define NG NORM+GREEN_F
#define NM NORM+MAGENTA_F
#define BM B_MAGENTA_F
#define BR B_RED_F
#define R  NORM+RED_F
#define BG B_GREEN_F
#define TP find_player(who)



int total_score(string who){
 int x; 
 x = 500; // will be used to additional varibles 
 x += TP->query_days_old();
 x += TP->query_level();
 x += TP->query_ac();
 x += TP->query_wc();
 x += TP->query_total_skills();
 x += TP->query_hp();
 x += TP->query_sp();
 x += TP->query_mp();
 return x/10;
}

status main(string who) {

  if(!who || !this_player()->query_security_level()  || !find_player(who))
  who = this_player()->query_real_name();

write(PRINTBAR+"\n");

   write("Current total score is: "
    +total_score(who));


write("\n"PRINTBAR+"\n");

return 1;
  }

