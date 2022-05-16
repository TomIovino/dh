#include <mudlib.h>
#include <rank_d.h>
#include <party.h>

inherit BASE;


#include <valid.cfg>

private static nomask closure add_u;
private static nomask int cost;

private nomask string name, race, alignment, house, profession;
private nomask int level, total_exp, master_exp, security_level;


#define MAX_COST 25000
#define CALL_OUT_TIME 0


#define DEBUG_WIZARD_NAME "geewhiz"
#define NO_PRINTD
#define NO_ASSERT
#include <debug_utils.h>

#define CHANNELD "/secure/channeld/channeld"




reset(arg){
    add_u = symbol_function("update_ranking", RANK_D);
}

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

static int disable = 0;
void clear_disable() { disable = 0; }

static mapping last = ([ ]);

#define MASTER_EXP_VALUE 1000000

void fix_exps(int m_exp, int t_exp) {
    m_exp += t_exp / MASTER_EXP_VALUE;
    t_exp %= MASTER_EXP_VALUE;
}


varargs nomask int continue_rank(int type, int x, mixed *list, object ob){
    int i, size, restored, max_cost;
    string n, err;
    max_cost = type == R_PARTY ? 20000 : MAX_COST;
    cost = get_eval_cost();
    if(!disable) {
        catch(CHANNELD->tell_channel("RankD","Rankings Daemon",sprintf("Updating all %s Rankings",type == R_PLAYER ? "Player" : "Party")));
        catch(RANK_D->disable());
        disable = 1;
    }
    if(x != last[type]) {
        last[type] = x;
        catch(CHANNELD->tell_channel("RankD","Rankings Daemon",sprintf("Now working with %s names starting with the letter '%c'",type == R_PLAYER ? "Player" : "Party",last[type])));
    }
    if (this_interactive()) ob = previous_object();
    if (!list) {
        switch(type) {
            case R_PLAYER:
                list = get_dir_special(sprintf("/usr/mortal/%c", x),"*.o")
                     + get_dir_special("/usr/newbies",sprintf("%c*.o", x));
                break;
            case R_PARTY:
                list = get_dir_special(sprintf("/ob_saves/partyd/%c",x),"*.o");
                break;
            default:
                raise_error("rank_update_d:  continue_rank():  Bad ranking type!\n");
                break;
        }
    }
PRINTD(list);    
    for(size = sizeof(list); i < size; i++ ) {
        if((cost - get_eval_cost() > max_cost) && (i < size)) {
            if (this_player())
                printf("\nRanking continues..... Next file %s (%d)\n",list[i], sizeof(list) - i);
            call_out("continue_rank", CALL_OUT_TIME, type, x, list[i..], ob);
            return 1;
        }
        switch(type) {
            case R_PLAYER:
                name = n = master_exp = total_exp = 0;
                if(err = catch(restored = restore_object(list[i]))) {
                    write("\nError in restoring "+list[i]+".\n\t"+err);
                    break;
                }
                if(!restored) {
                    write("\nError in restoring "+list[i]+".\n");
                    break;
                }
                printf(".");
                n = get_name(list[i]);
                if(security_level > 0) break;
                if(n != name) {
                    printf("\n*** %s: Name does not match file!  %O vs. %O\n",list[i],n,name);
                    break;
                }
                if(level < 1)
                    break;
                if(!stringp(race) || race == "" || !valid_race[race])
                    break;
                if(!stringp(alignment) || alignment == "" || !valid_alignment[alignment])
                    break;
                if(!stringp(house) || house == "")
                    break;
                if(!stringp(profession) || profession == "" || !valid_profession[profession])
                    break;                    
                fix_exps(&master_exp, &total_exp);
                    
                funcall(add_u, type, n, master_exp, level, race, alignment, house, security_level, total_exp, profession);
                break;
                
            case R_PARTY:
                name = get_name(list[i]);
                printf(".");
PRINTD(name);
                if(!PARTY_D->restore_party(name)) {
                    write("Error in restoring "+list[i]+".\n");
                    break;
                }
                PARTY_D->update_party_ranking(name);
                break;
                
            default:
                raise_error("rank_update_d:  continue_rank():  Bad ranking type!\n");
                break;
        }
        
    }
    if (x == 'z') {
        if(ob) ob->done_ranking();
        catch(RANK_D->enable(type));
        disable = 0;
        catch(CHANNELD->tell_channel("RankD","Rankings Daemon",sprintf("Finished with %s rankings.",type == R_PLAYER ? "Player" : "Party")));
    } else
        continue_rank(type, x+1, 0, ob);
}

