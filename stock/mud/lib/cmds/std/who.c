// Ash (duh)


#include <ansi.h>
#include <mudlib.h>
#include <bar.h>

string unique_fun;
int count;

int s_by_int(mixed *a, mixed *b);
int s_by_string(mixed *a, mixed *b);
int s_by_obj_string(mixed *a, mixed *b);

int pts(object o){ return "cmds/std/stats"->total_score(o); }

void chf(){
    if(this_player()->query_attrib("helper") &&
       this_player()->query_level() < 50)
    { this_player()->unset_attrib("helper"); 
    }
}
string wpc(int wp)
   { if(!wp) return "*";
     return ""+wp;
   }

int main(string arg) {
    closure s_by;
    mixed *arr;
    int acount,ts1;
    string x;
    int i, j;
    count = 0;
    chf();
    if(!arg || !stringp(arg)) 
        arg = "-s";
    switch(arg) {
        case "-f":
            unique_fun = "query_funds"; 
            s_by = #'s_by_int;
            break;
        case "-w":
        case "-wp":
        case "-l":
        case "-long":
            unique_fun = "query_wp"; 
            s_by = #'s_by_int;
            break;
        case "-h":
        case "-house":
            unique_fun = "query_house";
            s_by = #'s_by_string;
            break;
        default:
            unique_fun = "query_name";
            s_by = #'s_by_string;
            break;
    }
    
    i = sizeof(arr = unique_array(users(),unique_fun,-1));
   // i -= filter_array(i,#'query_environment);
    arr = sort_array(arr,s_by,unique_fun);
// START PROCESS
    write(PRINTBAR"\n");
    acount = 0;
     if(unique_fun != "query_name" 
     && unique_fun != "query_house" 
     && unique_fun != "query_wp" 
     && unique_fun != "query_funds" 
     && unique_fun != "query_rating")
     while(i--) {
        j = sizeof(arr[i]);
        while(j--)
             if(arr[i][j]->query_current_room() != "NONE")
             if(arr[i][j]->short()){
             printf("%s%s\n",
             (string)arr[i][j]->do_cc(""),
         //    to_string((mixed)call_other(arr[i][0],unique_fun)),
             (string)arr[i][j]->query_who());
             acount += arr[i][j]->query_age();
            }
   }
   if(unique_fun == "query_house")
     while(i--) {
        j = sizeof(arr[i]);
        while(j--)
            if(arr[i][j]->query_current_room() != "NONE")
            if(arr[i][j]->short()){
            printf("%s(%|15s)[%3s] %s%s\n",
               arr[i][j]->do_cc(""),
               arr[i][j]->query_house(),
               wpc(arr[i][j]->query_wp()),
               arr[i][j]->query_who(),
               NORM);
               acount += arr[i][j]->query_age();
            }
                                      
   }
   if(unique_fun == "query_wp")
     while(i--) {
        j = sizeof(arr[i]);
        while(j--)
            if(arr[i][j]->query_current_room() != "NONE")
            if(arr[i][j]->short()){
            printf("%s[%3s](%|15s) %s",
               arr[i][j]->do_cc(""),
               wpc(arr[i][j]->query_wp()),
               arr[i][j]->query_house(),
               arr[i][j]->query_who());
            if(arr[i][j]->query_env_var("ROLEPLAY") ||
               arr[i][j]->query_wp() > 5)
               printf("%s the %s%s.\n",
               arr[i][j]->do_cc(""),
              (string)capitalize(""+arr[i][j]->query_class()),NORM);
            else 
               printf("%s\n",NORM);
               acount += arr[i][j]->query_age();
            }
                                      
   }
// WIZ TOOL
   if(this_player()->query_security_level())
   if(unique_fun == "query_funds")
     while(i--) {
        j = sizeof(arr[i]);
        while(j--)
            if(arr[i][j]->query_current_room() != "NONE")
            if(arr[i][j]->short()){
            printf("%10s M%d/S%d\n",
               arr[i][j]->query_name(),
               arr[i][j]->query_money(),
               arr[i][j]->query_savings());
               acount += arr[i][j]->query_age();
            }
                                      
   }
// STD WHO
   if(unique_fun == "query_name")
    while(i--) {
        j = sizeof(arr[i]);
        while(j--)
 // use IF to overwrite - structured from less, to most in importants
 // of what should be shown on who if multiple cases exist
            if(arr[i][j]->query_current_room() != "NONE")
            if(arr[i][j]->short()){
            count++;
            if(!arr[i][j]->query_wp()){
            x =  sprintf("%s        %s[%s*%s]%s",
                 GREEN_F,NORM+GREEN_F,B_GREEN_F,
                 NORM+GREEN_F,B_GREEN_F);
            } if(arr[i][j]->query_wp()){
            x =  sprintf("%s        %s[%s%s%s]%s",
                 BLUE_F,B_BLUE_F,NORM+BLUE_F,
                 wpc(arr[i][j]->query_wp()),
                 B_BLUE_F,B_BLUE_F);
            } if(arr[i][j]->query_attrib("helper")){
            x =  sprintf("%s    (%sNH%s)%s[%s%s%s]%s",
                 B_BLUE_F,B_CYAN_F,B_BLUE_F,CYAN_F,B_BLUE_F,
                 wpc(arr[i][j]->query_wp()),
                 B_CYAN_F,B_BLUE_F);
            } if(arr[i][j]->query_last_pk()+86400 > time()){
            x =  sprintf("%s    [%sPK%s]%s[%s%s%s]%s",
                 B_RED_F,NORM+RED_F,BOLD,NORM+RED_F,B_RED_F,
                 wpc(arr[i][j]->query_wp()),
                 NORM+RED_F,B_RED_F);
            } if(arr[i][j]->query_security_level()){
            x =  sprintf("%s    [%s~%sGOD%s~%s]%s",
                 B_RED_F,B_WHITE_F,YELLOW_F,
                 B_WHITE_F,B_RED_F,NORM+MAGENTA_F);
            } 
            printf("%9s %-11s  %s",
                x, (string)arr[i][j]->query_name(),
                count % 3 ? "" : "\n");
            acount += arr[i][j]->query_age();
 
        }
    }
// END PROCESS
    write(NORM+
    "\n"+PRINTBAR+YELLOW_F+
    "\n who <-w -h>                   \t\t     High Count Since Reboot: "+
    "/cmds/wiz/mudstat"->player_count(1)+
    NORM+"\n"+PRINTBAR+"\n");
    if(this_player()->query_security_level()){
          write(GREY_F"Users: "+sizeof(users())+
          "   total days : "+acount/43200+"   average days: "
          +(acount/43200)/sizeof(users())+"\n");
   }
   return 1;
}



int s_by_int(mixed *a, mixed *b) {
    return a && b && a[0] && b[0] && (int) call_other(a[0],unique_fun) <   
    (int)call_other(b[0],unique_fun);
}

int s_by_string(mixed *a, mixed *b) {
    return a && b && a[0] && b[0] && (int) call_other(a[0],unique_fun) < 
    (int) call_other(b[0],unique_fun);
}

int s_by_obj_string(mixed *a, mixed *b) {
    return a && b && a[0] && b[0] && file_name((object)
           call_other(a[0],unique_fun)) < 
           file_name((object)call_other(b[0],unique_fun));
}


        
