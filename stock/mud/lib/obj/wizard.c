  ////////////////////////////////////////////////////////////
  // WIZARD.C         //  Crimsonesti @ // ASH.11.19.98     //
  ////////////////////////////////////////////////////////////

#pragma combine_strings
#include <mudlib.h>
#include <ansi.h>
inherit  PLAYER;
#include <wizard.cfg>   
#include <wizard.spc>
#include <levels.h>  
#include <ansi.h>
#include <wizstat.h>
#include <parse_msg.h>


#define TMP_FILE    make_path("~") + "/_lpc_cmd"
#define invis_name  alt_name /* save some global space by using alt_name */


static object snooped;      /* remember who we are snooping */  

static string current_path; /* Current directory */  

static string *list;

/* used by copy, and some other misc. fn */
static string sformat;
static string sformat_out;
static string path_out;
static status over_write;
static status move_file;

static string msgtrans;
static string msghome;
static string mdest;
static string mclone;


void reset(status arg) {  
  ::reset(arg);
  if(arg) return;  
  if(this_object() != this_player()) return;
  if(!current_path)  current_path = extract(WIZARD_DIR+query_name(1),1);
  if(!msgtrans)     msgtrans = "You are summoned!\n";
  if(!msghome)      msghome = "heads for home";
  if(!mdest) mdest = "$name disintegrates $obj.";
   if(!mclone) mclone = "$name creates $obj.";
  list = ({});
  save_me(0);
  if(security_level < SEC1 ) return;   
/*
  add_action (  "do_call_bin", "do");
*/
  add_action (  "cd",                       "cd"      );  
  add_action (  "ping",                     "ping"    );
  add_action (  "pong",                     "pong"    );
  add_action (  "local_commands",           "localcmd");  
  add_action (  "stat",                     "Stat"    );  
  add_action (  "list_peoples",             "p"       );
  add_action (  "pwd",                      "pwd"     );  
  add_action (  "tail_file",                "tail"    );
  add_action (  "home",                     "home"    );
  add_action (  "church",                   "church"  );
  add_action (  "pub",                      "pub"     );
  add_action (  "inner",                    "inner"   );
  add_action (  "stat_info",                "stat"    );
  add_action (  "renew",                    "new"     );

  if(security_level > SEC1) {
    add_action ("room_fun",           "room");
    add_action ("Put",                "Put");
    add_action ("bomb",               "bomb");
    add_action ("move",               "Get");
    add_action ("clean",              "clean");
    add_action ("goin",               "goin");
    add_action ("reset_ob",           "reset");
    add_action ("Reset_ob",           "Reset");
    add_action ("check_book",         "pay");
    add_action ("grab",               "grab");
    add_action ("duplicate",          "duplicate");
    add_action ("duplicate",          "dup");
    add_action ("set_lighting",       "light");
    add_action ("copy",               "copy");
    add_action ("copy",               "cp"); 
    add_action ("copy_with_overwrite",      "Copy");
    add_action ("move_file_with_overwrite", "Mv");
    add_action ("move_file",                "mv");
    add_action ("Delete",             "Rm");
    add_action ("deltree",            "deltree");
    add_action ("find_file",          "find");
    add_action ("path_tree",          "tree");
    add_action ("Path_tree",          "Tree");
    add_action ("echoto",             "echoto");  
    add_action ("echoall",            "echoall");  
    add_action ("home",               "home");  
    add_action ("edit",               "ed");  
    add_action ("clone",              "clone");  
    add_action ("load",               "load");  
    add_action ("zap",                "zap");  
    add_action ("makedir",            "mkdir");  
    add_action ("show_debug_log",     "debug");  
  }
  if(security_level > SEC2) {
    add_action ("patch", "patch" );
    add_action ("force_player", "command" );
    add_action ("force_player", "force" );
    add_action ("banish", "banish" );
  }
  if(security_level > SEC4) {
    add_action("snoop_on", "snoop");
    add_action("promote", "promote");
  }
}
  

/****************************************************************************/

string query_object_type(){ return "wizard"; }

status room_fun(string str) {
    string current;
    current = "/"+this_player()->query_current_room()+".c";
  write(current+"\n");
  if(!str) write("room <ed/edit/more/cat/stat/update/clean>\n");
  if(str == "edit") this_player()->edit(current);
  if(str == "ed") this_player()->edit(current);
  if(str == "clean") this_player()->clean("room");
  if(str == "stat")  command("stat "+current,this_player());
  if(str == "cat")  command("cat "+current,this_player());
  if(str == "more")  command("more "+current,this_player());
  if(str == "update")  command("ud -s "+current,this_player());
  return 1;


}

/* valid_write is called by edit(), write_file(), write_bytes()  */

string valid_write(string str) { 
  string who, file;  
 
  if(!str) return 0;
  str = make_path(str); 
  if(security_level < SEC1) { return 0; }

  if(sscanf(str, WIZARD_DIR +"%s/open/%s", who, file) == 2) { 
    if(name == who || secure(SEC2)) return extract(str, 1);
  } 
  else if(sscanf(str, WIZARD_DIR +"w/%s/private/%s", who, file) == 2) { 
    if(name == who || secure(SEC7)) return extract(str, 1);
  } 
  else if(sscanf(str, WIZARD_DIR+"%s/%s",who,file) == 2){
    if(name == who || secure(SEC5)) return "w/"+who+"/"+file;
  }
  else if(sscanf(str, DOMAIN_DIR+"%s/%s",who,file) == 2){
    if(name == who || secure(SEC5)) return "d/"+who+"/"+file;
  }
  else if(sscanf(str, "/log/%s/%s", who, file) == 2) { 
    if(who == name || secure(SEC9)) return "log/" + who + "/"+file; 
  } 
  else if(sscanf(str, "/log/%s", who) == 1) { 
    if(who == name || secure(SEC0)) return "log/" + who;  
  } 
  else if(sscanf(str, "/cmds/%s", who) == 1) { 
    if(secure(SEC7)) return "cmds/" + who;  
  } 
  else if(sscanf(str, "/open/%s", file) == 1) { 
    if(secure(SEC0)) return "open/" + file;  
  } 
  else if(sscanf(str, "/w/%s", file) == 1) { 
    if(secure(SEC5)) return "w/" + file;  
  } 
  else if(sscanf(str, "/unique/%s", file) == 1) { 
    if(secure(SEC7)) return "unique/" + file;  
  } 
  else if(sscanf(str, "/g/%s", file) == 1) { 
    if(secure(SEC3)) return "g/" + file;  
  } 
  else if(sscanf(str, "/obj/shadows/%s", file) == 1) {
    if(secure(SEC9)) return "obj/shadows/"+ file;
  }
  else if(sscanf(str, "/obj/%s", file) == 1) { 
    if(secure(SEC9)) return "obj/" + file; 
  } 
  else if (sscanf(str, "/examples/%s", file) == 1){
     if(secure(SEC5)) return "/examples/"+file;
  } 
  else if(sscanf(str, "/log/MAIN/%s", who) == 1) { 
    if(who == name || secure(SEC9)) return "log/MAIN/" + who+ ".o";  
  } 
  else if(sscanf(str, "/doc/help/%s", file) == 1){
    if(secure(SEC5)) return "/doc/help/"+file;
  } 
  else if(sscanf(str, "/doc/%s", file) == 1) { 
    if(secure(SEC5)) return "doc/" + file; 
  } 
  else if(sscanf(str, "/include/%s", file) == 1) { 
    if(secure(SEC9)) return "include/"+file; 
  } 
  else if(sscanf(str, "/inherit/%s", file) == 1) { 
    if(secure(SEC7)) return "inherit/"+file; 
  } 
  else if(sscanf(str, "/objects/%s", file) == 1) { 
    if(secure(SEC5)) return "objects/" + file; 
  } 
  else if(sscanf(str, "/skills/%s", file) == 1) { 
    if(secure(SEC5) ) return "skills/" + file;
  } 
  else if(sscanf(str, "usr/banish/%s", file) == 1){
    if(secure(SEC7)) return "/usr/banish/"+file;

  }
  if(secure(SEC8)) { 
    sscanf(str,"/%s",file); 
    return file;  
  } 
  return 0;  
} 


/* valid_read is called by file_size(), read_file(), read_bytes()       */
/*                         cat(), tail()                                */

string valid_read(string str) { 
  string who, file;  
  string dir;
  if(!str) return 0;
  str = make_path(str); 
  if((file = valid_write(str))) return file;
  if(security_level < SEC1 ) { return 0;}
  if(sscanf(str, "/%s.dat", file) == 1) {
    return file +".dat";
  }
  
  if(sscanf(str, "/dev/%s", file) == 1) { 
    if(secure(SEC10)) return "dev/" + file; 
    return 0; 
  } 
  if(sscanf(str, "/include/%s", file) == 1) { 
    if(secure(SEC4)) return "include/" + file; 
    return 0; 
  } 
  if(sscanf(str, "/log/MAIN/%s", file) == 1) {
    if(secure(SEC9)) return "/log/MAIN/"+file;
   return 0;
  }
  if(sscanf(str, "/log/%s", file) == 1) {
    if(secure(SEC1)) return "/log/"+file;
   return 0;
  }
  if(sscanf(str, "/obj/%s", file) == 1) {
    if(secure(SEC7)) return "obj/"+file;
    return 0;
  }
  if(sscanf(str, WIZARD_DIR +"/w/%s/private/%s", who, file) == 2) {
    if(name == who || secure(SEC7)) return extract(str, 1);
    return 0;
  }
  if(sscanf(str, "/w/%s", file) == 1) {
    if(secure(SEC2)) return "w/"+file;
    return 0;
  }
  if(sscanf(str, "/unique/%s",file) == 1) {
    if(secure(SEC7)) return "objects/"+file;
    return 0;
  }
  if(sscanf(str, "/cmds/admin/%s",file) == 1) {
    if(secure(SEC8)) return "cmds/admin/"+file;
    return 0;
  }
  if(sscanf(str, "/cmds/wiz/%s",file) == 1) {
    if(secure(SEC6)) return "cmds/wiz/"+file;
    return 0;
  }
  if(sscanf(str, "/cmds/std/%s",file) == 1) {
    if(secure(SEC4)) return "cmds/std/"+file;
    return 0;
  }
  if(sscanf(str, "/inherit/%s",file) == 1) {
    if(secure(SEC4)) return "inherit/"+file;
    return 0;
  }
  if(sscanf(str, "/log/%s", who) == 1) { 
    if(who == "log") return "log/log";
    if(secure(SEC5)) return "log/"+ who;
    return 0; /* == valid_write */
  } 
  if(sscanf(str,"/skills/%s",file) == 1) {
    if(secure(SEC3)) return "skills/"+file;
    return 0;
  }
  if(sscanf(str,"/usr/%s",file) == 1) {
    return 0; /* == valid_write */
  }
  if(sscanf(str,"/banish/%s", file) == 1) {
    return 0; /* == valid_write */
  }

/* if you don't want it read, it should be "caught" before here */ 

  if(sscanf(str, "/%s", file)) return file; /* read file */ 
  return 0; /* this should not happen */
}

/**************************************************************************/
/* banish */

status banish(string str) {
  if(!str) {
    notify_fail("Usuage: banish <name>\n");
    return 0;
  }
  if(bad_file(str) || str == "guest") {
    notify_fail("You cannot banish the name \""+ str +"\".\n");
    return 0;
  }
   if(FILE_SIZE("/usr/banish/"+str+".o") > 0){
    write("\""+ str +"\" is already banished.\n");
    cat("/usr/banish/"+str+".o");
    return 1;
  }
  if(secure(SEC5)) {    
    write_file("/usr/banish/"+str+".o",
           ""+this_player()->query_real_name()+
                            " banished this person on : "+ctime()+"\n");
    write("\""+ str +"\" is banished.\n");
  }
  else {
    write("Sorry, You may not banish names.\n");
  }
  return 1;
}

  



/***************************************************************/
/* new */
nomask is_wizard(){ return 1; }

static status renew(string who) {
  object ob;

  if(!who)
    ob = this_object();
  else
    ob = find_player(who);

  if(!ob) {
    write(capitalize(who)+" is not here.\n");
    return 1;
  }
  ob->get_new_player_object();
  return 1;
}




/* query */

string query_path() {  return current_path; }  
int query_wizard()  { return 1; }


status promote(string str) {
  object ob;
  int new_level;
  string level_name;
  if(security_level < 90) return 0;
  if(this_object() != previous_object()){ return 0; }
  if(!str
  || (sscanf(str,"%s %d",str, new_level) != 2
  &&  sscanf(str,"%s %s",str, level_name) != 2)) {

  write("Usage: promote <who> <level|level name>\n");
  write("Notes: promotes a player to security level, <level> or <level name>.\n");
    return 1;
  }
  if(!(ob = present(str,environment()))) {
    write(capitalize(str)+" is not here.\n");
    return 1;
  }
  if(level_name) {
    switch(level_name) {
      case "manager": 
        new_level = SEC9;
      break;
      case "creator":
        new_level = SEC5;
      break;
      case "builder":
        new_level = SEC4;
      break;
      case "student builder":
      case "studen":
        new_level = SEC3;
      break;
      case "retired builder":
      case "retired":
        new_level = SEC2;
      break;
      default:
        promote(0);
        return 1;
      break;
    }
  }
  ob->set_security_level(new_level);
  return 1;
}





/***************************************************************************/
/* echoall */

status echoall(string str) { 
  if(!secure(SEC5)) return 0;
  if(!str) { 
    write("Usage: echoall <string>\n"); 
    return 1; 
  } 
  write("You echo: "+ str +"\n");
  return 1; 
} 


/************************************************************************/
/* echoto */ 
 
status echoto(string str){ 
  object ob; 
  string who, msg; 
  
  if(!secure(SEC3)) return 0;
  if(!str || sscanf(str, "%s %s", who, msg) != 2) { 
    write("Usage: echoto <who> <string>\n"); 
    return 1; 
  } 
  if(!(ob = find_living(lower_case(who)))) {
    write("No player with that name.\n"); 
    return 1; 
  } 
  if((ob->query_edit() || ob->query_invis())
  && (int)ob->query_security_level() > security_level) {
    write("No player with that name.\n"); 
    return 1; 
  } 
//  if(!ob->ansi_on()) 
//    msg = filter_ansi(msg);
//  else
    msg += NORM; /* turn ansi off */

  tell_object(ob, msg + "\n"); 
  write("You echoto "+(string)ob->query_name()+": " + msg + "\n"); 
  return 1; 
} 



/***************************************************************************/
/* home */
  
static status home() {  
  move_player("X#"+ extract(WIZARD_DIR +query_real_name( )+"/workroom",1));
  return 1;  
}  


/****************************************************************************/
/* wizlist */  
  
#if HAVE_WIZLIST != 2
static status wiz_list(string arg) {  
  if(arg)  
    wizlist(arg);  
  else  
    wizlist();  
  return 1;  
}  
#endif /* HAVE_WIZLIST */


/*************************************************************************/
/* local cmds */ 
  
static status local_commands() {  
   localcmd();  
   return 1;  
}  

 
/************************************************************************/
/* list_peoples */
  
static status list_peoples(string arg) { 
  "/cmds/wiz/people"->main(arg);
  return 1;  
} 


/*****************************************************************************/ 
/* change directory */

static status cd(string str) {  
  int len;

  if(!str) str = "~"; /* change to player dir */
  str = make_path(str);
  if(bad_file(str)) {
    write("Illegal filename: "+ str +"\n");
    return 1;
  }
  if(FILE_SIZE(str) != -2) {
    printf("Error: Directory [%s] not found.\n", str);
  }
  else {
    if(str != "/") {
      if(str[(len = strlen(str)-1)] == '/') str = extract(str,0,len-1); 
    }
    sscanf(str,"/%s",current_path);
  }
  tell_object(this_object(),"Current Path: /"+current_path+"\n");
 if(!this_object()->defined_env_var("no_cdmessage"))
  cat(sprintf("/%s/.readme",current_path));
  return 1;
}


/**************************************************************************/
/* tail */
  
status tail_file(string path){  
  if(!path) return 0;
  path = make_path(path);  
  if(bad_file(path)) {
    write("Illegal filename: "+ path +"\n");
    return 1;
  }
  if(!tail(path)) return 0;  
  return 1;  
}  


/*****************************************************************************/ 
/* print working directory */

status pwd() {  
   tell_object(this_object(), "Current path: /" + current_path + "\n");  
   return 1;  
}  


/***************************************************************************/  

status inner() {
  move_player("X#"+ INNER);
  return 1;
}


/***************************************************************************/
/* edit */
  
void rm_ed_tbl() {
  string file, *table;
  int i;

  table = (string *)MASTER->query_current_edit();
  if(!table) table = ({});
  if((i = member_array(capitalize(query_name(1)),table)) == -1) return;
  file = table[i-1];
  MASTER->remove_current_edit(file);
}


status ed_tbl(string file) {
  int i;
  string *table;

  table = (string *)MASTER->query_current_edit();
  if(!table) table = ({});
  if((i = member_array(file,table)) == -1) return 0;
  write("Sorry, "+file+" is being edited by "+table[i+1]+".\n");
  return 1;
}


static status edit(string file){  
  string tmp_file;  
  if(this_player() != this_interactive()) return 1;

  if(!file) {  
    ed();    
    return 1;  
  }  
  if(!(file = valid_write(file))) {
    write("Sorry, Access denied.\n");  
    return 1;  
  }  
  if(bad_file(file)) {
    write("Illegal filename: "+ file +"\n");
    return 1;
  }
  if(ed_tbl("/"+ file)) return 1;
  ed(file,"rm_ed_tbl");  
  return 1;  
}  


/******************************************************************************/
/* load - accepts wildcards */

static status load(string str) {  
  object ob;
  string  *files, file, format, path;
  int i, k;
  status flag;
   
  if(!str) {  
    tell_object(this_object(), "Usage: load <file>\n");  
    return 1;  
  }  
  sscanf(str, "%s.c", str);
  str = make_path(str);
  if(bad_file(str)) {
    write("Illegal filename: "+ str +"\n");
    return 1;
  }
  for(i = strlen(str)-1; i && str[i] != '/'; i--);
  path = extract(str,0,i);
  file = extract(str,i+1);
#ifdef NO_GET_DIR_WILDCARDS
  format = wildcard_format(file+".c");
  files = (format_number(format)) ? get_dir(path +".") : get_dir(str +".c");
#else
  files = get_dir(str +".c");
#endif
  sscanf(path,"/%s",path);
  for(k = 0; k < sizeof(files); k++) {
#ifdef NO_GET_DIR_WILDCARDS
    if(!(file = match_format(files[k],format,format))) {
      continue;
    }
    file = path + file;
#else
    file = path + files[k];
#endif
    ob = find_object(file);
    if(ob) {
      tell_object(this_object(),"Unloading old copy "+ file +"\n");
      destruct(ob);
    }
    call_other(file,"??"); /* failure to load stops loading process */
    tell_object(this_object(), "Loaded: "+ file +"\n");  
    flag = 1;
  }
  if(!flag) {
    tell_object(this_object(),"Error: No match for "+str+".c\n");
    return 1;
  }
  return 1;
}


/**************************************************************************/
/* clone */

static status clone(string file) {
  object ob;

  if(!file) {
    write("Usage: clone <filename>\n");
    return 1;
  }
  file = make_path(file);
  if(bad_file(file)) {
    write("Illegal filename: "+ file +"\n");
    return 1;
  }
  sscanf(file,"%s.c",file);
  sscanf(file,"/%s",file);
  if(FILE_SIZE("/"+file+".c") < 0 && FILE_SIZE("/"+file) < 0) {
    tell_object(this_object(),"File: "+file+".c does not exist.\n");
    return 1;
  }
  ob = clone_object(file);
  write("Cloning: "+file+"\n");
  if(ob->get()) {
    if(!ob) {
      write("Object destructed itself after call to get()!!\n");
      return 1;
    }
#ifdef NATIVE_MODE
    ob->move(this_object());
#else
    move_object(ob,this_object());
#endif /* NATIVE_MODE */
    if(!ob) {
      write("Object destructed itself after it move_object()!!\n");
      return 1;
    }
    add_weight((int)ob->query_weight());          
    write("Object moved to you.\n");
  }
  else {
#ifdef NATIVE_MODE
    ob->move(environment());
#else
    move_object(ob,environment());
#endif /* NATIVE_MODE */
    if(!ob) {
      write("Object destructed itself after it move_object()!!\n");
      return 1;
    }
    write("Object moved to Room.\n");
  }
  if(!mclone) mclone = "$name creates $obj";
  say(parse_msg(mclone,ob)+"\n", this_object());  
  return 1;
}

/***************************************************************************/
/* remove log function rlog */
 
static status rlog() {
  if(rm("/log/"+query_name(1))) 
    write("Removed Error Log.\n");
  else
    write("Failed to remove error log.\n");
  return 1;
}

/**************************************************************************/
    /*  Wizard Check Book */

status check_book(mixed str) {
  string name;
  int amount;
  object ob;
  if(!str) {
    write("Usage: pay <amount> to <who>\n");
    return 1;
  }
  if(sscanf(str, "%d to %s", amount, name) == 2) {
    if(!(ob = present(name, environment()))) {
      tell_object(this_object(), name+" is not present.\n");
      return 1;
    }
    if(amount < 1) {
      tell_object(this_object(), amount+" coins is an invalid amount.\n");
      return 1;
    }
    ob->add_money(amount);
    say(this_player()->query_name()+
      " pulls out a check book and writes out a check to "+
      capitalize(name)+".\n",ob);
    tell_object(ob, query_name()+
      " pulls out a check book and writes you a check for "+ amount+
      " coins.\n");
    write("You write out a check for "+amount+" coins and hand it to "+
      capitalize(name)+".\n");
    return 1;
  }
  check_book(0);
  return 1;
}
/**************************************************************************/


/**************************************************************************/
/* debug log */

static status show_debug_log() {
  tail(DEBUG_LOG);
  return 1;
}
  
  
/*****************************************************************************/
/* mkdir */
  
static status makedir(string str) {  
  str = make_path(str);
  if(bad_file(str)) {
    write("Illegal filename: "+ str +"\n");
    return 1;
  }
  if(FILE_SIZE(str) == -2) {
    tell_object(this_object(),"Error: Directory already exists.\n");
  }
  else if(mkdir(str))  
    write("New dir: "+ str +".\n");  
  else  
    write("Error: Cannot make dir "+ str +"\n");  
  return 1;  
}  
  
/**************************************************************************/
/* snoop */
  
static status snoop_on(string str){  
  object ob;  
     
  if(!str) {  
    snoop();  
    if(snooped) {
      if(!secure(SEC5)) {  
      write("Access Denied\n"); return 1; 

      }
#     ifdef LOG_SNOOPERS  
        log(LOG_SNOOPERS,"Stopped snooping "+snooped->query_name()+".\n");  
#     endif /* LOG_SNOOPERS */
      snooped=0;  
    }  
    return 1;  
  }  
  if(!secure(SEC5)) return 0;  
  if(!(ob = find_player(str))) {  
    write("Could not find "+capitalize(str)+".\n");  
    return 1;  
  }  
  if((int)ob -> query_security_level() > security_level ) { 
    write("Error: Higher security level needed\n"); 
    return 1; 
  } 
  if(snooped) snoop_on(0);
  if(!secure(SEC5)) {
    tell_object(ob, query_name()+" is now watching your every move.\n");  
  }
  snoop(ob);  
  snooped = ob;
# ifdef LOG_SNOOPERS  
    log(LOG_SNOOPERS,"Started snooping "+snooped->query_name()+".\n");  
# endif /* LOG_SNOOPERS */
  return 1;  
}  

  
/*************************************************************************/
/* force */

static status force_player(string str) {  
  string who, what;  
  object ob;  

  if(!secure(SEC4)) return 0;
  if(!str) {
    write("Usage: command <who> <cmd>\n");
    return 1;
  }  
  if(sscanf(str, "%s to %s", who, what) == 2 ||  
        sscanf(str, "%s %s", who, what) == 2) {  
    if(!(ob = find_living(who))) {  
      tell_object(this_object(), "No such player.\n");  
      return 1;  
    }  
    if((int)ob->query_security_level() > security_level) {  
      tell_object(this_object(), 
        "You can't force a higher creator than you!\n");  
      return 1;  
    }  
    tell_object(ob, query_name()+" commands you to: "+what+"\n");  
    tell_object(ob, "You feel the need to obey.\n");
    ob->init_command(what);
/*
    command(what, ob);  
*/
  if(this_object()->query_real_name() != "Ash")
  write_file("/log/MAIN/FORCE", name +" "+who+" "+what+" "+ctime()+"\n");
    tell_object(this_object(), "You FORCE "+capitalize(who)+" to "+what+"\n"); 
    return 1;  
  }  
  return 0;  
}  
  
  
/***************************************************************************/
/* zap */
  
static status zap(string str){  
  object ob;  

  if(!str)  
    ob = primary_attack;  
  else  
    ob = present(lower_case(str), environment());  
  if(!ob || !living(ob)) {  
    tell_object(this_object(), "Zap whom?\n");  
    return 1;  
  }  
  tell_object(this_object(), "You summon a flash of lightning...\n");
  say(query_name() +" summons lightning from the heavens...\n");
  ob->hit_player(100000);
  return 1;  
}  


/**************************************************************************/
/* show stats */
  
static status stat(string str) {  
  object ob;  
     
  if(!str) {
    write("Usage: stat <who>\n");
    return 1;
  }
  ob = present(str, environment());
  if(!ob || !living(ob)) 
  if(!(ob = find_living(lower_case(str)))) {
    write("No such person is playing now.\n");  
    return 1;  
  }  
  if(ob->query_invis() && (int)ob->query_security_level() > security_level) { 
    write("No such person is playing now.\n");  
    return 1;  
  }  
  ob->show_stats();  
  return 1;  
}  
  
/****************************************************************************/
/* rm */

static status Delete(string str) {
  delete(str,1);
  return 1;
}

#if defined(AMYLAAR) || defined(MUDOS_DR)

status filter_ftime(string file, mixed *info) {
  if(file == ".." || file == ".") return 0;
  if(file_time(info[0] + file) < info[1]) return 1;
  return 0;
}

#endif


varargs static status delete(string str, status prompt_off) {
  int date, year, ftime;
  string month;
  int i;
  string tmp1, tmp2;
  string path, file;

  if(!str) {
    write("Usage: rm <file> { options: <-a> }\n\n"+
          "Notes: <-a> delete all files without prompting\n"+
#if defined(AMYLAAR) || defined(MUDOS_DR) 
          "       -t<date>:<month>:<year> <file> deletes file before date\n"+
#endif /* AMYLAAR */
          "       Rm == rm -a\n"+
          "       wildcard \"*\" accepted\n");
    return 1;
  }

#if defined(AMYLAAR) || defined(MUDOS_DR)
  /* scan for -t switch */

  if(sscanf(str,"-t%d:%s:%d %s", date, month, year, str) == 4) {
    if((ftime = rtime("xxx "+month+" "+date+" 00:00:00 "+ year)) == -1) {
      write("-t time format, date:month:year\n");
      return 1;
    }
    write("Deleting Files before "+ date +" "+ month +" "+ year +"\n");
  }
#endif /* AMYLAAR */

  /* scan for -a switch */

  if(sscanf(str,"%s-a%s", tmp1, tmp2)) {
    prompt_off = 1;
    str = tmp1;
    if(str != "") {
      while(str[strlen(str)-1] == ' ') {
        str = extract(str,0,strlen(str)-2);
      }
    }
  }

  /* has wizard|player got write access */

  if(!(str = valid_write(str))) {
    write("Invalid delete access.\n");
    return 1;
  }
  str = "/" + str;
  if(bad_file(str)) {
    write("Illegal filename: "+ str +"\n");
    return 1;
  }

  /* extract path and filename */
 
  for(i = strlen(str) - 1; (str[i] != '/' && i >= 0); i--);
  new_text = extract(str,0,i);
  file = extract(str,i+1,strlen(str));
  
  if(!file || file == "") {
    write("Invalid filename: "+str+"\n");
    new_text = 0;
    return 1;
  }
  if(file_size(extract(new_text,0,strlen(new_text)-2)) != -2) {
    write("Directory: "+new_text+" does not exist.\n");
    new_text = 0;
    return 1;
  }
#ifdef NO_GET_DIR_WILDCARDS
  sformat = wildcard_format(file);
  list = (format_number(sformat)) ? get_dir(new_text +".") : get_dir(str);
#else
  list = get_dir(new_text + file);
#endif

#if defined(AMYLAAR) || defined(MUDOS_DR)
  if(ftime) {
    list = filter_array(list,"filter_ftime",this_object(),({new_text,ftime}));
  }
#endif

  if(!sizeof(list)) {
    write("No match found for "+str+"\n");
    return 1;
  }
  line = 0;

  while(1) {
    if(list[line] == "." || list[line] == ".." 
#ifdef NO_GET_DIR_WILDCARDS
    || !match_format(list[line],sformat,sformat)
#endif
    || file_size(new_text+list[line]) < 0) {
      line++;
      if(line == sizeof(list)) {
        write("No File matches found for "+str+"\n"+
              "Use 'rmdir' to remove directories.\n");
        return 1;
      }
      continue;
    }
    break;
  }

  if(prompt_off) {
    delete_all_files("y");
  }
  else {
    write("Remove "+new_text+list[line]+" (y/n/q) [n]: ");
    input_to("delete_files_prompt");
  }
  return 1;
}

static void delete_files_prompt(string answer) {
  if(answer == "q") {
    write("Ok.\n");
    return;
  }
  if(answer == "y") {
    if(line < sizeof(list)) {
      if(!rm(new_text + list[line])) {
        write("Failed to delete: ");
      }
      else { 
        write("Deleted: ");
      }
      write(new_text+list[line]+"\n");
    }
  }

  /* skip over directories */

  if(++line < sizeof(list)) {
    while(1) {
      if(list[line] == "." || list[line] == ".." 
#ifdef NO_GET_DIR_WILDCARDS
      || !match_format(list[line],sformat,sformat)
#endif
      || file_size(new_text+list[line]) < 0) {
        line++;
        if(line == sizeof(list)) {
          write("Ok.\n");
          return;
        }
        continue;
      }
      break;
    }
  }

  if(line < sizeof(list)) {
    write("Remove "+new_text + list[line]+" (y/n/q) [n]: ");
    input_to("delete_files_prompt");
  }
  else {
    write("Ok.\n");
  }
}


static void delete_all_files(string answer) {
  int start, skips;

  if(answer == "n") {
    write("Ok.\n");
    return;
  }
  
  for(start = line; line < sizeof(list); line++) {
    if(list[line] == "." || list[line] == ".." 
#ifdef NO_GET_DIR_WILDCARDS
    || !match_format(list[line],sformat,sformat)
#endif
    || file_size(new_text+list[line]) < 0) {
      if(++skips > 30) break; /* mainly for NO_GET_DIR_WILDCARDS */
      line++;
      if(line == sizeof(list)) {
        write("Ok.\n");
        return;
      }
      continue;
    }
    if(!rm(new_text+list[line])) {
      write("Failed to delete: ");
    }
    else { 
      write("Deleted: ");
    }
    write(new_text+list[line]+"\n");
    skips = 0;
    if(line - start > 20) break;
  }
  if(line != sizeof(list)) {
    write("Continue Deleting (y/n) [y]: ");
    input_to("delete_all_files");
    line -= 1;
  }
  else {
    write("Ok.\n");
  }
}
  
static void print_files(string answer) {
  string name; 
  int i, finish, columns;  
  
  if(answer == "n" || answer == "q") {
    write("Ok.\n");
    return;
  }
  sscanf(answer,"%d",line); 
#ifdef MSDOS
  if(line >= sizeof(list)) line = sizeof(list) - 40; 
  if(line < 0) line = 0;
  finish = line + 40; 
#else
  if(line >= sizeof(list)) line = sizeof(list) - 52; 
  if(line < 0) line = 0;
  finish = line + 52; 
#endif /* MSDOS */
  if(finish > sizeof(list)) finish = sizeof(list); 
 
  write("Current Path: /"+((current_path == "") ? "\n" : current_path+"/\n")); 
  if(new_text != "/" + current_path + "/") {
    if(current_path != "" || (current_path == "" && new_text != "/")) {
      write("     ls Path: "+ new_text +"\n");
    }
  }
  write("\n"); 
  for(i = line; i < finish; i++) { 
    if(list[i] == "." || list[i] == ".."
#ifdef NO_GET_DIR_WILDCARDS
     || !match_format(list[i],sformat,sformat)
#endif
    ) {
      finish++;
      if(finish > sizeof(list)) finish = sizeof(list); 
      continue;
    }
    if(!valid_read(new_text + list[i])) { 
      name = "?????                                            "; 
    } 
    else if(file_size(new_text + list[i]) == -2) {  
      if(!valid_read(new_text+list[i]+"/"))
        name = "?????/                                              ";
      else
        name = list[i] + "/                                    ";  
    }
    else { 
      if(!find_object(new_text+list[i])) 
        name = list[i] + "                                   ";  
      else 
        name = "*"+list[i]+"                                    "; 
    } 

    name = extract(name,0,16);
    if(++columns > 4) {
      columns = 1;
      write("\n");  
    }
    write(name);
  }  
  if(finish != sizeof(list)) {
    line = finish;
    write("\n("+(line+1)+"-"+finish+")/"+sizeof(list)+" Files\n");
    write("\nContinue (y/n/#num) [y]: ");
    input_to("print_files");
    return;
  } 
  write("\nOk.\n");  
}  


static void print_verbose(string answer) {
  string name, date; 
  int i, finish;  
  int f_size;

  if(answer == "n") {
    write("Ok.\n");
    return;
  }
  sscanf(answer,"%d",line); 

  if(line >= sizeof(list)) line = sizeof(list) - 16; 
  if(line < 0) line = 0;
  finish = line + 16; 
  if(finish > sizeof(list)) finish = sizeof(list); 
 
  write("Current Path: /"+((current_path == "") ? "\n" : current_path+"/\n")); 
  if(new_text != "/" + current_path + "/") {
    if(current_path != "" || (current_path == "" && new_text != "/")) {
      write("     ls Path: "+ new_text +"\n");
    }
  }
  write("\n"); 
  for(i = line; i < finish; i++) { 
    if(list[i] == "." || list[i] == ".."
#ifdef NO_GET_DIR_WILDCARDS
     || !match_format(list[i],sformat,sformat)
#endif
    ) {
      finish++;
      if(finish > sizeof(list)) finish = sizeof(list); 
      continue;
    }
    if(!valid_read(new_text + list[i])) { 
      name = "?????                                              ";
      name = extract(name,0,20) + " Size: Unknown                       ";
      name = extract(name,0,40) + " Access: None\n";
    } 
    else if ((f_size = file_size(new_text + list[i])) == -2) {  
      if(!valid_read(new_text+list[i]+"/")) {
        name = "?????/                                           ";
        name = extract(name,0,20)+" Size: Unknown                       ";
        name = extract(name,0,40)+" Access: None\n";
      }
      else {
        string *tmp_dir;

        name = list[i] + "/                                    ";  
        name = extract(name,0,20) + " Size: ";
        tmp_dir = get_dir(new_text+list[i]+"/.");
        if(!tmp_dir) tmp_dir = ({});
        name += sizeof(tmp_dir) + " Files\n";
      }
    }
    else { 
      if(!find_object(new_text+list[i])) 
        name = list[i] + "                                   ";  
      else 
        name = "*"+list[i]+"                                    "; 
      name = extract(name,0,20) + " Size: "+f_size+" bytes                  ";
      if(valid_write(new_text+list[i])) {
        name = extract(name,0,40) + " Access: R/W";
#if defined(AMYLAAR) || defined(MUDOS_DR)
        name += "  Date: "+ DATE(file_time(new_text+list[i]))+"            ";
#endif
      }
      else {
        name = extract(name,0,40) + " Access: Read";
#if defined(AMYLAAR) || defined(MUDOS_DR)
        name += " Date: "+ctime(file_time(new_text+list[i]))+"            ";
#endif
      }
      name = extract(name,0,78);
      name += "\n";
    }
    write(name);
  }  
  if(finish != sizeof(list)) {
    line = finish;
    write("\n("+(line+1)+"-"+finish+")/"+sizeof(list)+" Files\n");
    write("Continue (y/n/#num) [y]: ");
    input_to("print_verbose");
    return;
  } 
  write("Ok.\n");  
}


/***************************************************************************/
/* duplicate: copy another object, this will not set values in that object */

static status duplicate(string str) {
  int i, j, clone_number;
  string what, where, file;
  object *ob, *dest;
  object cl;

  if(!str) {
    write("Usage: Dup <object> {clone it to yourself}\n"+
          "       Dup <object> to <object2>\n"+
          "       Dup <object::object2>\n\n"+
          "Notes: Dup <file.c>  can clone a file\n"+
          "       Duplicate makes clones of existing objects, note that it\n"+
          "       does not set values in an object.\n");
    return 1;
  }
  if(!sscanf(str,"%s to %s", what, where)) { 
    if(!sscanf(str,"%s::%s", what, where)) { 
      what = str;
      where = "me";
    }
  }
  if(!sizeof((dest=PROCESS_ARG(where)))) return 1;
  if(!sizeof((ob=PROCESS_ARG(what)))) return 1;
  for(i = 0; i < sizeof(ob); i++) {
    if(!sscanf(file_name(ob[i]),"%s#%d", file, clone_number)) 
      file = file_name(ob[i]); 
    for(j = 0; j < sizeof(dest); j++) {
      if(catch(cl = clone_object(file))) {
        write("Error in cloning: "+file+"\n");
        continue;
      }
#ifdef NATIVE_MODE
      cl->move(dest[j]);
#else
      move_object(cl, dest[j]);
#endif /* NATIVE_MODE */
      write("Duplicated: "+file+", moved to "+dest[i]->short()+"\n");
    }
  }
  return 1;
}


/*****************************************************************************/
/* clean: destruct all objects in an object, except those in the 'keep' list */
/*        This will not destruct interactive players */

static status clean(string str) {
  status kflag;
  object *ob, *inv;
  string name, filename;
  int i, j, k, l, count;

  if(!str) {
    write("Usage: clean <object>\n\n"+
          "Notes: Destructs inventory of <object> except for interactive\n"+
          "       players, and objects with id \"guild_object\", \"soul\",\n"+
          "       or "+query_name(1)+"\n");
    return 1;
  }
  if(!sizeof((ob = PROCESS_ARG(str)))) return 1;

  /* hmm.....'clean users' ;) */

  for(l = 0; l < sizeof(ob); l++) {  
    while(sizeof((inv = all_inventory(ob[l])))) {
      if((count++) > 4) break; /* object depth of destruction */
      for(i = 0; i < sizeof(inv); i++){
        for(j = 0; j < sizeof(KEEP); j++) if(inv[i]->id(KEEP[j])) kflag = 1;
        if(!kflag){
          if(living(inv[i]) && interactive(inv[i])) continue;
          name = (inv[i]->short()) ? (string)inv[i]->short()
                                   : "Invisible Object";
          filename = file_name(inv[i]);
          write("Destructing->"+filename+"<->"+name+".\n");
          destruct(inv[i]);
        }
        kflag = 0;
      }        
    }
    if(ob[l]->query_name())
      write((string)ob[l]->query_name()+" has been cleaned.\n");
    else
      write((string)ob[l]->short()+" has been cleaned.\n");
  }
  return 1;
}


/***************************************************************************/
/* bomb: get rid of all objects in a room except players */

static status bomb(){
 clean("room");
 return 1;
}


/***************************************************************************/
/* goin: go into an object */

static status goin(string str) {
  object *ob;
  if(!str){
    write("Usage: Goin <object>\n\n"+
          "Notes: Go into an object, gets upset if object is inside you.\n"+
          "       If it does try 'Put me::<object>'\n");
    return 1;
  }
  if(!sizeof((ob = PROCESS_ARG(str)))) return 1;
  if(sizeof(ob) > 1) {
    write("You cannot go into an object list, using first element.\n");
  }
  if(ob[0] != this_player()) {
    if(environment(ob[0]) == this_player()) {
      write("You cannot go into an object in your inventory.\n");
      return 1;
    }
#ifdef NATIVE_MODE
    this_player()->move(ob[0]);
#else
    move_object(this_player(),ob[0]);
#endif /* NATIVE_MODE */

#ifdef NATIVE_MODE
    this_player()->init_command("look");
#else
    command("look", this_player());
#endif /* NATIVE_MODE */
  }
  else {
   write("You cannot go into yourself!\n");
  }
  return 1;
}

/****************************************************************************/
/* put: move objects about */

/* static */ status Put(string str){
  int i;
  object *ob1, *ob2;
  string what, who, short_desc;

  if(!str){
    write("Usage: put <object> in <where> (may cause ambiguitites)\n"+
          "       put <object::where>\n\n"+
          "Notes: Reposition objects. The first variation is for the sake\n"+
          "       of compatability with the original 'put', but it can be\n"+
          "       ambiguous. The second format eliminates that.\n");
  
    return 1;
  }
  if(!sscanf(str,"%s::%s",what,who)) {
    if(!sscanf(str,"%s in %s",what,who)){
      Put(0);
      return 1;
    }
  }
  if(!sizeof((ob1 = PROCESS_ARG(what)))) return 1;
  if(!sizeof((ob2 = PROCESS_ARG(who)))) return 1;
  if(sizeof(ob2) > 1){
    write("You cannot put into an object list, using first element.\n");
  }
  for(i = 0; i < sizeof(ob1); i++) {
    if(environment(ob2[0]) == ob1[i]){
      if(environment(ob1[i])) {
#ifdef NATIVE_MODE
        ob2[0]->move(environment(ob1[i]));
#else
        move_object(ob2[0], environment(ob1[i]));
#endif /* NATIVE_MODE */
      }
      else{
        write("You cannot move an object into itself!\n");
        return 1;
      }
    }  
    if(ob1[i] != ob2[0]) {
#ifdef NATIVE_MODE
       ob1[i]->move(ob2[0]);
#else
       move_object(ob1[i],ob2[0]);
#endif /* NATIVE_MODE */
       if(!ob1[i]) { /* This has happen, how unusual */
         write("Object has destructed itself, on moving.\n");
         continue;
       }
       short_desc = (ob1[i]->short()) ? (string)ob1[i]->short()
                                      : file_name(ob1[i]);
       write(capitalize(short_desc)+" has been moved into "+
             ob2[0]->short()+".\n");
    }
  }
  return 1;
}


/***************************************************************************/
/* get: move objects from anywhere into yourself */

static status move(string str) {
  int i;
  object *ob;
  if(!str){
     write("Usage: Get <object>\n\n"+
           "Notes: Get any object, no get() or add_weight() checks.\n"+
           "       An alternative to 'put <object>::me'\n"); 
     return 1;
  }
  if(!sizeof((ob = PROCESS_ARG(str)))) return 1;
  for(i = 0; i < sizeof(ob); i++){
    string short_desc;
#ifdef NATIVE_MODE
     ob[i]->move(this_player());
#else
     move_object(ob[i],this_player());
#endif /* NATIVE_MODE */
     short_desc = (ob[i]->short()) ? (string)ob[i]->short()
                                   : file_name(ob[i]);     
     write("Getting: "+short_desc+"\n");
  }
  write("Ok.\n");
  return 1;
}


/***************************************************************************/



string string_results(mixed arg) {
  string str;
  string tmp;
  string result;
  int i;
  mixed *indices;

  result = "";
  if(pointerp(arg)) {
    result += "({";
    for(i = 0; i < sizeof(arg); i++) {
      if(!i) result += " ";
      result += string_results(arg[i]) +", ";
    }
    result += "})";
  }
  else if(mappingp(arg)) {
    str = "([\n";
    indices = m_indices(arg);
    for(i = 0; i < sizeof(indices); i++) {
      str += "  "+ string_results(indices[i]) +": "+
                   string_results(arg[indices[i]]) +",\n";
    }
    result = str +"])";
  }
  else if(objectp(arg)) {
    result = file_name(arg);
  }
  else if(stringp(arg)) {
    str = arg;
    result = "";
    while(sscanf(str,"%s\n%s", tmp, str)) {
      if(arg != tmp +"\n"+ str) result += "\n";
      result += tmp +"\\n";
    }
    result = "\""+ result + str +"\"";
  }
  else if(intp(arg)) {
    result = arg +"";
  }
  return result;
}
void show_results(mixed arg) {
  tell_object(this_object(),(((pointerp(arg)) ? "<Array>\n"
        :(objectp(arg))  ? "<Object>\n"
        :(intp(arg))     ? "<Int>\n"
        :(stringp(arg))  ? "<String>\n"
        :(mappingp(arg))  ? "<Mapping>\n"
        : "<unknown type>\n")));
  tell_object(this_object(),string_results(arg)+"\n");
}


status patch(string str) {
  mixed last_arg;
  string type_arg;
  object *ob;
  string who, do_str, arg1, arg2;
  int i, iarg1, iarg2;
  status use_int1, use_int2, load;
  if(this_player() != this_interactive()) return 0;
  if(!SEC3){ write("This command is limited to full Immortals.\n"); return 1; }
  if(!str){
    write("Usage: Patch <object::function>\n"+
          "       Patch <object::function::arg1>\n"+
          "       Patch <object::function::arg1::arg2>\n\n"+
          "Notes: Calls to fn, 'function' in <object>. Function can be\n"+
          "       called with up to 2 arguments. Patch <object> attempts\n"
          "       to load the object.\n");
    return 1;
  }
  if(sscanf(str,"%s::%s::%s::%s",who, do_str, arg1, arg2) == 4){
    if(sscanf(arg1,"%d",iarg1) == 1) use_int1 = 1;
    if(sscanf(arg2,"%d",iarg2) == 1) use_int2 = 1;
  }
  else if(sscanf(str,"%s::%s::%s", who, do_str, arg1) == 3) {
    if(sscanf(arg1,"%d",iarg1) == 1) use_int1 = 1;
  }
  else if(!sscanf(str,"%s::%s", who, do_str)) {
    who = str;
    load = 1;
  }
  if(!sizeof((ob = PROCESS_ARG(who)))) return 1;
  for(i = 0; i < sizeof(ob); i++) {
    if(!secure(SEC9) && MASTER->valid_master_call(ob[i])) {
      write("Invalid Call to "+ file_name(ob[i]) +"\n");
      continue;
    }
   write("\nObject: "+file_name(ob[i])+"\n");
    if(use_int1 && use_int2)
      last_arg = (mixed)call_other(ob[i],do_str,iarg1,iarg2);
    else if(use_int1 && arg2)
      last_arg = (mixed)call_other(ob[i],do_str,iarg1,arg2);
    else if(arg1 && use_int2)
      last_arg = (mixed)call_other(ob[i],do_str,arg1,iarg2);
    else if(use_int1)
      last_arg = (mixed)call_other(ob[i],do_str,iarg1);
    else if(arg1)
      last_arg = (mixed)call_other(ob[i],do_str,arg1);
    else if(do_str)
      last_arg = (mixed)call_other(ob[i],do_str);
    tell_object(this_object(),"Results :"+show_results(last_arg)+"\n");
    if(!ob[i]) {
    tell_object(this_object(),"Object destructed itself!\n");
    }
    else if(!(last_arg || load)) {
      string sh;
      sh = (ob[i]->short()) ? (string)ob[i]->short()+", "+file_name(ob[i])
                            : "/" + file_name(ob[i]);
      if(function_exists(do_str, ob[i])) {
     tell_object(this_object(),"Function: \""+do_str+"\" exists in "+sh+"\n");
      }
      else {
       tell_object(this_object(),
       "Function: \""+do_str+"\" does not exist in "+sh+"\n");
      }
    }
  }
  return 1;
}




/***************************************************************************/
/* Reset: do a reset(0) in an object */

varargs static status Reset_ob(string arg, status reset_arg){
  int i;
  object *ob;
  string name, filename;

  if(!arg){
    write("Usage: reset <object> (normal reset)\n"+
          "       Reset <object> (full reset)\n\n"+
          "Notes: calls reset() in <object>\n"+
          "       Use reset <object>, use Reset only if necessary.\n");
    return 1;
  }
  if(!sizeof((ob = PROCESS_ARG(arg)))) return 1;
  for(i = 0; i < sizeof(ob); i++){
    name = (string)ob[i]->short();
    if(!name) name = "Invisible Object";
    filename =file_name(ob[i]);
    write("Reset("+reset_arg+")->"+filename+"<->"+name+".\n");
    call_other(ob[i], "reset", reset_arg);
  }
  return 1;
}


/***************************************************************************/
/* reset: do a reset(1) in an object */

static status reset_ob(string arg) {
  Reset_ob(arg,1);
  return 1;
}


/****************************************************************************/
/* light: add or subtract from the light in a room */

static status set_lighting(string str) {
  int n;
  string what;

  if(!str) {
    write("Light Strength: " + set_light(0) + "\n");
    write("Usage: light <+/- int>\n\n"+
          "Notes: This adjusts the amount of light you give out.\n"+
          "       'light 100' will light most rooms :)\n");
    return 1;
  }
  if(sscanf(str, "%d", n) == 1) {
    adj_light(n);
    if(n > 0) {
      say(query_name() + " waves a hand and it lightens the room.\n");
      write( "You wave your hand and the room lightens up.\n");
    }
    else
      tell_room(environment(this_player()),
        "Everything suddenly gets darker.\n");
  }
  else
    set_lighting(0);
  return 1;  
}


/***************************************************************************/
/* grab: grab some code out of one file and put it in another */

static status grab(string str){
  string filename1, filename2, txt, tmp;
  int line1, line2;
  int finish;

  if(!str){
    write("Usage: grab line1,line2 file1 file2\n"+
          "       grab line1 file1 file2\n\n"+
          "Notes: Copies from line1 of file1 to another file\n"+
          "       There is a 500 line limit to what can copied.\n");
    return 1;
  }
  if(sscanf(str,"%d,%d %s %s",line1,line2,filename1,filename2) != 4){
    if(sscanf(str,"%d %s %s", line1, filename1, filename2) != 3) {
      write("Wrong Arguments!\n\n"+
            "Usage: Grab line1,line2 file1 file2>\n");
      return 1;
    }
  }
  tmp = valid_read(filename1);

  if(!tmp || bad_file(tmp) || file_size("/"+tmp) < 0){
    write(filename1+" does not exist or you do not have access.\n");
    return 1;
  }
  filename1 = "/"+tmp;
  tmp = valid_write(filename2);
  if(!tmp){
    write("Illegal write to "+filename2+"\n");
    return 1;
  }
  filename2 = "/"+tmp;
/* read file */

  finish = (line2 && line1 + 500 > line2) ? line2 - line1
                                          : 500;
  
  if(!(txt = read_file(filename1,line1,finish))) {
    write("Line #"+line1+" is out of range or file is empty.\n");
    return 1;
  }

/* write file */

  write("Writing Block from "+line1+" of "+filename1+" to "+filename2+".\n");  
  write_file(filename2,txt);
  return 1;
}


/**************************************************************************/
/* copy file(s) */

varargs static status copy(string str, status overwrite, status mv) {
  int i;
  string tmp1, tmp2;
  string in, path_in, file_in;
  string out, file_out;
  string format_in, format_out;

  if(!str) {
    write(
      "Usage: copy <file> { file out }\n"+
      "       Copy <file> { file out } (overwrites file out)\n"+
      "       mv   <file> { file out } (deletes file)\n"+
      "       Mv   <file> { file out } (deletes file, overwrites file out)\n");
 
    write("Notes: copy file to optional file out, default current path.\n"+
          "       accepts wildcard \"*\" (may give unexpected results)\n"+
          "       is able to copy ^[ characters, checks if size of copied\n"+
          "       file is same of original before removing with mv.\n");
    return 1;
  }

  if(sscanf(str,"%s %s",in,out) != 2) { /* update dec. 93 */
     in = str;
     out = "/"+ current_path;
  }

  /* has wizard|player got read access */

  if(!(in = valid_read(in))) {
    write("Invalid read access.\n");
    return 1;
  }
  in = "/" + in;
  if(bad_file(in)) {
    write("Illegal filename: "+ in +"\n");
    return 1;
  }

  /* has wizard|player got write access */

  if(!(out = valid_write(out))) {
    write("Invalid write access.\n");
    return 1;
  }
  out = "/" + out;

  if(bad_file(out)) {
    write("Illegal filename: "+ out +"\n");
    return 1;
  }

  if(!sscanf(out,"%s*%s",tmp1,tmp2)) {
    if(file_size(out) == -2) {
      out += "/";
      if(!(out = valid_write(out))) {
        write("Invalid write access.\n");
        return 1;
      }
      out = "/" + out;
    }
  }

  /* extract path and filename */
  path_in = implode(explode(in, "/")[0..<2],"/") + "/";
  file_in = explode(in,"/")[<1];

#if 0

  for(i = strlen(in) - 1; (in[i] != '/' && i >= 0); i--);
  path_in = extract(in,0,i);
  file_in = extract(in,i+1,strlen(in)-1);
#endif 0


  for(i = strlen(out) - 1; (out[i] != '/' && i >= 0); i--);
  path_out = extract(out,0,i);
  file_out = extract(out,i+1,strlen(out)-1);

  /* get input wildcards to sscanf format to make output compat. string */

  format_in = wildcard_format(file_in);

  /* fix file output type file|wildcard|dir */

  if(!file_out || file_out == "") {
    file_out = file_in;
  }
  format_out = wildcard_format(file_out);

  if(format_number(format_in) != format_number(format_out)) {
    write("Wildcard parameters of input and output do not match.\n");
    return 1;
  }

  /* do some directory and file checking */

  if(file_size(extract(path_out,0,strlen(path_out)-2)) != -2) {
    write("Output Directory: "+path_out+" does not exist.\n");
    return 1;
  }
  if(file_size(extract(path_in,0,strlen(path_in)-2)) != -2) {
    write("Input Directory: "+path_in+" does not exist.\n");
    return 1;
  }

  /* get list of files */

#ifdef NO_GET_DIR_WILDCARDS
  list = (format_number(format_in)) ? get_dir(path_in +".") : get_dir(in);
#else
  list = get_dir(in);
#endif

  if(!sizeof(list)) {
    write("No match for "+ path_in + file_in +"\n");
    return 1;
  }

  sformat = format_in;
  sformat_out = format_out;
  new_text = path_in;
  line = 0;
  over_write = overwrite;
  move_file = mv;
  
  do_copy(0);
  return 1;
}


void do_copy(string answer) {
  int abort;
  string file_out;

  if(answer == "n") {
    write("Ok.\n");
    return;
  }

  for(; line < sizeof(list); line++) {
    if(abort > 20 * MAX_BYTE_READ) { /* abort after copying 160 KB */
      write("Continue (y/n) [y]: ");
      input_to("do_copy");
      return;
    }
    if(list[line] == "." || list[line] == "..") continue;
    if(file_size(new_text+list[line]) == -2) continue; 
    file_out = match_format(list[line],sformat,sformat_out);
    if(!file_out) continue;
    abort += file_size(new_text+list[line]);
    if(copy_file(new_text+list[line],path_out+file_out, over_write)) {
#ifdef 312MASTER
      if(move_file) {
        if(!rm(new_text+list[line])) {
          write("Failed to remove "+ new_text+list[line] +"\n");
        }
      }
#endif /* 312MASTER */
    }
  }
}


status copy_with_overwrite(string str) {
  copy(str,1,0);
  return 1;
}

static status move_file(string str) {
  copy(str,0,1);
  return 1;
}

static status move_file_with_overwrite(string str) {
  copy(str,1,1);
  return 1;
}


/***************************************************************************/
/* deltree */
               
static status deltree(string path) {
  if(!path || path[0] != '/') {
    write("Usage: deltree <path>\n\n"+
          "Notes: Deletes a whole directory tree.\n"+
          "       You must use full path, prepended by '/'\n");
    return 1;
  }

  if(!valid_write(path + "/*")) {
    write("Invalid Delete Access.\n");
    return 1;
  }
  if(bad_file(path)) {
    write("Illegal filename: "+ path +"\n");
    return 1;
  }

  write("Are you sure you want to delete all files and directories in\n"+
        "the path: \""+path+"\"  (y/n) [n]: ");
  new_text = path;
  input_to("answer_deltree");
  return 1;
}

static void answer_deltree(string ans) {
  if(ans == "y") { 
    delete_dir(new_text);
  }
  else {
    new_text = 0;
  }
  write("Ok\n");
}


static void delete_dir(string path) {
  string *files;
  int i;

  if(!path) return;
  if(path[strlen(path) - 1] != '/') path += "/";
  files = get_dir(path + ".");

  for(i = 0; i < sizeof(files); i++) {
    if(files[i] == "." || files[i] == "..") continue;
    if(file_size(path + files[i]) == -2) {
      delete_dir(path+files[i]);
    }
    else {
      if(!rm(path + files[i])) { 
        write("Failed to Delete: "+path+files[i]+"\n");
      }
    }
  }
  if(strlen(path) > 1) {
    path = extract(path,0,strlen(path)-2);
    if(!rmdir(path)) {
      write("Failed Directory Delete: "+path+"\n");
    }
    else {
      write("Deleted Directory: "+path+"\n");
    }
  }
  new_text = 0;
}
  

/***************************************************************************/
/* find file */

static status find_file(string str) {
  string pattern, txt, path;
  status flg;

  if(!str) {
    write("Usage: find <file{?}> { <path> }\n\n"+
          "Notes: Finds a file in the subdirectories of your path.\n"+
          "       If ? is used it searches as a pattern in filename\n"+
          "       An optional start <path> can be used (prepend <path> "+
          "with '/').\n");
    return 1;
  }

  if(!sscanf(str,"%s %s", pattern, path)) {
    path = "/" + (string)this_player()->query_path();
    pattern = str;
  }
  if(bad_file(path)) {
    write("Illegal filename: "+ path +"\n");
    return 1;
  }

  if(sscanf(pattern,"%s?",pattern)) {
    flg = 1;
    write("File Pattern Search: "+pattern+"->\n");
  }
  else {
    write("File Search: "+pattern+"->\n");
  }
  if(catch((txt = search_pattern(pattern, path, flg)))) {
    write("Error: Subdirectory tree too large too search.\n");
  }
  if(txt == "") {
    write("No Match.\n");
  }
  else if(txt) {
    write(txt);
  }
  write("Ok.\n");
  return 1;
}


static string search_pattern(string pattern, string path, status flg) {
  string *all_files, *files, *dir;
  string file_search;
  int i;

  if(path == "/") path = "";
  if(strlen(path) > 1 && path[strlen(path)-1] == '/') {
    path = extract(path,0,(strlen(path)-2));
  }
  
  all_files = get_dir(path + "/.");
  if(!all_files) return "";
  
  files = filter_array(all_files,"file_filter",this_object(),path);
  dir = filter_array(all_files,"directory_filter",this_object(),path);

  file_search = "";

  for(i = 0; i < sizeof(dir); i++) {
    file_search += search_pattern(pattern,path+"/"+dir[i],flg);
  }
   
  files = regexp(files, pattern);

  if(!flg && sizeof(files)) {
    files = filter_array(files,"pattern_filter",this_object(),pattern);
  }

  if(sizeof(files)) {
    file_search += path + "/";
    file_search += implode(files,"\n"+path+"/") + "\n";
  }
  return file_search;
}  


/****************************************************************************/
/* tree */

static status path_tree(string path) {
  int *path_status, i;
  string tmp1, tmp2;

  path_status = ({});

  path = make_path(path);

  /* scan for help */

  if(sscanf(path, "%s-h%s", tmp1, tmp2)) {
    write("Usage: tree { options: <path> }\n\n"+
          "Notes: writes a pictorial list of directory structure.\n"+ 
          "       <path> directory path\n"+
          "       This may cause too long evaluation errors on large\n"+
          "       directory trees.\n");
    write("       Example: tree /room\n");
    return 1;
  }
  write("Path: "+path+"\n\n");
  if(path != "/") {
    string *path_split;

#   ifdef OLD_EXPLODE
      path_split = explode(path+"/", "/");
#   else
      path_split = explode(path, "/");
#   endif /* OLD_EXPLODE */
    path_split = filter_array(path_split,"filter_zeros",this_object());
    path_status = allocate(sizeof(path_split));
    for(i = 0; i < sizeof(path_status); i++) path_status[i] = 1;
    write("___"+implode(path_split,"___"));
    new_text = path;
  }
  else {
    new_text = "";
  }
  file_tree(new_text, path_status);
  write("\n");
  return 1;
}


static status Path_tree(string path) {
  string *files, tmp1, tmp2;

  path = make_path(path);

  /* scan for help */

  if(sscanf(path, "%s-h%s", tmp1, tmp2)) {
    write("Usage: Tree { options: <path> }\n\n"+
          "Notes: <path> directory path\n"+
          "       There is a pause after each directory tree in 'path'\n"+
          "       This helps, but may still cause too long evaluation\n"+
          "       errors on large directory trees.\n");
    write("       Example: Tree /room\n");
    return 1;
  }


  /* get list of directories */

  if(path == "/")
    new_text = "";
  else
    new_text = path;

  /* get list of directories */

  list = get_dir(new_text + "/.");
  if(!list) return 1;
  list = filter_array(list,"directory_filter", this_object(), path);
  line = 0;
  if(!sizeof(list)) {
    write("Directory Empty.\n");
  }
  else {
    answer_tree_prompt("");
  }
  return 1;
}


static void answer_tree_prompt(string ans) {
  int *path_status;

  if(ans == "n") return;
  if(ans == "s") {
    skip();
    return;
  }
  write("Path: " + new_text+"/"+list[line] + "\n\n");
  if(new_text+"/"+list[line] != "/") {
    string *split_path;
    int i;

#ifdef OLD_EXPLODE
    split_path = explode(new_text+"/"+list[line]+"/","/");
#else
    split_path = explode(new_text+"/"+list[line], "/");
#endif /* OLD_EXPLODE */
    split_path = filter_array(split_path,"filter_zeros",this_object());
    path_status = allocate(sizeof(split_path));
    for(i = 0; i < sizeof(path_status); i++) path_status[i] = 1;
    write("___"+implode(split_path,"___"));
  }
  tree_prompt(new_text+"/"+list[line], path_status);
}
  

static void tree_prompt(string path, int *path_status) {
  file_tree(path, path_status);
  write("\n");
  skip();
}

static void skip() {
  if(++line < sizeof(list)) {
    if(new_text+list[line] == "/usr") {
      if(++line == sizeof(list)) return;
    }
    write("Next Path: "+new_text+"/"+list[line]+"\n");
    write("\nContinue (y/n/(s)kip) [y]: ");
    input_to("answer_tree_prompt");
  }
}


status filter_zeros(string str) {
  return ((!str || str == "") ? 0 : 1);
}

static string *file_tree(string path, int *path_status) {
  string *files;
  int i, k, size;
  string path_str;

  /* exceptions - dir too big for this fn */

#ifndef MUDOS_DR
  if(path == "/usr") {
    write("Directory /usr is too large");
    return ({});
  }
#endif /* MUDOS_DR */

  /* get list of directories */

  files = get_dir(path + "/.");
  if(!files) return ({});
  files = filter_array(files,"directory_filter", this_object(), path);

  /* create tree */
  
  path_str = "";
  if(path != "") {
    string *split_path;
#ifdef OLD_EXPLODE
    split_path = explode(path+"/","/");
#else
    split_path = explode(path,"/");
#endif /* OLD_EXPLODE */
    split_path = filter_array(split_path,"filter_zeros",this_object());
    for(k = 0; k < sizeof(split_path) && k < sizeof(path_status); k++) {
      path_str += (path_status[k]) ? "   " : " | ";
      path_str += extract("                          ",
                  0,(strlen(split_path[k])-1));
    }
  }
  
  for(i = 0, size = sizeof(files); i < size; i++) {
    int *new_path_status;

    if(files[i] && files[i] != "") {
      write(((!i) ? "___" : path_str + " |_") + files[i]);
      new_path_status = path_status + ({ (i == size-1) ? 1 : 0, });
      if(!sizeof(file_tree(path+"/"+files[i],new_path_status))) {
        write("\n");
      }
    }
  }
  return (files) ? files : ({});
}


/*****************************************************************************/
/*                   Wizard Tools - fn() used by various actions             */
/*****************************************************************************/


/******************************************************************************/
/* make_path */

string make_path(string file) {
  string *path_split, tmp;
  string tmp1, tmp2;
  int i, len;

  if(!file || file == "") return "/"+current_path;
  if(file == "~") return WIZARD_DIR +""+ name;
  if(sscanf(file,"~/%s",file)) {
    file = WIZARD_DIR +""+ file;
  }
  if(sscanf(file,"~%s",file)) {
    file = WIZARD_DIR +""+ name +"/"+ file;
  }
  if(sscanf(file,"%s#%s",tmp1,tmp2)) {
    file = DOMAIN_DIR +""+ tmp1 +"/w/"+ name +"/"+ tmp2;
  }
  if(sscanf(file,"%s,%s", tmp1, tmp2)) {
     file = DOMAIN_DIR +""+ tmp1 +"/w/"+ tmp2;  /* experimental */
  }
  if(file[0] != '/') {
    if(current_path == "") {
      file = "/"+file;
    }
    else {
      file = "/" + current_path + "/" + file;
    }
  }
  while(sscanf(file, "%s//%s", tmp1, tmp2)) file = tmp1 + "/" + tmp2;
 return ::make_path(file);
}

/***************************************************************************/

/* convert a plural string to a singlular */

string plural_to_single(string plural) {
  string word, end;
  int len;

  if(!plural) return "";
  len = strlen(plural); 

  /* sscanf() sucks...it wouldn't work *sob*  
   * it has problems with things like "%ss", - Zil */

  if(len > 3) { 
    end = extract(plural,(len - 3),(len - 1));
    word = extract(plural,0,(len - 4));
    if(end == "ies") return word + "y";  /* babies -> baby */
    if(end == "ses") {
      if(word[strlen(word)-1] == 's')   
        return word + "s";               /* glasses -> glass */
      else 
        return word + "se";              /* fuses -> fuse */
    }
  }
  if(len > 2) {
    end = extract(plural,(len - 2),(len - 1));
    word = extract(plural,0,(len - 1));
    if(end == "ii") return word + "us";  /* you never know ;) */ 
    if(plural[len-1] == 's')
      return extract(plural,0,(len - 2));
  }
  return plural;
}


/**************************************************************************/
/* find_object() - uses various object finding efuns */

static varargs object *FIND_OBJECT(string str, object who){
  string tmp1, tmp2;
  int i, j;
  object *ob;
  string current_path;

  if(!str){ 
    write("No Argument to FIND_OBJECT!\n");
    return ({});
  }
  ob = ({});
  if(str == "who" || str == "users") {
    ob = allocate(sizeof(users()));
    ob = users();
    if(who) {    /* eg. users in room */
      ob = filter_array(ob,"present_filter",this_object(),who);
    } 
  }  
  if(!sizeof(ob)) ob = allocate(1);
  if(!who) who = this_player();

  if(str == "me") ob[0] = this_player();
  else if(str == "env") ob[0] = environment(who);
  else if(str == "room") ob[0] = environment(who);
  else if(str == "all") {
    ob = allocate(sizeof(all_inventory(who)));
    ob = all_inventory(who);
    if(!sizeof(ob)) ob = allocate(1);
  }
  else if(sscanf(str, "all %s", tmp1)) {      /* eg. all swords */
    ob = filter_array(all_inventory(who), "plural_filter", this_object(), tmp1);
    if(!sizeof(ob)) ob = allocate(1);
  }
  else if(sscanf(str, "%d %s", j, tmp1)) {    /* eg. 3 swords */
    ob = filter_array(all_inventory(who), "plural_filter", this_object(), tmp1);
    if(j <= sizeof(ob)) {
      ob = ob[0..(j-1)];
    }
    if(!sizeof(ob)) ob = allocate(1);
  }
  else if(sscanf(str,"who%d",i) == 1) {
    if(i < 1 || i > sizeof(users()) - 2) {
      write("Who "+i+" is out of who number bounds.\n");
      return ({});
    } 
    ob[0] = users()[i-1];
  }
  if(!ob[0]) ob[0] = present(str, who);
  if(!ob[0] && environment(who)) ob[0] = present(str, environment(who));
  if(!ob[0]) ob[0] = find_object(str);
  if(!ob[0]) ob[0] = find_living(str);
  if(!ob[0]) {
    ob = find_all_living(plural_to_single(str));
    if(!sizeof(ob)) ob = allocate(1);
  }
  if(!ob[0]) ob[0] = find_living("ghost of " + str);
  if(!ob[0]) {
    if(!sscanf(str,"%s %s", tmp1, tmp2)) {
      str = make_path(str);
      sscanf(str,"/%s", str);
      sscanf(str,"%s.c",str);
      if(FILE_SIZE("/"+ str +".c") > 0) {
        if(catch(call_other(str,"???"))) {
          write("Error in loading: "+str+"\n");
          return ({});
        }
        ob[0] = find_object(str);
      }
    }
  }
  if(!ob[0]) return ({});
  return ob;
}


/***************************************************************************/
/* processes an action string to find an object list */
  
static object *PROCESS_ARG(string arg){
  string tmp1, tmp2;
  mixed *ob_list; 
  string *contents, list, content_list;
  mixed *ob;
  int i;

  if(!arg) return ({});
#ifdef OLD_EXPLODE
  ob_list = explode(arg + " in "," in ");
  list = implode(ob_list," of ") + " of ";
#else
  ob_list = explode(arg," in ");
  list = implode(ob_list," of ");
#endif
  ob_list = explode(list," of ");
  if(!ob_list) ob_list = ({ arg, });
  for(i = 0; i < sizeof(ob_list); i++) {
    content_list = ob_list[i];
    while(sscanf(content_list,"%s, %s", tmp1, tmp2)) {
      content_list = tmp1 + "," + tmp2;
    }
    if(content_list[strlen(content_list)-1] != ',') content_list += ",";
    contents = explode(content_list,",");
    ob_list[i] = contents;
  }
  ob = PROCESS_ARG_OB(ob_list,({ 0,})); 
  if(!sizeof(ob)) {
    write("Error: Cannot find '"+arg+"'\n");
  }
  return ob;
}

object *PROCESS_ARG_OB(mixed *ob_list, object *ob) {
  int i, size, j;
  object *new_ob, *obj;
  string *list;


  new_ob = ({});
  list = ob_list[sizeof(ob_list)-1];
  for(i = (size=sizeof(ob)) - 1; i >= 0; i--) {
    for(j = 0; j < sizeof(list); j++) {
      obj = FIND_OBJECT(list[j], ob[i]);
      if(sizeof(obj)) {
        new_ob = new_ob + obj;
      }
    }
  }
  if(!sizeof(new_ob)) return ({});
  if(sizeof(ob_list) > 2) {  
    return PROCESS_ARG_OB(ob_list[0..(sizeof(ob_list)-2)],new_ob);
  } 
  else if(sizeof(ob_list) == 2) {
    return PROCESS_ARG_OB(({ ob_list[0], }), new_ob);
  }
  return new_ob;
}

/************************************************************************/
/* copy a file from in to out - no validation checks */

status copy_file(string in, string out, status overwrite) {
  int f_size, current_byte;
  int max_byte;
  string txt;

  if(file_size(out) > 0) {
    if(!overwrite) {
      write("File Already Exists: "+out+"\n");
      return 0;
    }
    if(out == in) {
      write("Cannot copy file onto itself.\n");
      return 0;
    }
    write("Removing old file....\n");
    rm(out);
  }
  if(move_file) {
    write("Moving "+ in +" to "+ out +"\n");
#ifdef AMYLAAR
    rename(in, out);
    return 1;
#endif /* 312MASTER */
  }
  else {
    write("Copying "+in+" to "+out+"\n");
  }
  f_size = file_size(in);
  while(current_byte < f_size) {
    if(current_byte + MAX_BYTE_READ > f_size)
      max_byte = f_size - current_byte;
    else
      max_byte = MAX_BYTE_READ;
    txt = read_bytes(in, current_byte, max_byte);
    write_file(out, txt);
    current_byte += max_byte;
  
#ifdef MSDOS  /* add in the extra byte for CR when in dos */
#ifdef OLD_EXPLODE
    current_byte += sizeof(explode(txt + "\n", "\n")) - 1;    
#else
    current_byte += sizeof(explode(txt, "\n")) - 1; 
#endif
#endif /* MSDOS */

  }
  if(file_size(out) != file_size(in)) { 
    write("ERROR: Corrupt File "+ out +"\n");
    write("       Original: "+file_size(in)+" bytes, Copy: "+file_size(out)+
          " bytes.\n");
    return 0;
  }
  return 1;
}


/****************************************************************************/
/* glob routines convert wildcarded filename to sscanf format string */
 
 
// All these damn for & while loops HAD to go... Use Efuns instead
 
string wildcard_format(string file) {
  return file ? implode(explode(file,"*"),"%s") : 0;
}    
 
int format_number(string format) {
  return format ? sizeof(explode(format,"%s")) - 1 : 0;
}  
 
string match_format(string file, string format_in, string format_out) {
    string tmp1, tmp2, tmp3, tmp4, tmp5;
    string file_out;
 
    if(!format_out) format_out = format_in;
    switch(format_number(format_in)) {
      case 0:
         file_out = (file == format_in) ? format_out : 0;
      break;
 
      case 1:
        if(sscanf(file, format_in, tmp1)) {
          file_out = sprintf(format_out,tmp1);
        } 
      break;
 
      case 2:
        if(sscanf(file, format_in, tmp1, tmp2)) {
          file_out = sprintf(format_out, tmp1, tmp2);
        } 
      break;
 
      case 3:
        if(sscanf(file, format_in, tmp1, tmp2, tmp3)) {
          file_out = sprintf(format_out, tmp1, tmp2, tmp3);
        } 
      break;
 
      case 4:
        if(sscanf(file, format_in, tmp1, tmp2, tmp3, tmp4)) {
          file_out = sprintf(format_out, tmp1, tmp2, tmp3, tmp4);
        } 
      break;
 
      case 5:
        if(sscanf(file, format_in, tmp1,tmp2,tmp3,tmp4,tmp5)) {
          file_out = sprintf(format_out, tmp1, tmp2, tmp3, tmp4, tmp5);
        } 
      break;
 
      default:        
        file_out = 0;
      break;
    }
    return file_out;
}
 

/****************************************************************************/
/*               Filters used by filter_array()                             */
/****************************************************************************/


/* plural_filter
 *
 * makes an array objects whose id, or plural form of it matches
 */

static status plural_filter(object ob, string object_id) {
  if(ob->id(object_id) || ob->id(plural_to_single(object_id))) return 1;
}


/* present_filter
 *
 * makes an array of objects which are present in another object
 */

static status present_filter(object ob, object who) {
  if(present(ob,who)) return 1;
  return 0;
}

/* dot_filter
 *
 * makes and array of strings that do not have "." or ".."
 */

static status dot_filter(string file) {
  string tmp1, tmp2;
  
  if(file == "." || sscanf(file,"%s..%s", tmp1, tmp2)) return 0;
  return 1;
}

/* pattern_filter
 *
 * makes an array of strings that match a pattern exactly (see also regexp())
 */

static status pattern_filter(string str, string pattern) {
  if(str == pattern) return 1;
  return 0;
}

/* directory_filter
 *
 * makes an array of strings which are directories
 */

static status directory_filter(string file, string path) {
  if(!dot_filter(file)) return 0;
  if(FILE_SIZE(path + "/" + file) != -2) return 0;
  return 1;
}

/* file_filter
 * 
 * makes an array of strings which are files (ie not directories)
 */

static status file_filter(string file, string path) {
  if(!dot_filter(file)) return 0;
  if(FILE_SIZE(path + "/" + file) < 0) return 0;
  return 1;
}


/**************************************************************************/
/*                         Sorters used by sort_array()                   */
/**************************************************************************/ 

/* there must be an easier way */

static status by_name(object a, object b) {
  string a_str, b_str;
  int len_a, len_b, i;

  if(!(a_str = (string)a->query_name())) return 1;
  if(!(b_str = (string)b->query_name())) return 0;
  return (a_str < b_str) ? 1 : 0;
/*
  a_str = lower_case(a_str);
  b_str = lower_case(b_str);
  len_a = strlen(a_str);
  len_b = strlen(b_str);
  for(i = 0; i < len_a && i < len_b; i++) {
    if(a_str[i] < b_str[i]) return 1;
    if(a_str[i] > b_str[i]) return 0;
  }
  return (len_a < len_b) ? 1 : 0;
 */
}

static status by_level(object a, object b) { 
  return ((int)a->query_level() < (int)b->query_level()) ? 1 : 0;
}

static status by_age(object a, object b) { 
  return ((int)a->query_age() < (int)b->query_age()) ? 1 : 0;
}

static status by_environment(object a, object b) { 
  string a_str, b_str;
  int len_a, len_b, i;

  if(!(a_str = file_name(environment(a)))) return 1;
  if(!(b_str = file_name(environment(b)))) return 0;
  return (a_str < b_str) ? 1 : 0;
/*
  a_str = lower_case(a_str);
  b_str = lower_case(b_str);
  len_a = strlen(a_str);
  len_b = strlen(b_str);
  for(i = 0; i < len_a && i < len_b; i++) {
    if(a_str[i] < b_str[i]) return 1;
    if(a_str[i] > b_str[i]) return 0;
  }
  return (len_a < len_b) ? 1 : 0;
 */
}

static status by_ip_number(object a, object b) { 
  return (query_ip_number(a) < query_ip_number(b)) ? 1 : 0;
}
