#include <mudlib.h>

nomask private variables private functions inherit "/obj/env_vars";

private nomask string race, name;
private nomask int level, total_exp, security_level, al;
private nomask int on, last_off;
private nomask mapping attribs;
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


string time_to_str(int t) {
    string ret;
    int n;
    if(t == 0) return "no time";
    ret = "";
    if(t < 0) {
        t = -t;
        ret += "-";
    }
    
    if(n = t/31556926) ret += sprintf("%c%dy",
                               (ret!="")?' ':  0 ,n,
                               (n==1) ? 0 : 's'), t %=  31556926;
/*
    if(n = t/2629744) ret += sprintf("%c%d month%c",
                               (ret!="")?' ':  0 ,n,
                               (n==1) ? 0 : 's'), t %=  2629744;
*/
    if(n = t/604800) ret += sprintf("%c%dw",
                               (ret!="")?' ':  0 ,n,
                               (n==1) ? 0 : 's'), t %=  604800;
    if(n = t/86400) ret += sprintf("%c%dd",
                               (ret!="")?' ':  0 ,n,
                               (n==1) ? 0 : 's'), t %=  86400;
    if(n = t/3600) ret += sprintf("%c%dh",
                               (ret!="")?' ':  0 ,n,
                               (n==1) ? 0 : 's'), t %=  3600;
    if(n = t/60) ret += sprintf("%c%dm",
                               (ret!="")?' ':  0 ,n,
                               (n==1) ? 0 : 's'), t %=   60;
    if(t) ret += sprintf("%c%ds",
                        (ret != "")?' ':0,t,(t==1) ? 0 : 's');
    
    return ret == "" ? "<nt>" : ret;
}

varargs nomask int continue_purge(int x, int remove, mixed *list, object ob, int deleted){
    int i, size;
    int ret;
    string err;
    string n;
    int max_time;
    cost = get_eval_cost();
    if (this_interactive()) ob = previous_object();
    if (!list) {
                list = /* get_dir_special(sprintf("/usr/mortal/%c", x),"*.o")
                     + */ get_dir_special("/usr/newbies",sprintf("%c*.o", x));
    }
PRINTD(list);    
    for(size = sizeof(list); i < size; i++ ) {
        if((cost - get_eval_cost() > MAX_COST) && (i < size)) {
            if (this_player())
                printf("Purge continues (%d)..... Next file %s (%d)\n",deleted,list[i], sizeof(list) - i);
            call_out("continue_purge", CALL_OUT_TIME, x, remove, list[i..], ob, deleted);
            return 1;
        }
        name = 0;
        perm_attrib = ([ ]);
        tmp_attrib = ([ ]);
        if(err = catch(ret = restore_object(list[i]))) {
            printf("*** %s *** Error Restoring = %s",list[i],err);
            break;
        }
        if(!ret) {
            write("Error in restoring "+list[i]+".\n");
            break;
        }

        if(!perm_attrib || !mappingp(perm_attrib))
            perm_attrib = ([ ]);

        if(!tmp_attrib || !mappingp(tmp_attrib))
            tmp_attrib = ([ ]);
            
        n = get_name(list[i]);

        if(n != name && n != "zealot") {
            printf("*** %s *** n = %O *** name = %O ***\n",list[i],n,name);
        }
        if(query_attrib("NO_PURGE")) {
            printf("! NO_PURGE -> %s skipped.\n",list[i]);
            continue;
        }
        max_time = (60 * 60 * 24 * 7);// * level * 2) + total_exp;
        //max_time += max_time / 2;
            
        if((time() - last_off) > max_time
        && level < 50) {
            if(!remove) write("!rmd: ");
            printf("%s off time exceeds max (%s > %s)... purging.\n",
              name,time_to_str(time()-last_off),time_to_str(max_time));
            if(remove) {
                if(rename("/"+list[i]+".o","/usr/PURGED/"+n+".o")) {
                    printf("Could not purge %s!\n",n);
                }
            }
            deleted++;
        }
    }

    if (x == 'z') {
        if(ob) ob->done_purge(deleted);
    } else
        continue_purge(x+1, remove, 0, ob, deleted);
}

