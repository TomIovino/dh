
#include <ansi_filter.h>

void get_desc(string s, string d, int i);
#define me this_player()

status main(string arg) {
    string s;
    if(arg) {
        switch(arg) {
           case "-c":
           case "clear":
           case "none":
               me->set_long(0);
               printf("Description cleared.\n");
               return 1;
               break;
           default:
               notify_fail("describe: Syntax:  describe [clear]\n");
               return 0;
               break;
        }
    }
    s = "";
  write("NOTICE!!!!!!!!!!!!\n");
  write("Only physical charactor descriptions are allowed in this space.\n");
  write("Anything else violates the rules, and will be punished. =(     \n");
    printf("Enter your characters description below (max 5 lines). (** saves, ~q aborts)\n");
    
    get_desc(0,s,0);
    return 1;
}

void set_desc(string e);

void get_desc(string e, string d, int line) {
    if(e) {
        e = to_string(e);
        switch(e) {
            case "**":
                set_desc(d);
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
    if(line > 5) return get_desc("**",d,0);
     printf("%1d] ",line);
     input_to("get_desc",0,d,line);
     return;
}

void set_desc(string desc) {
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
log_file("DESC","\n\n"+this_player()->query_real_name()+"--"+ctime()+"\n"+desc+"\n");
    me->set_long(desc + "\n");
    printf("Description changed.");
    return;
}
