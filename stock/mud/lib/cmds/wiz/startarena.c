#include "ansi.h"

string *query_cmd_aliases() { return ({ "sa", }); }

int msg(string inmsg){
    inmsg = CYAN_F+inmsg+NORM;
    catch("/secure/channeld/channeld"->
    tell_channel("info",inmsg,":"));
}


int do_it() {
    object *u;
    int i;
    u = users();
    i = sizeof(u);
    while(i--) {
      if(u[i]->query_env_var("ARENA")){
         move_object(u[i],"/d/arena/main");   
         u[i]->adj_hp(1000);
         u[i]->adj_sp(1000);
         u[i]->adj_mp(1000);
         u[i]->unset_env_var("ARENA");
         command("look",u[i]); 
       }  
    }
    msg("Grand Arena Battle has started!");
    return 1;
}
int one(string arg){
    call_out("do_it",60);
    msg("Grand Arena in one minute Type 'config arena' to join!");
    return 1;    
}
int five(string arg){
    call_out("one",240);
    msg("Grand Arena in five minutes Type 'config arena' to join!");
    return 1;
}
int ten(string arg){
    call_out("five",300);
    msg("Grand Arena in ten minutes Type 'config arena' to join!");
    return 1;
}

int twenty(string arg){
    call_out("ten",600);
    msg("Grand Arena in twenty minutes Type 'config arena' to join!");
    return 1;
}

int sixty(string arg){
    call_out("ten",2400);
    msg("Grand Arena in sixty minutes Type 'config arena' to join!");
    return 1;
}


int main(string arg){
  if(arg != "one" && arg != "five" 
 &&  arg != "ten" 
 &&  arg != "twenty" 
 &&  arg != "sixty"){
     write("sa <one/five/ten/twenty/sixty>\n");
    return 1;
    }
 call_out(arg,1);
return 1;
    
}

