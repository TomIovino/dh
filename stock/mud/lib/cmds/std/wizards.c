
// SKILLS (ash)

#include <bar.h>
#define TP this_player() 
 
mapping env;

status settings();
string desc();
// string *query_cmd_aliases() { return  ({"sk","skill", }); }

int sort_by_value(string k1, string k2) {
    return (int)env[k1] > (int)env[k2];
} 
 
status main() {
    string doh;
    string *idx;
    int i, j,count;
    string desc;

    env = (mapping) "secure/valid_d"->live_site_wiz();    
    env += (mapping) "secure/valid_d"->test_site_wiz();    
    printf("%s\n",PRINTBAR);

    j = sizeof( idx = sort_array(m_indices(env),#'sort_by_value));
     count = -1;
    for(i = 0; i < j; i++){
     count ++;
        switch(env[idx[i]]){
        case 0: doh = 0; 
       break;
        case 1..20: doh =    YELLOW_F    "(student builder)  ";
       break;
        case 21..30: doh =   YELLOW_F   "(builder)          ";
       break;
        case 31..40: doh =   YELLOW_F   "(creator)          ";
       break;
        case 41..50: doh =   YELLOW_F   "(creator)          ";
       break;
        case 51..80: doh =  YELLOW_F   "(contributor)      ";
       break;
        case 81..200: doh =  YELLOW_F   "(admin)            ";
       break; 
    default:
       doh = "(*corrupt map)";       
      break;    
     }
    if(doh)
    printf(B_WHITE_F "%-18s"  "%-17s%s" ,
           capitalize(idx[i]),NORM+doh,count% 2 ? "\n"
    : "  ");
   }
//   printf((count % 2 ? "\n" : ""));
   printf("\n%s\n",PRINTBAR);
   return 1;
}
 
