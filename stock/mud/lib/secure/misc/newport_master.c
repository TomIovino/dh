///////////////////////////
/// port_switch_master.c 
///////////////////////////
///  used to notify players of 6666 -> 8888 port change
///////////////////////////


/////// Striped, hacked, not suitible for use in a real mud


////////////////////////////////////////////////////////////
// MASTER.C         //  DRAGONHEART  // GEEWHIZ.5.17.97   //
////////////////////////////////////////////////////////////
 
#pragma verbose_errors
#pragma strict_types

#define GC_TIME 3600
#define BACKBONE_WIZINFO_SIZE 10

 /* Master File for DRAGONHEART mudlib */

#include "/include/mudlib.h"
#include "/include/cfg/levels.h"
#include "/include/cfg/master.cfg"
#include "/include/fn/driver_hook.h"

string get_master_uid() { return "MASTER"; }


string master_file_name(object ob) {
  string file;

if(!ob || !objectp(ob)) {
    printf("master->master_file_name(%O)\n",ob);
    return "<Unknown>";

}
  file = file_name(ob);
  if(file[0] == '/') file = file[1..(strlen(file)-1)];
  return file;
}

#define file_name master_file_name

// #endif /* NATIVE */

 /***************************************************************************/
 /* start-up flags */

void flag(string str) {
    string file, arg;

    if(sscanf(str, "echo %s", arg) == 1) {
       write(arg + "\n");
       return;
    }
    if(sscanf(str, "call %s %s", file, arg) == 2) {
       arg = (string)call_other(file, arg);
       write("Call Flag: "+ arg +"\n");
       return;
    }
    write("Master: Unknown flag " + str + "\n");
}



 /***************************************************************************/
 /* object player is connected */

 /*
 * This function is called every time a player connects.
 * input_to() can't be called from here.
 */

object connect() {
    object ob;
    string ret1, ret2;

    write("\n");
    if(ret1 = catch(ob = clone_object("/secure/newport.c")))
        write(ret1);
    return ob;
}


 /************************************************************************/
 /* changed to security_level */

 /*
 * There are several occasions when the game driver wants to check if
 * a player has permission to specific things.
 *
 * These types are implemented so far:
 * "error messages":     If the player is allowed to see runtime error
 *             messages.
 * "trace":         If the player is allowed to use tracing.
 * "wizard":        Is the player considered at least a "minimal" wizard ?
 * "error messages":     Is the player allowed to get run time error messages ?
 */

int query_player_level(string what) {
  int security_level;

  if(!this_player()) return 0;
  security_level = (int)this_player()->query_security_level();
  switch(what) {
    case "wizard":
      return (security_level >= SEC2);
    case "error messages":
      return (security_level >= SEC2);
    case "trace":
      return (security_level >= SEC0);
    case "arch":
      return (security_level >= SEC8);
  }
}

 /***************************************************************************/
 /* a number of fn() in master will accept calls only from specific objects */
 /* they form the basis of the security of the mud                          */


status valid_master_call(object ob) {
  int i;
  string file, *valid_master;

  if(!ob) return 0;
  file = file_name(ob);
  
  for(i = sizeof(MASTER_FILES); i--; ) {
    if(file == MASTER_FILES[i]) return 1;
  }
  
#ifdef MASTER_DIR
  for(i = sizeof(MASTER_DIR); i--; ) {
    if(sscanf("/"+ file, MASTER_DIR[i], file)) return 1;
  }
#endif /* MASTER_DIR */
  return 0;
}


status valid_player_call(object ob) {
  int i, arg;
  string file, *valid_player;

  if(!ob) {
    return 0;
  }
  if(valid_master_call(ob)) return 1;

  /* valid player files - are all cloned objects */
  valid_player = ({
    PLAYER, WIZARD,
#ifdef DEBUG
    DEBUG,
#endif /* DEBUG */
  });

  file = file_name(ob);
  for(i = sizeof(valid_player); i--; ) {
    if(sscanf(file, valid_player[i] +"#%d", arg)) return 1;
  }
  return 0;
}
 



 /****************************************************************************/
 /*
 * When an object is destructed, this function is called with every
 * item in that room. We get the chance to save players !
 */
string caller_stack();

void destruct_environment_of(object ob) {
  if(!interactive(ob)) return;
  tell_object(ob,
  "Everything you see is disolved.\n"+
  "Luckily, you are transported somewhere...\n");
  ob->move_player("is transfered#"+ VOID,0,1); /* domain safe move */
}



 /**********************************************************************/
 /* include directories */

 /*
 * Define where the '#include' statement is supposed to search for files.
 * "." will automatically be searched first, followed in order as given
 * below. The path should contain a '%s', which will be replaced by the file
 * searched for.
 */

string *define_include_dirs() {
   return 
   ({ "/include/%s", "/include/spc/%s","/include/cfg/%s",
     "/include/fn/%s","/include/skills/%s" });
}

 /**********************************************************************/

 /*
 * The master object is asked if it is ok to shadow object ob. Use
 * previous_object() to find out who is asking.
 *
 * In this example, we allow shadowing as long as the victim object
 * hasn't denied it with a query_prevent_shadow() returning 1.
 */

int query_allow_shadow(object ob) {
    string file;

     return 1;
}


 /************************************************************************/
 /* 3.2 allows two people to edit same file simultaneously!!
 * here is a hack to stop that
 */


 /*
 * Give a path to a simul_efun file. Observe that it is a string returned,
 * not an object. But the object has to be loaded here. Return 0 if this
 * feature isn't wanted.
 */

static string get_simul_efun_file() {
    return 0;

}

string get_simul_efun() {
  return get_simul_efun_file();
}


 /****************************************************************************/
 /* remove file for player object */

status master_remove_file(string file) {
  return (valid_player_call(previous_object())) ? rm(file) : 0;
}


 /**************************************************************************/
 /* exec() is called in player.c */

 /*
 * Function name:   valid_exec
 * Description:     Checks if a certain 'program' has the right to use exec()
 * Arguments:       name: Name of the 'program' that attempts to use exec()
 *                        Note that this is different from file_name(),
 *                        Programname is what 'function_exists' returns.
 *                  NOTE, the absence of a leading slash in the name.
 * Returns:         True if exec() is allowed.
 */

int valid_exec(string name) {
  return 1;
}



void runtime_error(string error,
           string program,
       string current_object,
                 int line) {
  if(this_player() && query_ip_number(this_player())) {
    if(!query_player_level("error messages") || !error) {
      write("Ash exclaims: Woops!!\n"+
         "                    You have found a Space-Time Anomaly.\n");
    }
    else {
      write(error +"\n");
      write(((current_object) 
        ? "program: "+ program +", object: "+ current_object 
         +" line "+ line +"\n"
        : ""));
    }
  }
}


int heart_beat_error(object heart_beat,
                 string error,
               string program,
           string current_object,
                     int line) {
  string wiz_name;

  if(heart_beat && query_ip_number(heart_beat) ) {
    tell_object(heart_beat,"Ash tells you: You have no heart beat !\n");
    if(query_player_level("error messages")) {
      tell_object(heart_beat, error +"\n");
      tell_object(heart_beat, ((current_object)
     ? "program: "+program+", object: "+current_object+" line "+line+"\n"
     : ""));
    }
  }
    
  return 0; /* Don't restart */
}



mixed make_path_absolute(string path) { 
  return (string)path;
}



 /*************************************************************************/
 /* validates whether an object can be loaded  */
 /* in native it gives object its uid          */

static mixed master_creator_file(string object_name) {
  return 1;  /* else legal */
}


mixed creator_file(string object_name) {
  mixed creator_status;

  if((creator_status = master_creator_file(object_name))) {
    return creator_status;
  }
  return 0;
}



void move_or_destruct(object what, object to)
 /* An error in this function can be very nasty. Note that unlimited recursion
 * is likely to cause errors when environments are deeply nested
 */
{
    do {
     int res;
     if (catch( res = transfer(what, to) )) res = 5;
     if ( !(res && what) ) return;
    } while( (res == 1 || res == 4 || res == 5) && (to = environment(to)) );

    /*
     * Failed to move the object. Then, it is destroyed.
     */
    destruct(what);
}


 /******************************************************************/
 /* snoops must routed via simul_efun */

int valid_snoop(object snooper, object snoopee) {
   return 1;
}


 /****************************************************************/
 /* snoops are usable by priviledged people */

int valid_query_snoop(object wiz) {
  return 1;
}



mixed *prepare_destruct1(object ob) {
    object super;

    super = environment(ob);
    if (super) {
     mixed error, *errors;
     mixed weight;
     object me;
     me = this_object();
     set_this_object(ob);
     errors = ({});
     if ( living(ob) ) {
      if (error = catch(super->exit(ob),0))
       errors = ({"exit"+": "+error});
     }
     if ( error = catch((weight = (mixed)ob->query_weight()),0) ) {
      set_this_object(me);
      return ({"query_weight"+": "+error}) + errors;
     }
     if (weight && intp(weight)) {
      if (error = catch(super->add_weight(-weight),0)) {
       set_this_object(me);
       return ({"add_weight"+": "+error}) + errors;
      }
     }
     set_this_object(me);
    }
    return ({});
}


mixed prepare_destruct(object ob) {
    object super;
    mixed *errors;
    int i;

#if 6 * 7 != 42 || 6 * 9 == 42
    return "Preprocessor error";
#endif

    catch(ob->notify_destruct());
    catch(ob->destructor());
    errors = prepare_destruct1(ob);
    for(i = sizeof(errors); i--; ) {
     write(errors[i]);
    }
    super = environment(ob);
    if (!super) {
     object item;

     while ( item = first_inventory(ob) ) {
      destruct_environment_of(item);
      if (item && environment(item) == ob) destruct(item);
     }
    } else {
     while ( first_inventory(ob) )
      move_or_destruct(first_inventory(ob), super);
    }
    return 0; /* success */
}

 /* privilege_violation is called when objects try to do illegal things,
 * or files being compiled request a privileged efun.
 *
 * return values: 
 *   1: The caller/file is allowed to use the privilege.
 *   0: The caller was probably misleaded; try to fix the error.
 *  -1: A real privilege violation. Handle it as error.
 */

int privilege_violation(string what, mixed who, mixed arg) {
    return 1;
}



void receive_imp(string sender,string msg, int port) {
     return;
}


void dangling_lfun_closure() {
  raise_error("dangling lfun closure\n");
}


void slow_shut_down(int minutes) {
}


void remove_player(object victim) {
  catch(victim->quit_2(0));
  if(victim) destruct(victim);
}


 /************************************************************************/
 /* I don't know anyone who uses it?
 * Parse_command() is a heavy load */

 /*
 * Default language functions used by parse_command() in non -o mode
 */

string *parse_command_id_list()
{
    return ({ "one", "thing" });
}

string *parse_command_plural_id_list()
{
    return ({ "ones", "things", "them" });
}

string *parse_command_adjectiv_id_list()
{
    return ({ "iffish" });
}

string *parse_command_prepos_list()
{
    return ({ "in", "on", "under", "behind", "beside" });
}

string parse_command_all_word()
{
    return "all";
}



string get_root_uid() { return "root"; }





mixed valid_write(string path, 
           string eff_user,
           string call_fun,
          object caller) {


   return path;
  return 0; /* access denied */
}



mixed valid_read(string path, 
          string eff_user, 
          string call_fun, 
            object caller) {
  mixed *error;
  string tmp, outp;
  string file;
  int i;

  return path;
}

 /**************************************************************************/
 /* wizlist */


void inaugurate_master(int arg) {

    set_auto_include_string("#pragma verbose_errors\n#pragma combine_strings\n");
 
    set_driver_hook(
      H_MOVE_OBJECT0,
      unbound_lambda( ({'item, 'dest}), ({#',,
     ({#'&&, ({#'living, 'item}), ({#'environment, 'item}), ({#',,
       ({#'efun::set_this_player, 'item}),
       ({#'call_other, ({#'environment, 'item}), "exit", 'item}),
     }) }),
     ({#'efun::efun308, 'item, 'dest}),
     ({#'?, ({#'living, 'item}), ({#',,
       ({#'efun::set_this_player, 'item}),
       ({#'call_other, 'dest, "init"}),
       ({#'?, ({#'!=, ({#'environment, 'item}), 'dest}), ({#'return})}),
     }) }),
     ({#'=, 'others, ({#'all_inventory, 'dest}) }),
     ({#'=, ({#'[, 'others, ({#'member, 'others, 'item}) }), 0}),
     ({#'filter_array, 'others,
       ({#'bind_lambda,
         unbound_lambda( ({'ob, 'item}),
           ({#'?, ({#'living, 'ob}), ({#',,
          ({#'efun::set_this_player, 'ob}),
          ({#'call_other, 'item, "init"}),
           }) })
         )
       }),
       'item,
     }),
     ({#'?, ({#'living, 'item}), ({#',,
       ({#'efun::set_this_player, 'item}),
       ({#'filter_objects, 'others, "init"}),
     }) }),
     ({#'?, ({#'living, 'dest}), ({#',,
       ({#'efun::set_this_player, 'dest}),
       ({#'call_other, 'item, "init"}),
     }) }),
      }) )
    );
    set_driver_hook(
      H_LOAD_UIDS,
      unbound_lambda( ({'object_name}),
     ({#'?,
       ({#'==,({#'sscanf, 'object_name, "players/%s", 'wiz_name}),1,}),
       ({#'?,
         ({#'==,({#'sscanf, 'wiz_name, "%s/%s", 'start, 'trailer}),2,}),
         ({#'&&, ({#'strlen, 'start}), 'start}),'wiz_name
       }),
       ({#'&&,
         ({#'!=, ({#'[..], 'object_name, 0, 3}), "ftp/"}),
         ({#'!=, ({#'[..], 'object_name, 0, 4}), "open/"}),
       })
     })
      )
    );
    set_driver_hook(
      H_CLONE_UIDS,
      unbound_lambda( ({'blueprint, 'new_name}), ({
     #'||,
       ({#'creator, 'blueprint}),
       ({#'creator, ({#'previous_object})}),
       1
      }) )
    );
    set_driver_hook(H_CREATE_SUPER,"reset");
#if 0
      unbound_lambda(0,
       ({#'||,
           ({#'call_other, ({#'this_object}), "create"}),
           ({#'call_other, ({#'this_object}), "reset", 0})
       })
      )
    );
#endif
    set_driver_hook(H_CREATE_OB,"reset");
#if 0
      unbound_lambda(0,
       ({#'||,
           ({#'call_other, ({#'this_object}), "create"}),
           ({#'call_other, ({#'this_object}), "reset", 0})
       })
      )
    );
#endif
    set_driver_hook(H_CREATE_CLONE,"reset");
#if 0
      unbound_lambda(0,
       ({#'||,
           ({#'call_other, ({#'this_object}), "create"}),
           ({#'call_other, ({#'this_object}), "reset", 0})
       })
      )
    );
#endif 
   set_driver_hook(H_RESET,"reset");
#if 0
      unbound_lambda(0, 
       ({#'call_other, ({#'this_object}), "reset", 1})
     
   ));  
#endif
    set_driver_hook(H_CLEAN_UP,     "clean_up");
    set_driver_hook(H_MODIFY_COMMAND, ([
      "e":"east", "w":"west", "s":"south", "n":"north",
      "d":"down", "u":"up",
      "nw":"northwest", "ne":"northeast", "sw":"southwest", "se":"southeast",
    ]));
    set_driver_hook(H_MODIFY_COMMAND_FNAME,"modify_command");
    set_driver_hook(H_NOTIFY_FAIL, "What ?\n");
    set_driver_hook(H_INCLUDE_DIRS, map_array(
        define_include_dirs(),
        #'[..<],
        0, 3
    ));

}
 


mixed current_time;

string *epilog(int eflag) {
    if (eflag) return ({});
    debug_message(sprintf("Loading init file %s\n", "<builtin>"));
    current_time = rusage();
    current_time = current_time[0] + current_time[1];
    return ({ "/secure/newport" });
}

void preload(string file) {
    int last_time;
    if (strlen(file) && file[0] != '#') {
        last_time = current_time;
        debug_message(sprintf("Preloading: %s", file));
        call_other(file, "");
        current_time = rusage();
        current_time = current_time[0] + current_time[1];
        debug_message(sprintf(" %.2f\n", (current_time - last_time)/1000.));
    }
}


mixed wizinfo(mixed arg) { return get_extra_wizinfo(arg); }
mixed master_wizlist_info() { return wizlist_info(); }
int uptime() { return get_extra_wizinfo(0)[9]; }



