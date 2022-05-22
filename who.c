#include <mudlib.h>
#include <ansi.h>
#include <bar.h>

string unique_fun;
int count;

int s_by_int(mixed *a, mixed *b);
int s_by_string(mixed *a, mixed *b);

int main(string arg) {
  closure s_by;
  mixed *arr;
  string x;
  int i,j;
  
  count = 0;
  
  switch(arg){
    case "-w":
    case "-wp":
    case "-l":
    case "-long":
      unique_fun = "query_wp";
      s_by = #'s_by_int;
      break;
    case "-a":
    case "-alignment":
      unique_fun = "query_alignment";
      s_by = #'s_by_string;
      break; 
    default:
      unique_fun = "query_wp";
      s_by = #'s_by_int;
      break;
  }     
  
  write(NORM+RED_F"\n===========================================\n\n"); 
  i = sizeof(arr = unique_array(users(),unique_fun,-1));
  arr = sort_array(arr,s_by,unique_fun);  
  
  while(i--) {
    j = sizeof(arr[i]);
    while(j--)
      if(arr[i][j]->query_current_room() != "NONE" && !arr[i][j]->query_security())
      if(arr[i][j]->short()){
        x="";
        
        if(arr[i][j]->query_last_pk()+86400 > time())
          x=sprintf("%s[%sPK%s]",B_RED_F,NORM+RED_F,B_RED_F); 
               
        printf("%s %s%s\n",arr[i][j]->short(),x,NORM);
      }                                
  }
  
  i = sizeof(arr = unique_array(users(),unique_fun,-1));
  arr = sort_array(arr,s_by,unique_fun);  
  
  while(i--) {
    j = sizeof(arr[i]);
    while(j--)
      if(arr[i][j]->query_current_room() != "NONE" && arr[i][j]->query_security())
      if(arr[i][j]->short()){ 
        printf("%s%s\n",arr[i][j]->short(),NORM);
      }                                
  }
  
  write(NORM+"\n"RED_F"==========================================="+YELLOW_F+
        "\n High Count Since Reboot: "+"/cmds/wiz/mudstat"->player_count(1)
  +NORM+RED_F+"\n===========================================\n"NORM);
  return 1;
}

int s_by_int(mixed *a, mixed *b) { 
  return a && b && a[0] && b[0] && (int) call_other(a[0],unique_fun) < (int)call_other(b[0],unique_fun); 
}

int s_by_string(mixed *a, mixed *b){ 
  return a && b && a[0] && b[0] && (int) call_other(a[0],unique_fun) < (int) call_other(b[0],unique_fun); 
}