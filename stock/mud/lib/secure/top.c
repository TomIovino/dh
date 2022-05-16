
#include <bar.h>
#include <mudlib.h>

#define SAVE_FILE "/ob_saves/rate/"
#define SAVE save_object(SAVE_FILE)
#define RESTORE restore_object(SAVE_FILE)

mapping members    = ([ ]);
mapping env;

void reset(int started);
status help();
status add_top(string org, string name,int rank);
status query_top(string org,string name);
status query_top_list(string org);
status query_title(string org,string name);
status remove_member(string org,string name);
status log_org(string org, string arg);    


int sort_by_value(string k1, string k2) {
    return (int)env[k1] < (int)env[k2];
}


int query_total_list(string org){  
    RESTORE;
    return sizeof(m_indices(members)); 
}

varargs mixed query_member(string org,string name){
    mixed ret;
    string title;
    members = ([]);
    RESTORE;
    if(!org) return 0;
    if(!name) return copy_mapping(members);
    ret = members[name];
    return ret;
}
varargs mixed query_title(string org,string name){
    mixed ret;
    string pretitle;
    members = ([]);
    RESTORE;
    if(!org) return 0;
    ret = members[name];
    pretitle = "";
    if(PLAYERD->query_gender(name) == 1)
    switch(ret){
     case -1: pretitle = "outcast"; break;
     case  1: pretitle = "nobleman"; break;
     case  2: pretitle = "lord"; break;
     case  3: pretitle = "lord"; break;
     case  4: pretitle = "highlord"; break;
     case  5: pretitle = "baron"; break;
     case  6: pretitle = "duke"; break;
     case  7: pretitle = "count"; break;
     case  8: pretitle = "prince"; break;
     case  9: pretitle = "king"; break;
    }
    if(PLAYERD->query_gender(name) == 2)
    switch(ret){
     case -1: pretitle = "outcast"; break;
     case  1: pretitle = "noblewoman"; break;
     case  2: pretitle = "lady"; break;
     case  3: pretitle = "lady"; break;
     case  4: pretitle = "highlady"; break;
     case  5: pretitle = "baroness"; break;
     case  6: pretitle = "duchess"; break;
     case  7: pretitle = "countess"; break;
     case  8: pretitle = "princess"; break;
     case  9: pretitle = "queen"; break;
    }
    return pretitle;
    SAVE;
}


status set_member(string org, string name, int rank){
    members = ([]);
    RESTORE;
    members[name] = (int)rank;
    SAVE;
    return rank;
}
status remove_member(string org, string name) {
    members = ([]);
    RESTORE;
    members = m_delete(members,name);
    SAVE;
    return 1;
}

status log_org(string org, string arg){
       log_file("REALMS",this_player()->query_name()+
       "||"+org+"||"+arg+"||"+ctime()+"\n");
       }   

status main(string arg){
    string org, order, pretitle,name;
    string *idx;
    int rank;
    int i,j,count;
    if(!arg){return help();}
       if (sscanf(arg, "%s %s %s %d", org, order, name, rank) != 4){
       if (sscanf(arg, "%s %s %s", org, order,name ) != 3)
       if (sscanf(arg, "%s %s", org, order) != 2)
       if (sscanf(arg, "%s", org) != 1)
       return help();
    }
    members = ([]);
    RESTORE;
 if(org != "good" && org != "main" &&  org != "evil" 
    && org != "neutral" && org != "divinity"){
     write("Invalid Organization.\n");
     return 1;
   }
  if(order)
    log_org(org, arg);
    switch(order) {
       case "add": 
       if(query_member(org, this_player()->query_real_name()) < 4)
          if(!this_player()->query_security_level()){
            write("You dont have access to that command in "+
            "this organization.\n");
            return 1;
            }  
       if(query_total_members(org) > 29){
            write("This organization is limited to 30 members.\n");
            return 1;
            } 
       if(rank+1 > query_member(org,this_player()->query_real_name()) &&
            !this_player()->query_security_level()){
            write("Your security is too low to do that.\n");
            return 1;
            }
            write("You have added "+name+" to the realm of "+org+".\n");
            set_member(org,name,1);
            return 1;
            break;
      case "demote": 
      case "promote": 
       if(!query_member(org, this_player()->query_real_name()) &&
          !this_player()->query_security_level()){
       write("You dont have access to that command in "+
            "this organization.\n");
            return 1;
            }
       if(!rank || rank > 9 || rank < -1){
            write("Thats not a valid rank it must be -1 through 8.\n");
            return 1;
            }
       if(!query_member(org,name)){
            write("For new members use: realm <realm> add <name>.\n");
            return 1;
            }
       if(query_member(org,name)+1 > 
            query_member(org,this_player()->query_real_name()) &&
            !this_player()->query_security_level()){
            write("You cant demote them.\n");
            return 1;
            }
 
       if(rank < 0)
          if(8 > query_member(org,this_player()->query_real_name()) &&
            !this_player()->query_security_level()){
            write("Only level 9 nobles can outcast!\n");
            return 1;
            }

       if(rank+1 > query_member(org,this_player()->query_real_name()) &&
            !this_player()->query_security_level()){
            write("Your security is too low to do that.\n");
            return 1;
            }
            write("You have (pro-de)moted "+name+" to noble rank "+rank+".\n");
            set_member(org,name,rank);
            return 1;
            break;
       case "remove": 
       if(!query_member(org, this_player()->query_real_name()) &&
          !this_player()->query_security_level()){
       write("You dont have access to that command in "+
            "this organization.\n");
            return 1;
            }
       if(query_member(org, this_player()->query_real_name()) < 8)
          if(!this_player()->query_security_level()){
            write("You dont have access to that command in "+
            "this organization.\n");
            return 1;
            }

            remove_member(org,name); 
            return 1;
            break;
    default:  
     printf("%s\n\t\t          * The Realm of %s *\n",
     PRINTBAR,capitalize(org));
     env = (mapping) query_member(org);
     if(!env) return printf("No members!\n"),1;
     j = sizeof( idx = sort_array(m_indices(env),#'sort_by_value));
     count = 0;
   for(i = 0; i < j; i++){
     name = idx[i];
     pretitle = PLAYERD->query_pretitle(idx[i]);
     printf(B_WHITE_F "  %-18s" GREY_F "%-19s%s" ,
     capitalize(name),capitalize(pretitle),count% 2 ? "\n" : "  ");
      count ++;
    }
    printf("\n%s\n",PRINTBAR);
    return 1;
    }
    help();
}
 

status help(){
         write("See also 'help realms'.\n");
         write("See also 'help realm ranks'.\n");
         write("See also 'help realm cmds'.\n");
         write("See also 'help theme'.\n"); 
         return 1;
}
