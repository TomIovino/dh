#include <ansi.h>
#include <mudlib.h>
#define TPQN this_player()->query_alt_name()

#define QTT this_player()->query_attrib("QTT")
#define STT this_player()->set_tmp_attrib("QTT",time()+2)
#define FTT this_player()->set_tmp_attrib("QTT",time()+1)
#define XXX file_name(environment(this_player()))
#define QLT this_player()->query_attrib("XXX")
#define SLT this_player()->set_tmp_attrib("XXX",XXX)

int main(string what);
int second(object who, string what);
int help();
int result();
   
int main(string what){
    STT;
    SLT;
    if(!what) what = query_verb();
    call_out("second",1,this_player(),what);
    write("You call on your primal rage to flurry.\n");
    return 1;
}
int second(object me, string what){
    string poss;
    object who;
    if(QLT != XXX){
      write("Stop running first.\n");
      return 1;
    }
    if(QTT >  time()){
      write("Unable to call your primal rage to flurry!\n");
      return 1;
    }
    FTT;
    if((status)environment(me)->query_no_fight() == 1) {
       write("No.\n");
       return 1;
    }
    if(me->query_var("slow")){
        write("Your too SLOW to flurry.\n");
        return 1; 
    }
    if(me->query_tactic() != "offensive"){
        write("Must use offensive tactics.\n");
        return 1;
    }
    if(!me->query_skill("flurry")){
        write("You dont seem to have that skill.\n");
        return 1;
     }
    if(me->query_mp() < 100){
        write("Not enough movement points!\n");
        return 1; 
     }
     write("You flurry your opponent!\n");
     me->checkskill("flurry","primal",10);         
     me->adj_mp(-100);
  if(!random(1))
     me->attack();
  if(me->query_var("frenzy") && !random(2) )
     me->attack();
  if(me->query_skill("flurry") > 35  && !random(4) )
     me->attack();
  if(me->query_skill("flurry") > 70  && !random(5) )
     me->attack();
      return 1;
}

