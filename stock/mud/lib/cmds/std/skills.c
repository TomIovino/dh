
// PRAC (ash)

#include <bar.h>
#define TP find_player(arg) 

string *query_cmd_aliases() { return ({"skill","abilities" });}
 
mapping env;

status settings();
string desc();

int sort_by_value(string k1, string k2) {
    return (int)env[k1] > (int)env[k2];
} 
 
status main(string arg) {
    string doh;
    string *idx;
    int x, i, j,count;
    string desc;
 
  if(!arg) arg = this_player()->query_real_name();
  if(!this_player()->query_security_level())
      arg = this_player()->query_real_name();
  if(!find_player(arg)){ write("No.\n"); return 1; }

    env = (mapping) TP->query_skill();


    printf("%s\n",PRINTBAR);

    j = sizeof( idx = sort_array(m_indices(env),#'sort_by_value));
     count = -1;
    for(i = 0; i < j; i++){
     count ++;
// PRAC ONLY
   if(find_object("secure/valid_d")) // novelconcept
    if(!"secure/valid_d"->is_valid_skill(idx[i]) &&
       !"secure/valid_d"->is_valid_spell(idx[i])){
        write("\n"+idx[i]+"(REMOVED)\n");
             TP->unset_skill(idx[i]);
          }
// PRAC ONLY
     x = env[idx[i]];
      if(!"secure/valid_d"->is_valid_skill(idx[i]))
         { count-- ; continue; }
        switch(x){
        case 0: doh = 0; 
       break;
        case  1 ..4:  doh =  WHITE_F   +x+"  (bad)          ";
       break;
        case   5..9:  doh =  YELLOW_F  +x+"  (poor)         ";
       break;  
        case 10..19:  doh =  BROWN_F   +x+" (decent)       ";
       break;
        case 20..29:  doh =  RED_F     +x+" (average)      ";
       break;
        case 30..39:  doh =  CYAN_F    +x+" (good)         ";
       break;
        case 40..49:  doh =  B_BLUE_F  +x+" (great)        ";
       break; 
        case 50..59:  doh =  MAGENTA_F +x+" (excellent)    ";
       break;
        case 60..69:  doh = B_MAGENTA_F +x+" (incredible)   ";
       break;
        case 70..79: doh =   GREEN_F   +x+" (amazing)      ";
       break;
        case 80..89: doh =   GREEN_F   +x+" (superb)       ";
       break;
        case 90..99: doh =   B_GREEN_F +x+" (*DIVINE*)     ";
       break;
        case 100..200: doh = B_GREEN_F +x+"(*GODLY*)      ";
       break;
    default:          doh =  B_RED_F      "   (*corrupt)     ";       
      break;    
     }
    if(doh)
    printf(B_BLUE_F "%-18s"  "%-17s%s" ,
           idx[i],NORM+doh,count% 2 ? "\n"
    : "  ");
   }
//   printf((count % 2 ? "\n" : ""));
   write("\nTotal Practiced Disciplines:"+TP->query_total_skills()+".\n");
   printf("%s\n",PRINTBAR);
   return 1;
}
 

help(){
    write("command : prac\n");
    write("This lists all know abilities.\n");
    write("Type 'prac validonly' to remove ALL invalid skills, which\n");
    write("could possibly lower the cost of your next skill/spell purchase.\n");
   return 1;
}

