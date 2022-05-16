////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
 
#pragma verbose_errors
#pragma strict_types

#define GC_TIME 3600
#define BACKBONE_WIZINFO_SIZE 10

//#define NO_MKDIR_IN_DOMAIN_DIR      


#include "/include/mudlib.h"
#include "/include/fn/levels.h"
#include "/include/fn/driver_hook.h"
#include "/include/cfg/master.cfg"
#include "/include/cfg/wizlist.h"

object clone_object(string f) { return efun::clone_object(f); }

string get_master_uid() { return "MASTER"; }


void external_master_reload() {
    catch("/secure/backup_notify_d"->toggle());
}

string object_name(object obj) {
    string ret;
    ret = (string)obj->query_name(1);
    return ret ? ret : 0;
}

void shout(string s) {
    filter_array(users(), lambda(({'u}),({#'&&,
      ({#'environment, 'u}),
      ({#'!=, 'u, ({#'this_player})}),
      ({#'tell_object, 'u, s})
    })));
}

status valid_master_call(object ob);



static void wiz_decay() {
    mixed *wl;
    int i;
    wl = wizlist_info();
    for (i=sizeof(wl); i--; ) {
        set_extra_wizinfo(wl[i][WL_NAME], wl[i][WL_EXTRA] * 99 / 100);
    }
    call_out("wiz_decay", 3600);
}

void save_wiz_file() {
    rm("/WIZLIST");
    write_file(
      "/WIZLIST",
      implode(
        map_array(wizlist_info(),
          lambda(({'a}),
            ({#'sprintf, "%s %d %d\n",
              ({#'[, 'a, WL_NAME}),
              ({#'[, 'a, WL_COMMANDS}),
              ({#'[, 'a, WL_EXTRA})
            })
          )
        ), ""
      )
    );
}

void notify_shutdown() {
  if(!valid_master_call(previous_object())) return;
  filter_array(users(), #'tell_object,
      "Game driver shouts: LPmud shutting down immediately.\n");
  save_wiz_file();
}



/*
void Garbage_Collection_Scheduled() {
    while(find_call_out("Garbage_Collection_Scheduled") != -1)
        remove_call_out("Garbage_Collection_Scheduled");
    call_out("Garbage_Collection_Scheduled",GC_TIME);
    garbage_collection();
    debug_info(3); debug_info(4,0); debug_info(4,"swap");
}
*/

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
#ifdef USE_DEBUG
    return clone_object(DEBUG);
#endif /* force usuage of DEBUG-player */

#ifdef DEBUG
    /* attempt to load WIZARD,
       loads "obj/base_obj",
             "obj/living",
             "obj/player",
             "obj/wizard"
    */
    ret1 = (string)catch(call_other(WIZARD,"??")); 
    if(ret1) {
      write("Crimsonesti Connect:\n");
      ret2 = (string)catch((ob = clone_object(DEBUG)));
      if(ret2) {
     write("Error1: "+ret1+"\n");
     write("Error2: "+ret2+"\n");
     return 0;
      }
      else {
     ob->set_error_msg(ret1);
      }
    }
    else {
      ob = clone_object(PLAYER);
    }   
#else
    ob = clone_object(PLAYER);
#endif
    return ob;
}


 /****************************************************************************/
 /* where are error logs written */

 /*
 * Get the owner of a file. This is called from the game driver, so as
 * to be able to know which wizard should have the error.
 */

static mixed master_creator_file(string ob);

string get_wiz_name(string file) {
    string name, domain, rest;
    mixed ret;
    ret = master_creator_file(file);
    if(ret && intp(ret)) return "Backbone";
    if(!ret) return "Noname";
    return to_string(ret);
}


 /*
 * Write an error message into a log file. The error occured in the object
 * 'file', giving the error message 'message'.
 */

void log_error(string file, string message) {
    string name,rest;

    file = "/"+ file;
    if (sscanf(file, WIZARD_DIR +"%s/%s", name, rest) == 2) {
    } else if(sscanf(file, DOMAIN_DIR +"%s/%s", name, rest) == 2) {
    } else {
        name = "log";
    }
    write_file("/log/"+name, message);
}


 /****************************************************************************/
 /* edit setup save */

 /*
 * The wizard object 'who' wants to save his ed setup. It is saved in the
 * file /players/wiz_name/.edrc . A test should be added to make sure it is
 * a call from a wizard.
 * Don't care to prevent unauthorized access of this file. Only make sure
 * that a number is given as argument.
 */

int save_ed_setup(object who, int code) {
    string file;
    if(!intp(code)) return 0;
    file = WIZARD_DIR+(string)who->query_name(1)+"/"+ ED_SAVE;
    rm(file);
    who->set_attrib("EDRC",code);
    return 1;
    //write_file(file,code+"");
}


 /*
 * Retrieve the ed setup. No meaning to defend this file read from
 * unauthorized access.
 */

int retrieve_ed_setup(object who) {
    string file;
    int code;

    file = WIZARD_DIR+(string)who->query_name(1)+"/"+ ED_SAVE;
    if (file_size(file) <= 0) {
        return (int)who->query_attrib("EDRC");
    }
    sscanf(read_file(file), "%d", code);
    return code;
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
           return (security_level >= SEC2) || 
           this_player()->query_attrib("OK_TO_SEE_ERRORS");
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
  VOID->foo(); // Load the void
  tell_object(ob,
  "Everything you see is disolved.\n"+
  "Luckily, you are transported somewhere...\n");
  ob->move_player("is transfered#" VOID,0,1); /* domain safe move */
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
      "/include/d/%s","/include/fn/%s","/include/skills/%s" });
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
printf("q_a_s(%O) PO = %O\n",ob,previous_object());
    if(previous_object() && valid_master_call(previous_object()))
        return 1;
    if(!sscanf(file_name(previous_object()),"obj/shadows/%s",file)) {
      return 0; /* only shadows that have been inspected can be used */
    }
    /* do not allow shadows with our security fns in them */
    if(function_exists("valid_read",previous_object())
    || function_exists("valid_write",previous_object())) {
      return 0;
    }
//  if(!function_exists("destructor",previous_object())
//  || !function_exists("notify_destruct",previous_object())) {
//    return 0;
//  }
    return !ob->query_prevent_shadow(previous_object());
}


 /************************************************************************/
 /* 3.2 allows two people to edit same file simultaneously!!
 * here is a hack to stop that
 */

static string *current_edit; /* list of files currently edited */

string *query_current_edit() { return current_edit; }


static status add_current_edit(string file) {
  string name;
  int i;

  name = capitalize((string)this_player()->query_name(1));
  if(!current_edit) current_edit = ({});
  if((i = member_array(file, current_edit)) != -1) {
    write("Sorry, "+ file +" is being edited by "+ current_edit[i+1] +".\n"); 
    return 0;
  }
  current_edit += ({ file, name, });
  return 1;
}  

void remove_current_edit(string file) {
  int i;

  if(!current_edit) current_edit = ({});
  if((i = member_array(file, current_edit)) == -1) return;
  current_edit = current_edit[0..(i-1)]
            + current_edit[(i+2)..(sizeof(current_edit)-1)];
}



 /************************************************************************/
 /* where is file written if wizard goes net-dead during edit */

 /*
 * Give a file name for edit preferences to be saved in.
 */

string get_ed_buffer_save_file_name(string file) {
  string *file_ar;

  if(file && file != "")
  remove_current_edit(((file[0] != '/') ? "/" : "")+ file); 
  file_ar=explode(file,"/");
  file=file_ar[sizeof(file_ar)-1];
  return WIZARD_DIR+(string)this_player()->query_name(1)+"/"+DEAD_ED+"/"+file;
}


 /***************************************************************************/

 /*
 * Give a path to a simul_efun file. Observe that it is a string returned,
 * not an object. But the object has to be loaded here. Return 0 if this
 * feature isn't wanted.
 */

static string get_simul_efun_file() {
  if(catch(call_other(SIMUL_EFUN, "??"))) {
    write("Failed to load " + SIMUL_EFUN + "\n");
    if(catch(call_other(SPARE_SIMUL_EFUN_FILE, "??"))) {
      write("Failed to load spare " + SPARE_SIMUL_EFUN_FILE + "\n");
      shutdown();
      return 0;
    }
    SPARE_SIMUL_EFUN_FILE->start_simul_efun();
    return SPARE_SIMUL_EFUN_FILE;
  }  
 SIMUL_EFUN->start_simul_efun();
 return SIMUL_EFUN;
}

string get_simul_efun() {
  return "/"+ get_simul_efun_file();
  return get_simul_efun_file();
}

 /****************************************************************************/
 /* file_size that can be used by all objects */

int master_file_size(string file) {
  return file_size(file);
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
  if(valid_master_call(previous_object())) return 1;
  if(name && name != "" && name[0] == '/') { 
    name = name[1..(strlen(name)-1)];
  }
  switch(name) {
    case  PLAYER+".c":
    case  WIZARD+".c":
      return 1;
    break;
  }
  write("Invalid Exec() call by object: "+name+"\n");
  return 0;
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

if(this_player()  && environment(this_player())) {
               write_file("/log/HEARTBEAT",
               "Room: "+ file_name(environment(this_player())) +
               "  Time: "+ctime()+
               "\nPlayer: "+this_player()->query_name()+                 
               "\nError: "+ error +
               "  Program: "+ program +
               "\nObject: "+ current_object + 
               "\nLine: "+ line +"\n\n");
  }
  return 0; /* Don't restart */
}


 /************************************************************************/
 /* used by ed() to resolve a full path name */


mixed make_path_absolute(string path) { 
  return (string)this_player()->valid_read(path); 
}


 /*************************************************************************/
 /* validates whether an object can be loaded  */
 /* in native it gives object its uid          */

static mixed master_creator_file(string object_name) {
  string wiz_name, domain, trailer;

  /* directories that should not have objects in them */
  if(sscanf(object_name,"usr/%s", trailer))                       return 0; 
  if(sscanf(object_name,"doc/%s", trailer))                       return 0;
  if(sscanf(object_name,"info/%s", trailer))                      return 0;
  if(sscanf(object_name,"open/%s", trailer))                      return 0;      
  if(sscanf(object_name,"log/%s", trailer))                       return 0;

  if(sscanf(object_name,"cmds/%s/%s",wiz_name,trailer)) {
    return "cmds"+capitalize(wiz_name);
  }

  if(sscanf(object_name,"skills/%s",trailer)) {
    return "skills";
  }

  if(sscanf(object_name,"secure/%s/%s",wiz_name,trailer)) {
    return "secure"+capitalize(wiz_name);
  }

  if(sscanf(object_name,"secure/%s",trailer)) {
    return "secure";
  }

  if(sscanf(object_name,"inherit/%s",trailer)) {
    return "inherit";
  }  
  
  if(sscanf("/"+object_name,WIZARD_DIR+"%s/%s",wiz_name,trailer) == 2) {
    return wiz_name;
  }

  if(sscanf("/"+object_name,DOMAIN_DIR+"%s/%s",domain,trailer) == 2) {
    return domain;
  } 

  if(sscanf(object_name,"open/%s/%s", wiz_name, trailer)) {
    if(wiz_name == "paste") return 0;
    return wiz_name;
  }
  if(sscanf("/"+object_name,WIZARD_DIR+"%s",trailer))             return 0;
  if(sscanf("/"+object_name,DOMAIN_DIR+"%s",trailer))             return 0;

  return 1;  /* else legal */
}


mixed creator_file(string object_name) {
  mixed creator_status;

  if((creator_status = master_creator_file(object_name))) {
//printf("cf:  returning %O\n",creator_status);
    return creator_status;
  }
//printf("cf:  returning %O\n",0);
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

object * _object_stack() {
    int i;
    object *cs;
    i = caller_stack_depth();
    cs = ({ });
    while(i--)
        cs += ({ previous_object(i) });
    return cs;
}

 /******************************************************************/
 /* snoops must routed via simul_efun */

int valid_snoop(object snooper, object snoopee) {
   if(member_array(find_object("/cmds/wiz/lpc"),_object_stack()) != -1) {
      tell_object(snooper,"\n\n*sigh* If you were meant to have snoop then 'snoop' would work.\n\n");
      return 0;
   }
   return 1;
   if(file_name(previous_object()) == get_simul_efun()) return 1;
}



 /****************************************************************/
 /* snoops are usable by priviledged people */

int valid_query_snoop(object wiz) {
  return (int)this_player()->query_security_level() >= SEC5;
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
    switch(what) {
     case "call_out_info":
    case "input_to":
       return (valid_player_call(who)) ? 1 : -1;
     break;


     case "send_imp":
     case "wizlist_info":
       return (valid_player_call(who) || file_name(who) == INETD) ? 1 : -1;
     break;

     case "shutdown":
       return (valid_player_call(who) || file_name(who) == SHUTD) ? 1 : -1;
     break;
     
     case "erq":
     case "attach_erq_daemon":
     case "nomask simul_efun":
     case "set_auto_include_string":
     case "add_worth":
     case "bind_lambda":
     case "get_extra_wizinfo":
     case "rename_object":
     case "set_extra_wizinfo":
     case "set_extra_wizinfo_size":
     case "set_this_object":
     case "shadow_add_action":
       return (valid_master_call(who)) ? 1 : -1;
     break;

     default:
         return -1;
    }
    return -1;
}


void FtpAccess(string host, string message, int port);

void receive_imp(string sender,string msg, int port) {
     if(!msg) return;
//if(find_player("geewhiz")) { tell_object(find_player("geewhiz"),sprintf("\nRIMP: [%s:%d] %-=65O\n\n",sender,port,msg)); }
     if(strlen(msg) > 5 && lower_case(msg)[0..5] == "nftpd\t") {
           return FtpAccess(sender, msg,port);
     }
     INETD->receive_udp(sender,msg,port);
}


void dangling_lfun_closure() {
  raise_error("dangling lfun closure\n");
}


void slow_shut_down(int minutes) {
  write_file("/log/MEMORY",
  sprintf("[%s] __MASTER_OBJECT__->slow_shut_down(%d) called.\n",
        ctime(), minutes));
debug_info(3);
debug_info(4,0);
debug_info(4,"swap");
    SHUTD->shut(minutes);
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



UID_TYPE get_root_uid() { return UID_ROOT; }



 /***********************************************************************/
 /* promote new wizards and domain wizards */

string master_create_wizard(string owner,string domain,object caller) {
  string txt, tmp1, tmp2;
  object access;
  int tmp;

  if(!owner) return 0;
  
  if(!valid_player_call(caller)) {
    write_file("/log/ILLEGALS",
      "Create_wizard() called for owner, "+ owner
     +" by object, "+ file_name(caller) +" at "+ ctime(time())+"\n");
    return 0;
  }
  write_file("/log/WIZ",
    "Name:"+ owner +", By: "+ (string)this_player()->query_name(1)
   +" Time: "+ ctime(time()) +"\n");

  /* promote wizard */
  if(!domain) {
    if(file_size(WIZARD_DIR+ owner) != -2) {
      tell_object(caller,"Adding Wizard Directory...\n");
      mkdir(WIZARD_DIR+ owner); 
    }
    if(file_size(WIZARD_DIR+ owner +"/open") != -2) {
      tell_object(caller,"Adding Open directory, ~ /open\n");
      mkdir(WIZARD_DIR+owner+"/open"); 
    }
    if(file_size(WIZARD_DIR+ owner +"/closed") != -2) {
      tell_object(caller,"Adding Private directory, ~ /closed\n");
      mkdir(WIZARD_DIR+owner+"/private");
    }
    if(file_size(WIZARD_DIR+ owner +"/forest") != -2) {
      tell_object(caller,"Adding Forest directory, ~ /forest\n");
      mkdir(WIZARD_DIR+owner+"/forest");
    }
    if(file_size(WIZARD_DIR +owner+"/"+DEAD_ED) != -2) {
      tell_object(caller,"Adding dead edit directory, ~ /"+DEAD_ED+"\n");
      mkdir(WIZARD_DIR+owner+"/"+DEAD_ED);
    }
/*    if(file_size(INIT_ACCESS) > 0 
    && file_size(WIZARD_DIR+owner+"/access.c") < 0) {  
      tell_object(caller,"Adding Configurable Access Object...\n");
      txt = "#define NAME \""+owner+"\"\n";
      write_file(WIZARD_DIR+owner+"/access.c",txt);
      txt = read_file(INIT_ACCESS); 
      write_file(WIZARD_DIR+owner+"/access.c",txt);
    }
*/
    if(file_size(INIT_WORKROOM) > 0
    && file_size(WIZARD_DIR+owner+"/workroom.c") < 0) {  
      tell_object(caller,"Adding Workroom...\n");
      txt = "#define NAME \""+owner+"\"\n\n";
      write_file(WIZARD_DIR+owner+"/workroom.c",txt);
      txt = read_file(INIT_WORKROOM); 
      write_file(WIZARD_DIR+owner+"/workroom.c",txt);
    }
  }
  else { /* Add wizard to domain */
    if(file_size(DOMAIN_DIR+domain) != -2) {
      if(query_player_level("arch")) { /* only arches can make New Domains */
     write("Adding New Domain...\nMaking "+owner+" the Owner.\n");
     if(this_player() != caller) {
       tell_object(caller,"Adding New Domain...\n"+
                    "You are now owner of "+ domain +".\n"); 
     }
     mkdir(DOMAIN_DIR+ domain);
     txt = read_bytes(DOMAIN_DIR +"access.c",
                               0,
           file_size(DOMAIN_DIR +"access.c"));
     sscanf(txt, "%s});%s", tmp1, tmp2);
     txt = "\""+owner+"\",                     ";
     txt = extract(txt,0,15);
     txt += "\""+domain+"\",\n";
     txt = tmp1+txt +"    });"+ tmp2;
     rm(DOMAIN_DIR +"access.c");
     write_file(DOMAIN_DIR +"access.c",txt);
     if((access = find_object(DOMAIN_DIR +"access"))) {
       destruct(access);
     }
     if(catch(call_other(DOMAIN_DIR +"access","??"))) {
       write("Error In Reloading Head Domain Access Object!\n");
     }
      }
      else {
     write("You cannot add a New Domain.\n");
     return 0;
      }
    }
    if(file_size(DOMAIN_DIR+domain+"/access.c") < 0) {
      txt = read_bytes(DOMAIN_DIR +"access.dom",
                               0,
         file_size(DOMAIN_DIR +"access.dom"));
      write_file(DOMAIN_DIR+domain+"/access.c",txt);
      if(catch(call_other(DOMAIN_DIR+domain+"/access","??"))) {
     write("Error In Reloading Domain Access Object!\n");
      }
    }           
    if(file_size(DOMAIN_DIR+ domain +"/w") != -2) {
      mkdir(DOMAIN_DIR+ domain +"/w");
    }
    if(file_size(DOMAIN_DIR+ domain +"/w/"+ owner) != -2) {
      tell_object(caller,"Adding Domain directory...\n");
      mkdir(DOMAIN_DIR+ domain +"/w/"+ owner);
    }
    if(file_size(DOMAIN_DIR+ domain +"/w/"+ owner +"/access.c") < 0) {  
      tell_object(caller,"Adding Configurable Access Object...\n");
      txt = read_bytes(DOMAIN_DIR +"p_access.dom",
                              0,
         file_size(DOMAIN_DIR +"p_access.dom"));
      write_file(DOMAIN_DIR+domain+"/w/"+owner+"/access.c",txt);
      if(catch(call_other(DOMAIN_DIR+domain+"/w/"+owner+"/access","??"))) {
     write("Error In Reloading Domain Creator Access Object!\n");
      }
    }
  }
}


void make_inn_room(string room){
    string txt;
    if(file_size("/d/inn/code/TEMPLATE.c") > 0
    && file_size("/d/inn/"+room+".c") < 0) {
      txt = read_file("/d/inn/code/TEMPLATE.c");
      write_file("/d/inn/"+room+".c",txt);
      }
}

 /*********************************************/
 /* set up file security for amylaar or mudos */

status restricted_path(string file) {
  string *path;
  int i;
  
  /* these are paths that can only be write-accessed by objects that
   * valid_player_call(object) returns 1.  
   */
   
  path = ({ 
    "/include/%s", "/obj/%s", "/usr/%s", "/secure/%s", "/inherit/%s",
    "/function/%s", "/skills/%s", "/objects/%s",  
  });
#ifdef MASTER_DIR
  path += MASTER_DIR;
#endif /* MASTER_DIR */
  for(i = sizeof(path); i--; ) {
    if(sscanf(file, path[i], file)) return 1;
  }
  return 0;
}



mixed valid_write(string path, 
           string eff_user,
           string call_fun,
          object caller) {

  string domain, who, file, tmp;
  int i;

  if(!caller || stringp(caller)) caller = previous_object();
  if(file_name(caller) == "cmds/wiz/ftp")
        return (path[0..3] == "/ftp") ? path : 0;
  if(this_player() && sscanf(file_name(caller), "cmds/%s", tmp) == 1)
    caller = this_player();

  /* clean up file path */

  if(!path) return 0;
  path = "/"+ path;
  while(sscanf(path,"%s//%s",path,file)) path += "/"+ file;
  if(sscanf(path,"%s..%s",path,file)) return 0; /* illegal path */ 


  /* master objects have access to everything */

  if(valid_master_call(caller)) { 
    return extract(path,1);
  }

  /* validate caller for restricted paths */
  
  if(restricted_path(path) && !valid_player_call(caller)) {
    if(query_player_level("error messages")) {
      write("Invalid Write Access by "+ file_name(caller) 
        +" for path "+ path +",\nEfun: "+ call_fun +"\n");
    }
    return 0;
  }
  
  /* make validations for specific efuns */

  switch(call_fun) {
    case "save_object":
      if(sscanf(path,"/usr/%s",file)) {
     return extract(path,1);
      }
#if defined(MAIL_DIR) && defined(MAILER)
      if(sscanf(path,"/"+ MAIL_DIR +"%s", file)) { /* only mailer here */
     file = file_name(caller);
     sscanf(file,"%s#%d",file,i);
#ifdef INTERMUD
     if(file == "secure/UDP_CMD_DIR/mail") return extract(path,1);
#endif /* INTERMUD */
     return (file == MAILER) ? extract(path,1) : 0;
      }
#endif /* MAILER */

      if(sscanf(path,"/banish/%s",file)) { 
     if(valid_player_call(caller)) return extract(path,1);
      }
      else {
#ifdef PARANOIA
     if(!(who = get_wiz_name(extract(path,1)))) {
       if(!sscanf(path,"/ob_saves/%s",file)) return 0;
       return extract(path,1);
     }
     if(who == get_wiz_name(file_name(caller))) {
       return extract(path,1);
     }
     return 0;
#else
     return extract(path,1);
#endif /* PARANOIA */        
      }
    break;

    case "ed_start":
      if(!valid_player_call(caller)) return 0;
      remove_current_edit(path);
    break;

    case "mkdir":  /* domain stuff */
      if(!valid_player_call(caller)) return 0;

#ifdef NO_MKDIR_IN_DOMAIN_DIR      
      /* Necassary to make sure Access objects are automatically made! */
      if(sscanf(path,DOMAIN_DIR +"%s",domain) == 1) {
     if(sscanf(domain,"%s/%s",domain,file) != 2) {
       write("Use 'domain' command to make a New Domain.\n");
       return 0; /* only create_wizard() fn can add domain creators */
     }
     if(sscanf(path,DOMAIN_DIR +"%s/w/%s",domain,file) ==  2) {
       if(sscanf(file,"%s/%s",who,file) != 2) {
         write("Use 'domain' command to add a New Creator Directory.\n");
         return 0; /* only create_wizard() fn can add domain creators */
       }
     }
      }
#endif

      if(sscanf(path,WIZARD_DIR +"%s", file) == 1) {
     if(sscanf(file,"%s/%s",who,file) != 2) {
       write("Use 'promote' command to add a New Creator Directory.\n");
       return 0; /* only create_wizard() fn can add creators */
     }
      }
    break;

    case "write_file":
      if(sscanf(path,"/log/%s",file)) {
         return extract(path,1);
      }
      if(sscanf(path,"/w/%s/open/%s",file,file) == 2) {
         return extract(path,1);
      }
      if(sscanf(path,"/w/%s/private/logs/%s",file,file) == 2) {
         return extract(path,1);
      }
    case "cindent":
    case "rmdir":
    case "do_rename":
    case "remove_file":
    case "write_bytes":
#ifdef PARANOIA 
     if(get_wiz_name(extract(path,1)) == get_wiz_name(file_name(caller))) {
       break;
     }
     if(!valid_player_call(caller)) return 0;
#endif /* PARANOIA */
    break;
  }

 /* check the original interactive caller's access */
  if(this_interactive()) {
    if(this_interactive() != this_player()) {
      funcall(bind_lambda(#'enable_commands,this_interactive()));
    }
    caller = this_interactive();

    /* validate unique master objects */
    
    if((file = (string)caller->valid_write(path))) {
      for(i = sizeof(MASTER_FILES); i--; ) {
     if(path == MASTER_FILES[i]
     && !this_player()->secure(MASTER_SECURITY)) {
       file = 0;
       break;
     }
      }
    }

    /* validate master directories */
#ifdef MASTER_DIR
    if(file) {
      for(i = sizeof(MASTER_DIR); i--; ) {
     if(sscanf(path,MASTER_DIR[i],who) == 1
     && !this_player()->secure(MASTER_SECURITY)) {
       file = 0;
       break;
     }
      }
    }
#endif /* MASTER_DIR */

    if(!stringp(file)) {
      if(query_player_level("error messages")) {
     write("Invalid Write Access: "+ path +", Efun: "+ call_fun +"\n"); 
      }
      else {
     write("Bad file name.\n");
      }
      return 0;
    }
    return file;
  }
  path = (string)PLAYER->valid_write(path);
  if(stringp(path)) return path;
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

  if(!caller || stringp(caller)) caller = previous_object();
  if(!path) {
    if(call_fun == "ed_start") {
      error = get_error_file((string)this_player()->query_real_name());
      if(!error || error[3]) {
     write("No error.\n");
     return 0;
      }
      write(extract(error[0],1)+" line "+error[1]+": "+error[2]+"\n");
      return error[0];
    }
    return 0;
  }

  path = "/"+ path;
  while(sscanf(path,"%s//%s",path,file)) path += "/"+ file;


  /* master objects have access to everything */

  if(valid_master_call(caller)) {
    return extract(path,1);
  }

  switch(call_fun) {
    case "restore_object": /* we have made the mail reader more secure */
#if defined(MAIL_DIR) && defined(MAILER)
      if(sscanf(path,MAIL_DIR +"%s",file)) {
     file = file_name(caller);
     sscanf(file,"%s#%d",file,i);
#ifdef INTERMUD
     if(file == "secure/UDP_CMD_DIR/mail") return 1;
#endif INTERMUD
     if(file != MAILER) { 
       return 0;
     }
      }
#endif /* MAILER */
      return 1;
    break;

    case "do_rename":
#ifdef INTERMUD
      if(file_name(caller) == INETD) return 1;
#endif
    break;

    case "file_size":
      return extract(path,1); /* allow valid read access to everyone */
    break;

    case "tail":
    case "read_bytes":
    case "read_file":
      if(member_array(path, OPEN_FILES) != -1) {
     return extract(path,1);
      }
    break;

    case "ed_start":
      if(!valid_player_call(caller)) return 0;
      if(!add_current_edit(path)) return 0; /* someone is already editing */
      rm(implode(explode(path,"/")[0..<2],"/")+"/.ed.backup");
      outp = sprintf("Creating backup file...");
      tmp = read_file(path);
      if(!tmp) {
         outp += "No backup made.\n";
      } else {
         tmp = catch(
          write_file((implode(explode(path,"/")[0..<2],"/")+"/.ed.backup"),tmp)
          );
      if(!tmp) outp += sprintf("done.\n");
     }
      printf("%-=75s",outp);
    break;

    case "get_dir":
      if(valid_player_call(caller)) {
     return extract(path,1);
      }
    break;

    case "print_file":
    break;
  }

  if(this_player() && interactive(this_player())) {
    file = (string)this_player()->valid_read(path);
    if(!stringp(file)) {
      if(query_player_level("error messages")) {
     write("Invalid Read Access: "+ path +", Efun: "+ call_fun +"\n"); 
      }
      else {
     write("Bad file name.\n");
      }
      return 0;
    }
    return file;
  }
  path = (string)PLAYER->valid_read(path);
  if(stringp(path)) return path;
  return 0;
}


void inaugurate_master(int arg) {
  mixed *wizinf;
 
    if (!arg)
        set_extra_wizinfo(0, allocate(BACKBONE_WIZINFO_SIZE));
  wizinf = get_extra_wizinfo(0);
  if(arg == 0 || !wizinf[9]) wizinf[9] = time(); 
    if (find_call_out("wiz_decay") < 0)
        call_out("wiz_decay", 3600);
//    if(find_call_out("Garbage_Collection_Scheduled") == -1)
//        call_out("Garbage_Collection_Scheduled",GC_TIME);
 
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

printf("H_LOAD_UIDS called\n");

    set_driver_hook(
      H_LOAD_UIDS,
      unbound_lambda( ({'object_name}), ({ #'creator_file, 'object_name })
/*
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
*/
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
/*
    set_driver_hook(H_MODIFY_COMMAND, ([
      "e":"east", "w":"west", "s":"south", "n":"north",
      "d":"down", "u":"up",
      "nw":"northwest", "ne":"northeast", "sw":"southwest", "se":"southeast",
    ]));
*/ 
    set_driver_hook(H_MODIFY_COMMAND_FNAME,"modify_command");
    set_driver_hook(H_NOTIFY_FAIL, "Eh?\n");
    set_driver_hook(H_INCLUDE_DIRS, map_array(
        define_include_dirs(),
        #'[..<],
        0, 3
    ));

   set_extra_wizinfo(0,wizinf);
}
 


mixed current_time;

string *epilog(int eflag) {
    if (eflag) return ({});
    debug_message(sprintf("Loading init file %s\n", INIT_FILE));
    current_time = rusage();
    current_time = current_time[0] + current_time[1];
    return explode(read_file(INIT_FILE), "\n");
}

void preload(string file) {
    int last_time;
    mixed err;
    if (strlen(file) && file[0] != '#') {
        last_time = current_time;
        debug_message(sprintf("Preloading: %s", file));
        err = catch(call_other(file, ""));
        current_time = rusage();
        current_time = current_time[0] + current_time[1];
        if(err) {
            debug_message(sprintf(" %.2f ... FAILED\n\t%s",(current_time - last_time)/1000.,err));
            return;
        }
        debug_message(sprintf(" %.2f\n", (current_time - last_time)/1000.));
    }
}



mixed wizinfo(mixed arg) { return get_extra_wizinfo(arg); }

mixed master_wizlist_info() { return wizlist_info(); }

int uptime() { return get_extra_wizinfo(0)[9]; }


#define FTPutil "/secure/FTPutil"

int send_imp(string host, int port, string msg) {
     return efun::send_imp(host,port,msg);
}

void FtpAccess(string host, string message, int port)
{
  string *comp;
  string reply, head, tmp;
  string *dir;
  int i;

  comp = explode(message,"\t");
#define FTP_ID   0
#define FTP_SEQ  1
#define FTP_TAG  2
#define FTP_CMD  3
#define FTP_ARG1 4
#define FTP_ARG2 5
#define FTP_ARG3 6


  reply = "INVALID";
  head = sprintf("%s\t%s\tRPLY\t%s\t", comp[FTP_ID], comp[FTP_SEQ], comp[FTP_CMD]);

  switch (lower_case(comp[FTP_CMD]))
  {
    case "ping":
      reply = "OK";
      break;

    case "user":
      if (sizeof(comp) <= FTP_ARG1)
        break;
       catch(i = (int)FTPutil->VerifyName(comp[FTP_ARG1]));
        if(i)
            reply = "/w/"+lower_case(comp[FTP_ARG1]);
        else
            reply = "NONE";
      break;
    
    case "pass":
      if (sizeof(comp) <= FTP_ARG2)
        break;
      catch(i = (int)FTPutil->VerifyPassword(comp[FTP_ARG1],comp[FTP_ARG2]));
      if(i)
        reply = "OK";
      else
        reply = "FAIL";
      break;
    
    case "resv":
      if (sizeof(comp) <= FTP_ARG3)
        break;
      if (comp[FTP_ARG2] == "" 
          || member("~/", comp[FTP_ARG2][0]) == -1)
        comp[FTP_ARG2] = comp[FTP_ARG3]+"/"+comp[FTP_ARG2];
      catch(reply = (string)FTPutil->_get_path(comp[FTP_ARG2], lower_case(comp[FTP_ARG1])));
      break;
    
    case "read":
      if (sizeof(comp) <= FTP_ARG2)
        break;
      catch(i = (int)FTPutil->VerifyRead(comp[FTP_ARG1],comp[FTP_ARG2]));
      if(i)
        reply = "OK";
      else
        reply = "FAIL";
      break;
    
    case "writ":
      if (sizeof(comp) <= FTP_ARG2)
        break;
      catch(i = (int)FTPutil->VerifyWrite(comp[FTP_ARG1],comp[FTP_ARG2]));
      if(i)
        reply = "OK";
      else
        reply = "FAIL";
      break;

    case "list":
      if (sizeof(comp) <= FTP_ARG2)
        break;
      tmp = 0;
      catch(tmp = (string)FTPutil->show_dir(comp[FTP_ARG2], comp[FTP_ARG1]));
      if(tmp) {
          dir = explode(tmp,"\n");
      for (i = 0; i < sizeof(dir); i++)
        send_imp(host, port, head+"LINE\t"+dir[i]+"\r\n");
      reply = "OK";
      }
      break;

    default:
      break;
  }
  send_imp(host, port, head+reply);
}


//     /d/castles/name/0,0,0
//     /w/geewhiz/vocean/upper/mon/xxx

object compile_object(string file) {
    string path, args, virtual_d;
    string *parts;
    int i, k, imax, valid;
    object ob;
    
    if(!file)
        return 0;
    if(file[0] != '/')
        file = "/" + file;
    if(file[<2..<1] == ".c")
        file = file[0..<3];
        
    imax = i = sizeof(parts = explode(file,"/"));
    k = 2;
    valid = 0;
    
    while(i-- && !valid && k < imax) {
        valid = file_size(path = implode(parts[0..<++k],"/")) != -1;
        if(valid) {
            valid = file_size(virtual_d = path + "/virtual_d.c") > 0;
        }
    }

    if(!valid)
        return 0;

    i = sizeof(parts) - k;

    parts = parts[i+1..];

    if(catch(virtual_d->__load()))
        return 0;
    ob = find_object(virtual_d);
       
    return apply(#'call_other,ob,"compile_object",parts);
}
    

//////////////////////////////////////////////////////////////////////////
// Please do not remove this comment block.  Thank you.
//////////////////////////////////////////////////////////////////////////
// $Id: master.c,v 1.1 1998/05/29 00:16:34 atgraham Exp atgraham $
//////////////////////////////////////////////////////////////////////////
// Changelog:
//   $Log: master.c,v $
//   Revision 1.1  1998/05/29 00:16:34  atgraham
//   Initial revision
//
//
//////////////////////////////////////////////////////////////////////////

