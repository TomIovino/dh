// BIN_D.C -- Bin Command Daemon v2.10
// Copyright (c) 1995,1994 Andrew T. Graham -- All Rights Reserved
// May be copied and modfied as long as this header remains attached.
// Internet Email:  atgraham@cs.millersv.edu
// Intermud Email:  Geewhiz@DragonHeart
 
// Geewhiz@Dreamtime for Dreamtime & DoD
// Modified For DragonHeart 950723G
// Further Enhancements 950805G
 
// Features:
// o FAST! Command Lookups
// o Easily configurable to fit any Bin command directory structure
// o Easily configurable to level restrict bin commands
// o Allows for bin commands to have secondary verb aliases
//   (e.g. _score.c can add the verb 'score' and 'sc')
// o Global Command Table Rehash; Only needs to be done once after addding
//   a command.
// o Automaticly Detects (and performs) Auto-Rehashing on a command-by-command
//   basis.
// o Removes error-causing Bin Commands from the Command Table automaticly.
// o Handles any common bin command 'main' call format:
//   cmd_verb(), _verb(), verb(), & main()
//
 
 
#include <mudlib.h>
#include <levels.h>
#include <page.h>
 
// UserConfig
 
#define LEVEL_FUNC "query_security"
#define LEVEL_TEST #'>=
#define ALIAS_FUNC "query_cmd_aliases"
#define BIN_DIRS ({\
                   ({ "cmds/std"        ,  SEC0 }),\
                   ({ "cmds/skills"     ,  SEC0 }),\
                   ({ "cmds/party"      ,  SEC0 }),\
                   ({ "cmds/wiz"        ,  SEC2 }),\
                   ({ "cmds/admin"      ,  SEC8 }),\
                 })
 
#undef FORCE_IMMEDIATE_SWAP
 
#undef DEBUG_WIZ "geewhiz"
// End Config
 
#ifdef DEBUG_WIZ
#define DBG(X) if(find_player(DEBUG_WIZ))\
                     tell_object(find_player(DEBUG_WIZ),(X)+"\n")
#else
#define DBG(X)
#endif
 
#define DIR          0
#define LEV          1
#define C_WARNING    "WARNING"
#define C_FAILURE    "FAILURE"
#define C_FATAL      "FATAL  "
#define C_INFO       "INFO   "
#define OBJ_DEST_ERR "*Object the closure was bound to has been destructed\n"
 
mixed *list = BIN_DIRS;
mapping cmd_table = ([ ]);
closure do_cmd,do_help,do_rehash,print_entry,sprint_entry,short_sprint_entry;
 
  void log(string class, string msg);
status query_prevent_shadow();
  void add_cmd_verb(string v, closure c, closure h, int l, string f);
  void add_cmd_file(string dir, int min_level);
  void fill_cmd_table(string dir, int min_level);
  void rehash();
  void reset(int started);
status dumpcmdtable(string long);
status bin_dir_hook(string arg);
 
 
status query_prevent_shadow() { return 1; }
 

mapping query_cmdtable() { return copy_mapping(cmd_table); }
 
void log(string class, string msg) {
    log_file("BIND",sprintf("%s %s: %s\n",ctime(),class,msg));
}
 
 
void add_cmd_verb(string v,closure c,closure h,int l,string f) {
    cmd_table += ([ v : c; h; l; f ]);
}
 
void find_or_load(string verb) {
    string file, msg;
    object ob;
    if(ob = find_object(file = cmd_table[verb,3])) return;
    msg = catch(call_other(file,"_?",0));
    if(!msg) return;
   log(C_FATAL,sprintf("Verb: '%s' File: '%s' -> Error: '%s'",verb,file,msg));
    printf("bin_d:  Cannot load file for '%s'.  Notify a wizard.\n");
}
 
void add_cmd_file(string file, int min_level) {
    string *verbs, verb, f, func, tmp1, tmp2;
    closure ccl, hcl;
    int i;
 
    if(!mappingp(cmd_table)) cmd_table = ([ ]);
    ccl = hcl = 0;
 
    f = explode(file,"/")[<1];
#if 0
   if(sscanf(f,"_%s.c",verb) != 1) return;
    if(sscanf(verb,"%s.%s",tmp1,tmp2) > 0) return;
    if(sscanf(verb,"%s_%s",tmp1,tmp2) > 0) return;
#else
    if((sscanf(f,"_%s.c",verb) != 1)
    && (sscanf(f,"%s.c",verb) != 1)) return;
    if(sscanf(verb,"%s.%s",tmp1,tmp2) > 0) return;
    if(sscanf(verb,"%s_%s",tmp1,tmp2) > 0) return;
#endif
    
#ifdef MSDOS
    catch(     tmp1 = (string)call_other(file,"verb") );
        if(stringp(tmp1)) verb = tmp1;
#endif
 
    cmd_table = m_delete(cmd_table, verb);  // Failsafe
 
    if(catch(ccl = symbol_function("main",file)))
        return log(C_FAILURE,"File "+file+" would not load!");
    if(!ccl) ccl = symbol_function(sprintf("_%s",verb),file);
    if(!ccl) ccl = symbol_function(verb,file);
    if(!ccl) ccl = symbol_function("main",file);
    if(!ccl) return;
 
    hcl = symbol_function("help",file);
 
DBG(sprintf("bin_d: add_cmd_file:\t%s",verb));
    add_cmd_verb(verb,ccl,hcl,min_level,file);
 
#ifdef ALIAS_FUNC
    if((verbs=(string*)call_other(file,ALIAS_FUNC)) && (i=sizeof(verbs)))
        while( ( ( ( i-- ) ) && ( verbs[i] != verb ) ) ){
DBG(sprintf("\t\t\t%s",verbs[i],ccl,min_level,file));
            add_cmd_verb(verbs[i],ccl,hcl,min_level,file);
        }
#endif /* ALIAS_FUNC */
 
#ifdef FORCE_IMMEDIATE_SWAP
     swap(find_object(file));
#endif /* FORCE_IMMEDIATE_SWAP */
}
 
 
void fill_cmd_table(string dir, int min_level) {
    string *files, file; 
    int i;
 
    if(!mappingp(cmd_table)) cmd_table = ([ ]);
 
    files = get_dir(sprintf("/%s/*",dir));
 
    if(!files || !(i = sizeof(files)))
        return log(C_WARNING,"No Files In /"+dir);
    while(i--) 
        add_cmd_file(sprintf("/%s/%s",dir, files[i]), min_level);
}
 
 
void rehash() {
    int i;
DBG("bin_d: rehash();");
    cmd_table = ([ ]);
    log(C_INFO,"Command Table Rehashed");
    if(i = sizeof(list))
      while(i--) fill_cmd_table(list[i][DIR],list[i][LEV]);
}
     
void reset(int started) {
DBG(sprintf("bin_d: reset(%d);",started));
   rehash();
    if(started) return;
    log(C_INFO,"Bin_D Loaded");
 
   do_rehash = lambda( ({ 'func, 'verb, 'arg }),
    ({ #',,
      ({ #'?,
        ({#'mapping_contains,({#'&,'ccl}),({#'&,'hcl}),({#'&,'minlev}),
                             ({#'&,'file}),({#'cmd_table}),'verb}),
        ({ #',,
            ({ #'log,C_WARNING,({#'sprintf,"Auto Rehashed %O",'file})}),
            ({ #'add_cmd_file, 'file, 'minlev }),
            ({ #'funcall, 'func, 'verb, 'arg })
        }), 0
      })
    }));
 
    do_cmd = lambda( ({ 'verb, 'arg }),
    ({ #',,
      ({ #'?,
        ({#'mapping_contains,({#'&,'ccl}),({#'&,'hcl}),({#'&,'minlev}),
                             ({#'&,'file}),({#'cmd_table}),'verb}),
        ({ #',,
            ({ #'=,'lev,({#'call_other,({#'this_player}),LEVEL_FUNC})}),
            ({ #'?,({LEVEL_TEST,'lev,'minlev}),({#'funcall,'ccl,'arg}),0})
        }), 0 
      })
    }));
 
    do_help = lambda( ({ 'verb, 'null }),
    ({ #',,
        ({ #'?,
            ({#'mapping_contains,({#'&,'ccl }),({#'&,'hcl}),({#'&,'minlev}),
                                 ({#'&,'dir}),({#'cmd_table}),'verb}),
            ({ #',,
                ({ #'=,'lev,({#'call_other,({#'this_player}),LEVEL_FUNC})}),
                ({#'?,({LEVEL_TEST,'lev,'minlev}),({#'funcall,'hcl}),1})
            }), 0
        })
    }));
 
    print_entry = lambda( ({ 'verb, 'c, 'h, 'l, 'f }),
        ({ #'sprintf, "%-10s : %O %O\n", 'verb, 'c, 'h }) 
    ); 
 
    sprint_entry = lambda( ({ 'verb, 'c, 'h, 'l, 'f, 'ret }),
        ({ #'+=, 'ret, ({ #'sprintf, "%-10s [%3d] %s %-30s\n",
                 'verb, 'l, ({ #'?, 'h, "*", " "}), 'f })
    })); 
 
    short_sprint_entry = lambda( ({ 'verb, 'c, 'h, 'l, 'f, 'ret }),
        ({ #'+=, 'ret, ({ #'sprintf, "%s, ", 'verb }) })
    ); 
 
    call_out("rehash",2);
}
 
void print_cmdtable() {
    walk_mapping(cmd_table,print_entry);
}
 
status dumpcmdtable(string long) {
    string *verbs, header, data;
DBG(sprintf("bin_d: dumpcmdtable(%O);",long));
    log(C_INFO,"Command Table Dumped" + (long ? "(Long)" : "(Short)"));
    if(!long) {
        header = sprintf("%|79s\n","Standard Commands"); 
        data = "";
        walk_mapping(cmd_table,short_sprint_entry,&data);
        data = implode(sort_array(explode(data,", "),#'>),", ");
        data = header + sprintf("%-=79s\n",data[2..]);
    } else {
        header = sprintf("%|10s %|5s %s\n%|10s %|5s %s %|30s",
                       "","SEC","Help Available","Verb","Level","|","File"); 
        data = "";
        walk_mapping(cmd_table,sprint_entry,&data);
        data = implode(sort_array(explode(data,"\n"),#'>),"\n");
        data = header+data;
    }
    return page(data),1;
}
 
status bin_dir_hook(string arg) {
    string verb, err;
    status ret;
    verb = query_verb();
DBG(sprintf("bin_d: bin_dir_hook:  %s(%O);",verb,arg));
    if(!cmd_table || !mappingp(cmd_table)) 
        return notify_fail("bin_d:  cmd_table empty!\n"),0;
    switch(verb) {
        case "rehash": 
            printf("Rehashing command lookup table...");
            rehash(); printf("Done.\n"); return 1; break;
        case "dumpcmdtable":
            return dumpcmdtable(arg); break;
        case "help":
            find_or_load(verb);
            err = catch(ret = funcall(do_cmd,verb,arg,0));
            if(err || !ret) {
                find_or_load(arg);
                err = catch(ret = funcall(do_help,arg));
                if(err && err == OBJ_DEST_ERR) {
                    notify_fail("Auto-Rehash failed on 'help "+arg+"'.\n");
                    ret = funcall(do_rehash,do_help,arg,arg);
                }
                return ret;
            }
            return ret;
            break;
    }
    find_or_load(verb);
    err = catch(ret = funcall(do_cmd,verb,arg,0));
    if(err && err == OBJ_DEST_ERR) {
        notify_fail("Auto-Rehash failed on '"+verb+"'.\n");
        ret = funcall(do_rehash,do_cmd,verb,arg);
    }
   if(stringp(err)) 
   log(C_WARNING,sprintf("Cmd %O Err %O",verb,err));
    return ret;
}
