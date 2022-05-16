#include <ansi.h>

string chan,nom,msg;
int time,bonus;

main (int arg){
    int x,y;
     if(!arg){ 
         write("exex bonus time\n");
         write("exex 100 100     (double exp, 100 mins)\n");
         return 1; 
    }
    if(sscanf(arg, "%d %d", bonus,time) != 2) {   
         write("exex bonus time\n");
         write("exex 100 100     (double exp, 100 mins)\n");
         return 1; 
    }
    chan = "info";
    msg = ":";
    nom = YELLOW_F"Gods have granted "+bonus+
          " percent bonus experience for "+
          time+" minutes."NORM;
      time = time*60;
    catch("/secure/channeld/channeld"->
    tell_channel(chan,nom,msg));
    remove_call_out("end_bonus");
    call_out("end_bonus",time);
    return 1;
}
query_bonus(){
   return bonus;
}
end_bonus(){
    chan = "info";
    nom = YELLOW_F"Bonus experience has ended."NORM;
    catch("/secure/channeld/channeld"->
    tell_channel(chan,nom,msg));
 bonus = 0; 
}



