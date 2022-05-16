#include <mudlib.h>
#include <rank_d.h>
#include <party.h>

private nomask string race, name;
private nomask int level, total_exp, security_level, al;
private static nomask int cost;

#define MAX_COST 20000
#define CALL_OUT_TIME 6


#define DEBUG_WIZARD_NAME "geewhiz"
#define NO_PRINTD
#define NO_ASSERT
#include <debug_utils.h>


string get_name(string file) { return explode(file,"/")[<1]; }

string fix_file(string file, string dir) {
    return sprintf("%s/%s",dir,file[0..<3]);
}

string *get_dir_special(string dir, string spec) {
    string *files;
    files = get_dir(sprintf("%s/%s",dir,spec));
    if(!files || !sizeof(files))
        return ({ });
    return map_array(files,#'fix_file,dir);
}                             

int len = 0;

varargs nomask int continue_rank(int x, mixed *list, object ob){
    int i, size;
    int ret;
    string err;
    string n;
    cost = get_eval_cost();
    if (this_interactive()) ob = previous_object();
    if (!list) {
                list = get_dir_special(sprintf("/usr/mortal/%c", x),"*.o")
                     + get_dir_special("/usr/newbies",sprintf("%c*.o", x));
    }
PRINTD(list);    
    for(size = sizeof(list); i < size; i++ ) {
        if((cost - get_eval_cost() > MAX_COST) && (i < size)) {
            call_out("continue_rank", CALL_OUT_TIME, x, list[i..], ob);
            return 1;
        }
        name = 0;
        if(err = catch(ret = restore_object(list[i]))) {
            printf("*** %s *** Error Restoring = %s",list[i],err);
            break;
        }
        if(!ret) {
            write("Error in restoring "+list[i]+".\n");
            break; 
        }
        n = get_name(list[i]);
        if(n != name) {
            printf("*** %s *** n = %O *** name = %O ***\n",list[i],n,name);
        } else {
            if(strlen(name) > len) {
                len = strlen(name);
                printf("Max length now = %d\n",len);
            }
            if(strlen(name) == len) {
                printf("\t%s\n",name);
            }
        }
    }
    if (x == 'z') {
        if(ob) ob->done_ranking();
    } else
        continue_rank(x+1, 0, ob);
}


void reset(int arg) {
  if(arg) return;
  continue_rank('a');
}

