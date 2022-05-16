#include <mudlib.h>
#include <ansi.h>


string *query_cmd_aliases() { return ({ "pp", "pep" }); }
#include <filters.h>
 /************************************************************************/
 /* list_peoples */
  
void page(string s) {
    object pager;
    move_object((pager=clone_object(PAGER)),this_player());
    pager->page(s);
    return;
}
main(arg) { 
  string temp, domain_str;
  object *who;
  string str, outp, space;
  int i;
  int a; 
  int idle_count;
  string location;
     
  if(!this_player()->query_security_level()) return 1; 

  who = filter_array(users(),"filter_users",this_object());  

  if(arg == "1")
    who = sort_array(who, "by_name", this_object());
  else if(arg == "2")
    who = sort_array(who, "by_level", this_object());
  else if(arg == "3")
    who = sort_array(who, "by_age", this_object());
  else if(arg == "4")
    who = sort_array(who, "by_environment", this_object());
  else if(arg == "5")
    who = sort_array(who, "by_ip_number", this_object());
  else 
    who = sort_array(who, "by_sec_level", this_object());
 if(!this_player()->is_stupid()){
    str =  "Name           WL Level Age   Idle IP Address      Location\n"; 
    str += "-----------   --- ----- ----- ---- ---------------";
    str += " -------------------------\n"; 

    write(str);
    for(i = 0; i < sizeof(who); i++) {
      str = capitalize((string)who[i]->query_real_name());
      str += (who[i]->query_invis())
          ? " I"
          : "";
      str += (who[i]->query_edit())
          ? " E"
          : "";
      str += (who[i]->query_disguise_on())
          ? " D"
          : "";
      str += "                                ";
      str = extract(str,0,12) + " ";
      str += (string)who[i]->query_security_level()+"         ";
      str = extract(str,0,16) + " ";
      str += (string)who[i]->query_extrastats_total()+"-"+
             (string)who[i]->query_level()+"        ";
      str = extract(str,0,24) + " ";
      a = (int)who[i]->query_age();
      if(a/302400) {
        str += (a/302400) + " W                  ";
      }
      else if(a/43200) {  
        str += (a/43200) +" D                    ";  
      }  
      else if(a/1800) {  
        str += (a/1800) +" h                     ";  
      }  
      else if(a/30) {  
        str += (a/30) +" m                       ";  
      }  
      else 
        str += (a*2) + " s                        ";
      str = extract(str,0,29) + " ";
      str += (query_idle(who[i])/60)+"            ";
      str = extract(str,0,33) + " ";
      str += query_ip_name(who[i])+"            ";
      str = extract(str,0,49) + " ";

      location = file_name(environment(who[i]));  
      if(sscanf(location, extract(WIZARD_DIR +"%s",1), temp))
        str += "~" + temp;  
      else if(sscanf(location, extract("/g/env/" +"%s",1), temp))
        str += "#" + temp;  
      else
        str += location;
      str = extract(str,0,78) + "\n";
      write(str);
      if(query_idle(who[i]) > 10) idle_count++;
    }  
    write("\nThere are now " + i + " players ("+(i-idle_count)+" active). ");  
    write(query_load_average() + "\n\n");  
    return 1;  
  } /* if no color */

  printf("%60|s\n", YELLOW_F+"--== Wizard Style Who Listing ==--"+NORM);
  outp = "\n";
  space = "                                 ";
  for(i = 0; i < sizeof(who); i++) {
    if((string)who[i]->query_real_name())
      str = B_BLUE_F+capitalize((string)who[i]->query_real_name())+ ":";
    str += (who[i]->query_invis())
        ? " (  I) "
        : "";
    str += (who[i]->query_edit())
        ? " (E) "
        : "";
    str += (who[i]->query_disguise_on())
        ? " (D) "
        : "";
    str += "                                 ";
    str = extract(str,0,15);
    str += RED_F+"\tWL:    "+(string)who[i]->query_security_level()+ "\t";
    a = (int)who[i]->query_age();
    if(a/302400) { 
      str += (a/302400) + CYAN_F+" WK old ";
    }
    else if(a/43200) {  
      str += (a/43200) +CYAN_F+" DY old ";  
    }  
    else if(a/1800) {  
      str += (a/1800) +CYAN_F+" hr old ";  
    }  
    else if(a/30) {  
      str += (a/30) +CYAN_F+" mn old ";  
    }  
    else 
      str += CYAN_F+(a*2) + " s old ";
    str += "\t"+GREEN_F+" IP: "+query_ip_name(who[i])+"\n";
    str += MAGENTA_F+"              Lvl:   "+(string)who[i]->query_level();
    str += "\t"+WHITE_F+"   Idle:  "+(query_idle(who[i])/60) +"\t";
    location = file_name(environment(who[i]));  
    if(sscanf(location, extract(WIZARD_DIR +"%s",1), temp))
      str += BROWN_F+"Loc: "+"~" + temp;  
    else if(sscanf(location, "room/%s", temp))  
      str += BROWN_F+"Loc: "+"#" + temp;  
    else if(sscanf(location,extract(DOMAIN_DIR+"%s/w/%s",1),domain_str,temp))
      str += BROWN_F+"Loc: "+domain_str +","+ temp;
    else
      str += BROWN_F+"Loc: "+location;
    str = str + NORM+"\n";
    if(query_idle(who[i]) > 10) idle_count++;
  outp += str;
  }
  outp +="\nThere are now " + RED_F+i + NORM+" players ("+(i-idle_count)+" active). ";
  outp += query_load_average() + "\n\n";
  page(outp);
  return 1;  
}
