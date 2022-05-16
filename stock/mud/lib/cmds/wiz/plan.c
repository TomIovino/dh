
#include <ansi_filter.h>

void query_extra_info(string s, string d, int i);
#define me this_player()

status main(string arg) {
    string s;
    if(arg) {
        switch(arg) {
           case "-c":
           case "clear":
           case "none":
               me->set_extra(0);
               printf("Plan cleared.\n");
               return 1;
               break;
           default:
               notify_fail("plan: Syntax: plan [clear]\n");
               return 0;
               break;
        }
    }
    s = "";
    printf("Enter your characters plan below (max 5 lines). (** saves, ~q aborts)\n");
    
    query_extra_info(0,s,0);
    return 1;
}

void set_extra(string e);

void query_extra_info(string e, string d, int line) {
    if(e) {
        e = to_string(e);
        switch(e) {
            case "**":
                set_extra(d);
                return;
                break;
            case "~q":
                printf("Aborted.\n");
                return;
                break;
            default:
                d += e + "\n";
                break;
         }
     }
    line++;
    if(line > 5) return query_extra_info("**",d,0);
     printf("%1d] ",line);
     input_to("query_extra_info",0,d,line);
     return;
}

void set_extra(string desc) {
    string *lines;
    desc = filter_ansi(desc);
    desc = subst(desc,"\a","*BEL*");
    desc = subst(desc,"\n\n","\r");
    desc = sprintf("%-=79s\n",desc);
    desc = subst(desc,"\n\n","\n");
    desc = subst(desc,"\r","\n\n");
    lines = explode(desc,"\n");
    if(sizeof(lines) > 5) {
        lines = lines[0..4];
    }
    desc = implode(lines,"\n"); 
    me->set_extra(desc + "\n");
    printf("Plan changed.");
    return;
}
