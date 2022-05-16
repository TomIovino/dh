// Code and Implementation @Ash
// VERSION 2 CASTLE SUPPORT 

#include "/secure/bim/user_exists.c"

#include <bar.h>
#include <mudlib.h>
#define ORG "houses"
#define SAVE_FILE "/ob_saves/houses/"
#define SAVE save_object(SAVE_FILE+org)
#define RESTORE restore_object(SAVE_FILE+org)

mapping members    = ([ ]);
mapping ally       = ([ ]);
mapping env;
mapping keep;


void reset(int started);
status help();
status set_member(string org, string name,int rank);
status query_member(string org,string name);
status query_total_members(string org);
status query_title(string org,string name);
status query_rank(string org,string name);
status remove_member(string org,string name);
status log_org(string org, string arg);    


int sort_by_value(string k1, string k2) {
    return (int)env[k1] < (int)env[k2];
}

int query_total_members(string org){  
    members = ([]);
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
    string xtitle;
    members = ([]);
    RESTORE;
    if(!org) return 0;
    ret = members[name];
    xtitle = "";
    switch(ret){
     case  1: xtitle = "squire"; break;
     case  2: xtitle = "knight"; break;
     case  3: xtitle = "ambassador"; break;
     case  4: xtitle = "regent"; break;
     default: xtitle = "ally";
    }

    return xtitle;
    SAVE;
}

varargs mixed query_rank(string org,string name){
    int rank;
    members = ([]);
    RESTORE;
    rank = members[name];
    SAVE;
    return rank;
}


varargs mixed query_ally(string org,string name){
    ally = ([]);
    RESTORE;
    if(!org) return 0;
    if(!name) return copy_mapping(ally);
    return ally[name];
}
status set_member(string org, string name, int rank){
    members = ([]);
    RESTORE;
  if(!org || !name || !rank) return 0;
    if(query_ally(org,name)) return 0;
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
status set_ally(string org, string name, int rank){
    ally = ([]);
    RESTORE;
  if(!org || !name || !rank) return 0;
    if(query_member(org,name)) return 0;
    ally[name] = (int)rank;
    SAVE;
    return rank;
}
status remove_ally(string org, string name) {
    ally = ([]);
    RESTORE;
    ally = m_delete(ally,name);
    SAVE;
    return 1;
}

status log_org(string org, string arg){
       log_file("HOUSES",org+"--->"+arg+"\n");
       }   

status ally(string org){
        string name;
        name = this_player()->query_real_name();
#ifdef HARDASS
      if(this_player()->query_house() == "commoner")
      {
         write("Commoner's are not allowed to ally anyone.\n");
          return 1;
     }
#endif
        if(query_ally(org,name)){
        write("You are an ally of House "+capitalize(org)+".\n");
        write("Establishing communication..\n");
          this_player()->set_channel(org, 1);
          catch("/secure/channeld/channeld"->
          tell_channel(org,this_player()->query_alt_name(),
                 ":has turned on ally communication.\n"));
        return 1; 
        }
        write("No such alliance.\n");
        return 1;
}


status main(string arg){
    string org, order, pretitle,name;
    string *idx;
    int rank;
    int i,j,count;

    if(arg == "list"){
         return "/cmds/wiz/more"->main("/doc/help/theme/house_list");
    }
    if(!arg){return help();}
       if (sscanf(arg, "%s %s %s %d", org, order, name, rank) != 4){
       if (sscanf(arg, "%s %s %s", org, order,name ) != 3)
       if (sscanf(arg, "%s %s", org, order) != 2)
       if (sscanf(arg, "%s", org) != 1)
       return help();
    }
    members = ([]);
    ally = ([]);
    RESTORE;
    if(!query_member(org) && this_player()->query_security_level()){
        write("That house does not exist.\n");
        return 1;
    }
    switch(order) {
       case "add": 
       if(query_member(org, this_player()->query_real_name()) < 2)
          if(!this_player()->query_security_level()){
            write("You dont have access to that command in "+
            "this organization.\n");
            return 1;
            }  
       if(query_total_members(org) > 9){
            write("This organization is limited to 10 members ranks 1-4.\n");
            return 1;
            } 

       if(!find_player(name)){
            write("Cannot find "+name+".\n");
            return 1;
       }
       if(find_player(name)->query_invis()){
            write("Cannot find "+name+".\n");
            return 1;
       }
       if(find_player(name)->query_house() != "commoner"){
            write("Sorry "+name+" is already in a house.\n");
            return 1;
       }
       if(rank+1 > query_member(org,this_player()->query_real_name()) &&
            !this_player()->query_security_level()){
            write("Your security is too low to do that.\n");
            return 1;
            }
            write("You have added "+name+" to the House of ");
            write(capitalize(org)+", which now has "
            +query_total_members(org)+" members.\n");
            find_player(name)->set_house(org);
            set_member(org,name,1);
            log_org(org, arg);
            SAVE;
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
       if(rank > 3 && !this_player()->query_security_level()){
           write("Only immortals can make regents.\n");
           return 1; 
           }
       if(!rank || rank > 4){
            write("Thats not a valid rank it must be 1 through 4.\n");
            return 1;
            }
  if(!this_player()->query_security_level())
       if(!query_member(org,name)){
            write("For new members use: house <house> add <name>.\n");
            return 1;
            }
       if(query_member(org,name)+1 > 
            query_member(org,this_player()->query_real_name()) &&
            !this_player()->query_security_level()){
            write("You cant demote them.\n");
            return 1;
            }
 
       if(rank < 1){
            write("Use the house remove command for this.\n");
//          if(4 > query_member(org,this_player()->query_real_name()) &&
//          !this_player()->query_security_level()){
//          write("Only rank 3 + 4 house members can outcast!\n");
            return 1;
            }

       if(rank+1 > query_member(org,this_player()->query_real_name()) &&
            !this_player()->query_security_level()){
            write("Your security is too low to do that.\n");
            return 1;
            }
            write("You have (pro-de)moted "+name+" to house rank "+rank+".\n");
            log_org(org, arg);
            set_member(org,name,rank);
            SAVE;
            return 1;
            break;
       case "remove": 
       case "quit":
       if(!query_member(org, this_player()->query_real_name()) &&
          !query_ally(org, this_player()->query_real_name()) &&
          !this_player()->query_security_level()){
          write("You dont have access to that command in "+
             "this organization.\n");
            return 1;
            }
       if(name != this_player()->query_real_name())
       if(query_member(org, this_player()->query_real_name()) < 3)
          if(!this_player()->query_security_level()){
            write("You dont have access to that command in "+
            "this organization.\n");
            return 1;
            }  
            if(find_player(name)){
                  find_player(name)->remove_channel(org);
               if(find_player(name)->query_house() == org)
                  find_player(name)->set_house("remove");
            }
            log_org(org, arg);
            remove_member(org,name); 
            remove_ally(org,name); 
            return 1;
            break;

       case "ally":
        if(!user_exists(name)) {
                    return 1;
        }
 
       if(query_ally(org, this_player()->query_real_name())){
                ally(org);
                return 1;
          }
       if(query_member(org, this_player()->query_real_name()) < 1)
          if(!this_player()->query_security_level()){
            write("You dont have access to that command in "+
            "this organization.\n");
            return 1;
            }  

         write("You have added "+name+" to the House of "+org+" allies.\n");
            set_ally(org,name,1);
            log_org(org, arg);
            SAVE;
            return 1;
            break;
    default:  
     printf("%s%s\n\t\t      * The House of %s *\n",
     PRINTBAR,B_BLUE_F,capitalize(org));
     env = (mapping) query_member(org);
     if(!env) return printf("No members!\n"),1;
     j = sizeof( idx = sort_array(m_indices(env),#'sort_by_value));
     count = 0;
   for(i = 0; i < j; i++){
     name = idx[i];
     pretitle = query_title(org,idx[i]);
    printf(B_YELLOW_F"  %-15s"B_BLUE_F"%-19s%s" ,
     capitalize(name),capitalize(pretitle),count% 2 ? "\n" : "  ");
      count ++;
    }

////////
     write("\n");
     env = (mapping) query_ally(org);
     if(!env) return printf("No allies!\n"),1;
     j = sizeof( idx = sort_array(m_indices(env),#'sort_by_value));
     count = 0;
   for(i = 0; i < j; i++){
     name = idx[i];
     pretitle = query_title(org,idx[i]);
    printf(B_YELLOW_F"  %-15s"B_BLUE_F"%-19s%s" ,
     capitalize(name),capitalize(pretitle),count% 2 ? "\n" : "  ");
      count ++;
    }
////


    printf("\n%s\n",PRINTBAR);
    return 1;
    }
    help();
}
 

status help(){
         write("See also 'help houses'.\n");
         write("See also 'help house ranks'.\n");
         write("See also 'help house cmds'.\n");
         write("See also 'help theme'.\n");
         return 1;
}
