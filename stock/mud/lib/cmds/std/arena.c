object winner;

int main(string arg) {
    string chan,nom,msg,x,y;
    object *u;
    int i,c;
    u = users();
    i = sizeof(u);
    c = 0;
    write("\nCurrent Arena Contestants:\n");
    while(i--) {
      if(sscanf(u[i]->query_current_room(), "%s/arena/%s", x,y) == 2){
       c++;
       printf("[%|15s] (%15s %s)\n",
          u[i]->query_house(),
          u[i]->query_alt_name(),
          u[i]->query_condition());
       winner = u[i];
      }  
    }
   write("\nTotal Arena Contestants: "+c+"\n");
   if(c == 0){ return 1000; }
   if(c == 1){
   while(c--){   
    winner->death();
    winner->adj_hp(1000);
    winner->look();
    chan = "info";
    nom =  winner->query_cap_name();
    msg =  ":is the arena champion!";
    catch("/secure/channeld/channeld"->
    tell_channel(chan,nom,msg));
   }
  }
   return c;
}


query_alt_name(){ return "Oldage"; }
