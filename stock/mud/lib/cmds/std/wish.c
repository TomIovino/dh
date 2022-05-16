#include <bar.h>
#include <ansi.h>
#include <mudlib.h>
#define TP this_player()

int check_exp(string x){
  int AMT;
  
  if(x == "bonus") AMT =   2000000;
  if(x == "stat")  AMT =  50000000;
  
  if(this_player()->query_exp() < AMT){
    write("You cannot seem to create the energy");
    return 0;
  }  
  
  return 1;
}

int check_money(string x){
  int AMT;
  
  if(x == "bonus") AMT =     0;
  if(x == "stat")  AMT = 10000;
  
  if(this_player()->query_money() < AMT){
    write("You cannot seem to create the energy");
    return 0;
  }  
  
  return 1;
}

int main(string what){
  string stat;

  string x, y;

  if(!what){
    write("\n"+PRINTBAR+"\n");
    this_player()->check_extra_stats();
    write("\nBonus Stat Points ");
    write(NORM+"\n");
    write("\n"+PRINTBAR+"\n");
    return 1;
  }

  if(TP->query_level() != 100){
    write("You're not level 100, too bad, so sad.\n");
    return 1;
  }
 
  if(sscanf(what, "%s", stat) != 1) {
    write("You must pick a valid stat.\n");
    return 1;
  }

  switch(stat){
    case "strength": "stamina": "agility": "dexterity":
          "focus": "intellect": "wisdom": "primal":
            what = "stat";
            break;
    case "hp": "sp": "mp":
            what = "bonus";
            break;
  }
  
  if(!what){
    write("That is not a valid stat.\n");
    return 1;
  }
  
  if(stat == "hp" || stat == "sp" || stat == "mp"){
  
  }

  if(this_player()->query_extra_stat(stat) > 10){
     write("You can only wish stats up to ten.\n");
   return 1;
  }
    if(!this_player()->query_security())
     write_file("/log/MAIN/WISH",
     TP->query_name()+" ("+TP->query_level()+
     ") WISHED for "+stat+"("+ctime()+") *\n");
     TP->adj_extra_stat(stat,1);
     TP->adj_exp(-100000000);
     TP->adj_money(-1000000);
     write("OK!");
     return 1;
}