////////////////////////////////////////////////////////////
// SIM_EF_1.C       //  DRAGONHEART  // GEEWHIZ.5.17.97   //
////////////////////////////////////////////////////////////
// DO NOT change this file!  unless:
//    a)  You make a backup first; and
//    b)  You make the changes in a second copy; and
//    c)  You make sure the new copy loads
// DO NOT UNDER ANY CIRCUMSTANCES EVER MODIFY THEN UPDATE/LOAD sim_ef_1.c !
// Why?  You confuse the driver and you create a crash situation and if the
// MUD shutdown/crashes with errors in sim_ef_1.c you will not be able to 
// fix it.  I'll have to fix it and you'll make me mad.


#pragma strict_types
#pragma save_types

 /* simul_ef.c

  This object simulates efuns for the DRAGONHEART mudlib.
 */

#include "/include/mudlib.h"

string cap(string str) {
    if(!stringp(str))
        raise_error("cap():  Bad arg 1 !\n");
    return capitalize(str);
}

status blueprintp(object ob) { return sizeof(explode(file_name(ob),"#")) == 1; }
status clonep(object ob) { return !blueprintp(ob); }
nomask void breakpoint() {}  // Potential Crasher.  950729G
string query_host_name() { return "dh.mudservices.com"; }
status notify_fail(string s) { return efun::notify_fail(s),0; }

#define MAX_LOG_SIZE 50000
#define BACKBONE_WIZINFO_SIZE 10
#define LIVING_NAME 3
#define NAME_LIVING 4
 
#include "/include/cfg/wizlist.h"
#include "/include/fn/erq.h"
 
#undef MASTER
#define MASTER __MASTER_OBJECT__
 
nomask void efun308() {}
nomask void garbage_collection() {}

object find_object_or_load(string file) {
    object ret;
    if(ret = find_object(file))
        return ret;
    catch(file->loadFILE());
    return find_object(file);
}

void move_object(mixed what, mixed where) {
    object env, old_env;
//printf("\nmove_object(%O, %O)\n\n",what,where);
    if(what && stringp(what)) what = find_object_or_load(what);
    if(where && stringp(where)) where = find_object_or_load(where);
    if(!what || !objectp(what))
        raise_error("move_object: Bad arg 1.\n");
    if(!where || !objectp(where))
        raise_error("move_object: Bad arg 2.\n");
    if(what && old_env = environment(what))
        catch(old_env->object_left(what));
    efun::move_object(what,where);
    if(what && old_env != (env = environment(what)))
        catch(env->object_entered(what));
}            


string caller_stack() {
    int i;
    string ret;
    ret = "\tObject Stack:\n";
    i = caller_stack_depth();
    while(i--)
        ret += sprintf("\t\t%O\n",previous_object(i));
    ret += "\n";
    return ret;
}

varargs string timestamp(int t) {
    string cts, ret;
    string dy,mo;
    int dt,hr,mn,sc,yr;
    mapping mo_to_int;
    mo_to_int = ([ "Jan" : 1,   "Feb" : 2,     "Mar" : 3,       "Apr" : 4,
                   "May" : 5,   "Jun" : 6,     "Jul" : 7,       "Aug" : 8,
                   "Sep" : 9,   "Oct" : 10,    "Nov" : 11,      "Dec" : 12 ]);
                   
    cts = t ? ctime(t) : ctime();
    sscanf(cts,"%s %s %d %d:%d:%d %d",dy,mo,dt,hr,mn,sc,yr);
    yr = yr - ( (yr / 100) * 100 );
    return sprintf("[%02d%02d%02d %02d%02d%02d]",
                    yr,mo_to_int[mo],dt,hr,mn,sc);
}

void log_file(string file, string str);
#if 1 
object clone_object(string file) {
    object o;
    mixed err;
    if(!file || !stringp(file))
        raise_error("clone_object():  Bad argument 1!\n");
    if(file[0..7] == "/players") {
        file = "/w" + file[8..];
        log_file("FIXME", sprintf("%s co() w/ \"/players\" %O\n",
                     timestamp(), previous_object()));
    }
    err = catch(o = efun::clone_object(file));
    if(err) {
        log_file("clone_object",sprintf("%s\n\tFile: %s\n\tError: %s",
                 timestamp(),file,err));
        log_file("clone_object",caller_stack());
        o = efun::clone_object("secure/obj/rift");
        o->rift();
        return o;
    }
    return o;
}
#endif

#if 0
void shout(string s) {
    filter_array(users(), lambda(({'u}),({#'&&,
      ({#'environment, 'u}),
      ({#'!=, 'u, ({#'this_player})}),
      ({#'tell_object, 'u, s})
    })));
}
#endif
 
mapping living_name_m, name_living_m;
 
void start_simul_efun() {
    mixed *info;
 
    if ( !(info = get_extra_wizinfo(0)) )
        set_extra_wizinfo(0, info = allocate(BACKBONE_WIZINFO_SIZE));
    if (!(living_name_m = info[LIVING_NAME]))
        living_name_m = info[LIVING_NAME] = allocate_mapping(0, 1);
    if (!(name_living_m = info[NAME_LIVING]))
        name_living_m = info[NAME_LIVING] = allocate_mapping(0, 1);
    if (find_call_out("clean_simul_efun") < 0)
        call_out("clean_simul_efun", 1800);
}
 
static void clean_name_living_m(string *keys, int left) {
    int i, j;
    mixed a;
 
    if (left) {
        if (pointerp(a = name_living_m[keys[--left]]) && member(a, 0)>= 0) {
            i = sizeof(a);
            do {
                if (a[--i])
                    a[<++j] = a[i];
            } while (i);
            name_living_m[keys[left]] = a = j > 1 ? a[<j..] : a[<1];
        }
        if (!a) m_delete(name_living_m, keys[left]);
        call_out("clean_name_living_m", 1, keys, left);
    }
}
 
static void clean_simul_efun() {
    /* There might be destructed objects as keys. */
    m_indices(living_name_m);
    remove_call_out("clean_simul_efun");
    if (find_call_out("clean_name_living_m") < 0) {
        call_out(
          "clean_name_living_m",
          1,
          m_indices(name_living_m),
          sizeof(name_living_m)
        );
    }
    call_out("clean_simul_efun", 3600);
}
 
/* disable symbol_function('set_living_name, SIMUL_EFUN_OBJECT) */
protected void set_living_name(string name) {
    string old;
    mixed a;
    int i;
 
    if (old = living_name_m[previous_object()]) {
        if (pointerp(a = name_living_m[old])) {
            a[member(a, previous_object())] = 0;
        } else {
            m_delete(name_living_m, old);
        }
    }
    living_name_m[previous_object()] = name;
    if (a = name_living_m[name]) {
        if (!pointerp(a)) {
            name_living_m[name] = ({a, previous_object()});
            return;
        }
        /* Try to reallocate entry from destructed object */
        if ((i = member(a, 0)) >= 0) {
            a[i] = previous_object();
            return;
        }
        name_living_m[name] = a + ({previous_object()});
        return;
    }
    name_living_m[name] = previous_object();
}

nomask
object find_living(string name) {
    mixed *a, r;
    int i;

//    if(catch("/secure/misc/hide_d"->valid_find_living(this_player(),name)))
//        return 0;
 
    if (pointerp(r = name_living_m[name])) {
        if ( !living(r = (a = r)[0])) {
            for (i = sizeof(a); --i;) {
                if (living(a[<i])) {
                    r = a[<i];
                    a[<i] = a[0];
                    return a[0] = r;
                }
            }
        }
        return r;
    }
    return living(r) && r;
}
 
nomask
object find_player(string name) {
    mixed *a, r;
    int i;
 
    if(!name || !stringp(name))
        raise_error("find_player:  Bad argument 1\n");

//    if(catch("/secure/misc/hide_d"->valid_find_player(this_player(),name)))
//        return 0;
        
    if(!name_living_m || !mappingp(name_living_m))
        name_living_m = ([ ]); 
    if (pointerp(r = name_living_m[name])) {
        if ( !(r = (a = r)[0]) || !query_once_interactive(r)) {
            for (i = sizeof(a); --i;) {
                if (a[<i] && query_once_interactive(a[<i])) {
                    r = a[<i];
                    a[<i] = a[0];
                    return a[0] = r;
                }
            }
            return 0;
        }
        return r;
    }
    return r && query_once_interactive(r) && r;
}

/* not quite compatable with Bubbs _find_player
nomask
object _find_player(string name) {
    mixed *a, r;
    int i;
 
    if(!name || !stringp(name))
        raise_error("_find_player:  Bad argument 1\n");

    if(!name_living_m || !mappingp(name_living_m))
        name_living_m = ([ ]); 
    if (pointerp(r = name_living_m[name])) {
        if ( !(r = (a = r)[0]) || !query_once_interactive(r)) {
            for (i = sizeof(a); --i;) {
                if (a[<i] && query_once_interactive(a[<i])) {
                    r = a[<i];
                    a[<i] = a[0];
                    return a[0] = r;
                }
            }
            return 0;
        }
        return r;
    }
    return r && query_once_interactive(r) && r;
}
*/
/* Bubbs _find_player, ported by Elric 23/11/98 */

closure filt;

varargs mixed _find_player(string name, status flag) 
{
    object ob, *us;
    int size;
    if (!filt) filt=lambda(({ 'user, 'name }),
      ({ #'!,
        ({ #'strstr,
          ({ #'call_other, 'user, "query_real_name" }),
          'name
        }),
      }));
    if (flag && ob=find_player(name)) return ob;
    us=filter_array(users(), filt, name);
    size=sizeof(us);
    if (flag) return (size == 1 ? us[0] : 0);
    return us;
}



/*
nomask
object *users() { 
    object *u, *t;
    u = efun::users();
    if(catch(t = (object*)"/secure/misc/hide_d"->valid_users(this_player())) || t)
        if(pointerp(t))
            u -= t;
    return u;
}
*/
 
void cindent(string name) {
    string valid;
 
    valid = funcall(
        bind_lambda(#'call_other, previous_object()),
        __MASTER_OBJECT__,
        "valid_write",
        name,
        0,
        "cindent", previous_object()
    );
    if (valid)
    send_erq(ERQ_FORK, "indent_wrapper " + (stringp(valid) ? valid : name));
}
 

 /* have 'earmuffs on shout */



#if defined(NATIVE_MODE)

varargs string creator(object ob) {
  if(!ob) ob = previous_object();
  return (string)MASTER->get_wiz_name(file_name(ob));
}


varargs mixed create_wizard(string owner, string domain) {
    mixed result;

    result =
      (mixed)MASTER->master_create_wizard(owner, domain, previous_object());
    if(stringp(result)) return result;
    return 0;
}


string file_name(object ob) {
  string file;

  file = efun::file_name(ob);
  return file[1..(strlen(file)-1)];
}


#endif  /* NATIVE_MODE */



#if defined(AMYLAAR) || defined(MUDOS_DR)  /* amylaar driver */

#define MAX_LOG_SIZE 50000


void log_file(string file, string str) {
    string file_name;

    file_name = "/log/" + file;
#ifdef AMYLAAR
    if(file_size(file_name) > MAX_LOG_SIZE) {
       catch(rename(file_name, file_name +".old")); 
    }
#endif  /* AMYLAAAR */
    write_file(file_name, str);
}


void log_file_ts(string file, string s) {
    if(!s || !stringp(s)) return;
    if(s[<1] == '\n') s = s[0..<2];
    log_file(file,sprintf("%s %-=60s\n",timestamp(),s));
}

#ifndef NATIVE_MODE

varargs mixed create_wizard(string owner, string domain) {
    mixed result;

    result =
      (mixed)MASTER->master_create_wizard(owner, domain, previous_object());
    if(stringp(result)) return result;
    return 0;
}

#endif  /* not native */


mixed snoop(mixed snoopee) {
    int result;

    if(this_player() != this_interactive())
        raise_error("Security:  snoop():  Bad call stack (forced?)\n:");
    if(this_player()
    && snoopee
    && (int) this_player()->query_security() < (int) snoopee->query_security()
    && (int) this_player()->query_security() < 100) 
        raise_error("Security:  snoop():  No privilige.\n:");
        

    if (snoopee && query_snoop(snoopee)) {
     write("Busy.\n");
     return 0;
    }
#if defined(NATIVE_MODE)
    result = efun::snoop(this_player(), snoopee);
#elif defined(COMPAT_FLAG)
    result = snoopee ? efun::snoop(this_player(), snoopee)
               : efun::snoop(this_player());
#endif  /* compat - native */
    switch (result) {
     case -1:
         write("Busy.\n");
         break;
     case  0:
         write("Failed.\n");
         break;
     case  1:
         write("Ok.\n");
         break;
    }
    if (result > 0) return snoopee;
}

#endif  /* amylaar or mudos */


#ifdef AMYLAAR

void localcmd() {
  string *cmds;
  cmds = sort_array(query_actions(this_player()), #'>);
  write(implode(cmds,", ") +"\n");
}

int file_time(string path) {
    mixed *v;

    if(sizeof(v=get_dir(path,4))) return v[0];
}

mixed *unique_array(mixed *arr,string func,mixed skipnum) {
    mixed *al, last;
    int i, j, k, *ordinals;

    if (sizeof(arr) < 32) return efun::unique_array(arr, func, skipnum);
    for (ordinals = allocate(i = sizeof(arr)); i--; )
     ordinals[i] = i;
    al = order_alist(map_objects(arr, func), ordinals, (ordinals=0,arr));
    arr = al[2];
    ordinals = al[1];
    al = al[0];
    if (k = i = sizeof(al)) {
     for (last = al[j = --i]; i--; ) {
         if (al[i] != last) {
          if (last != skipnum) {
              arr[--k] = arr[i+1..j];
              ordinals[k] = ordinals[j];
          }
          last = al[j = i];
         }
     }
     if (last != skipnum) {
         arr[--k] = arr[0..j];
         ordinals[k] = ordinals[j];
     }
    }
    return order_alist(ordinals[k..], arr[k..])[1];
}

#ifndef NO_MAPPINGS
mapping m_delete(mapping m, mixed key) {
    return efun::m_delete((m), key);
}
#endif



 /*********************************************************************/
 /* Force an object to become 'this_player()'. Only callable by       */
 /* valid player objects                                              */

nomask varargs object set_this_player(object ob) {
  if(!ob) ob = previous_object();
  if(!MASTER->valid_player_call(previous_object())) {
    log_file("ILLEGAL","Call set_this_player() by "+
               file_name(previous_object()) +"\n");
    return 0;
  }
  return efun::set_this_player(ob),ob;
}



 /*********************************************************************/
 /* returns an list of objects that can be found by find_living()
   Basically giving the contents of the living 'name' hash table.
   Used in wizard.c in 'FIND_OBJECT'  

  limitations:  evaluation limits to less then 30

*/


object *find_all_living(string name) {
  object ob, *all;
  closure live;
  int i;

  live = unbound_lambda(({'live_name}),({#'set_living_name,'live_name}));
  for(all = ({}),i = 0; (ob = find_living(name)) && i < 45; i++) {
    if(member_array(ob, all) != -1) break;
    all += ({ ob, });
    funcall(bind_lambda(live,ob),"DUMMY");
  }
  for(i = sizeof(all); i--; ) {
    funcall(bind_lambda(live,all[i]),name);
  }
  return all;
}

#endif



#if defined(312MASTER) || defined(MUDOS_DR)
 /************************************************************/
 /* returns an list of objects that can be found by find_living()
   Basically giving the contents of the living 'name' hash table.
   Used in wizard.c in 'FIND_OBJECT'  

  limitations:  evaluation limits to less then 30,
          works in conjunction with lfun set_living(name),
          (it will stop evaluating if it doesn't have this
          lfun)

example lfun (it has been put in base_obj.c):
         void set_living(string str) {
           if(file_name(previous_object()) == SIMUL_EFUN) {
          set_living_name(str);
           }
         }
*/


object *find_all_living(string name) {
  object ob;
  object *all;
  int i;

  for(all = ({}); (ob = find_living(name)) && i < 45; i++) {
    if(member_array(ob, all) != -1) break;
    all += ({ ob, });
    ob->set_living("DUMMY");
  }
  for(i = sizeof(all); i--; ) all[i]->set_living(name);
  return all;
}
  

#endif  /* 312MASTER */



 /************************************************************************/
 /* rtime -  convert a ctime() format time string back to time() int */
 /*          returns -1 if input string has wrong clock format       */
 /*          used by 'rm' action in wizard.c                         */


#define LEAP_YEAR      ((year%4) ? 0 : 1)

#define ZERO_YEAR      1970
#define ZERO_DAY       1
#define ZERO_HOUR      10
#define ZERO_MIN       0
#define ZERO_SEC       0
#define NEXT_LEAP_YEAR 1

#define YEAR   31536000
#define DAY    86400
#define HOUR   3600
#define MIN    60


int rtime(string str) {
  int i;
  string day, month;
  int date, year, hour, min, sec;
  string *month_names; 
  int month_index, *month_days;
  int rtime;

  if(sscanf(str,"%s %s %d %d:%d:%d %d", day,
                          month,
                           date,
                           hour,
                         min,
                         sec,
                         year) == 7
  || sscanf(str,"%s %s  %d %d:%d:%d %d", day,
                           month,
                         date,
                         hour,
                          min,
                          sec,
                          year) == 7) {
    month = capitalize(lower_case(month));
    month = extract(month,0,2);

    month_names = ({ "Jan", "Feb", "Mar", "Apr", "May", "Jun",
               "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", 
    });
    month_days = ({ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, });
    if((month_index = member_array(month, month_names)) == -1) return -1;

    rtime = (year-ZERO_YEAR)*YEAR;
    rtime += ((year-ZERO_YEAR+NEXT_LEAP_YEAR)/4) * DAY;
    for(i = 0; i < month_index; i++) {
      rtime += (month_days[i] + ((i == 1 && LEAP_YEAR) ? 1 : 0)) * DAY;
    } 
    rtime += (date-ZERO_DAY) * DAY;
    rtime += (hour-ZERO_HOUR) * HOUR;
    rtime += min * MIN;
    rtime += sec;
    return rtime;
  }
  return -1;
}



 /*************************************************************************/
 /* update actions, used in room2.c                                       */
 /* limited to top 45 objects                                             */

varargs void update_actions(object ob) {
  int i;
  object *inv;

  if(!ob) ob = previous_object();
  inv = all_inventory(ob);
  for(i = 0; i < sizeof(inv) && i < 45; i++) {
    if(!inv[i] || !living(inv[i])) continue;
#ifdef NATIVE_MODE
    inv[i]->move(ob);
#else
    move_object(inv[i], ob);
#endif  /* NATIVE_MODE */
  }
  if(environment(ob)) {
#ifdef NATIVE_MODE
    ob->move(environment(ob));
#else
    move_object(ob, environment(ob));
#endif  /* NATIVE_MODE */
  }
}


#ifndef NATIVE_MODE

 /**********************************************************************/
 /* restrict command() usage                                           */

nomask varargs status command(string str,object ob) {
  if(!ob) ob = previous_object();
  if(ob != this_player() && this_player()) {
    if((int)this_player()->query_security_level() 
     < (int)ob->query_security_level()
     && (int)this_player()->query_security_level() < 100) {
      raise_error("Security: command():  No privilige.\n");
    }
  }
  return efun::command(str,ob);
}

#endif  /* NATIVE_MODE */


#ifdef VERSION
string version() { return VERSION; }
#endif  /* VERSION */

#if defined(AMYLAAR) && HAVE_WIZLIST == 0

#include "/include/cfg/wizlist.h"

void add_worth(int value, object ob) {
    mixed old;
#ifdef OLD_EXPLODE
    switch (explode(file_name(previous_object()) +"/", "/")[1]) {
#else
    switch (explode(file_name(previous_object()), "/")[1]) {
#endif
      default:
        raise_error("Illegal call of add_worth.\n");
      case "obj":
      case "room":
      case "secure":
    }
    if (!ob) {
        if ( !(ob = previous_object(1)) )
            return;
    }
    if (intp(old = get_extra_wizinfo(ob)))
        set_extra_wizinfo(ob, old + value);
}


void wizlist(string name) {
    int i, pos, total_cmd;
    int *cmds;
    mixed *a;
    mixed *b;

    if (!name) {
        name = (string)this_player()->query_real_name();
        if (!name)
            return;
    }
    a = transpose_array(wizlist_info());
    cmds = a[WL_COMMANDS];
    a[WL_COMMANDS] = a[0];
    a[0] = cmds;
    a = order_alist(a);
    cmds = a[0];
    a[0] = a[WL_COMMANDS];
    a[WL_COMMANDS] = cmds;
    if ((pos = member(a[WL_NAME], name)) < 0 && name != "ALL")
        return;
    b = allocate(sizeof(cmds));
    for (i = sizeof(cmds); i;) {
        b[<i] = i;
        total_cmd += cmds[--i];
    }
    a = transpose_array(a + ({b}) );
    if (name != "ALL") {
        if (pos + 18 < sizeof(cmds)) {
            a = a[pos-2..pos+2]+a[<15..];
        } else if (pos < sizeof(cmds) - 13) {
            a = a[pos-2..];
        } else {
            a = a[<15..];
        }
    }
    write("\n\t   -=[ Top Creators of "+ MUD_NAME +" ]=-\n\n");
    if (total_cmd == 0)
        total_cmd = 1;
    for (i = sizeof(a); i; ) {
        b = a[<i--];
        printf("%-15s %5d %2d%% (%d)\t[%4dk,%5d] %6d %d\n",
          b[WL_NAME], b[WL_COMMANDS], b[WL_COMMANDS] * 100 / total_cmd, b[<1],
          b[WL_EVAL_COST] / 1000,
          b[WL_HEART_BEATS], b[WL_EXTRA], b[WL_ARRAY_TOTAL]
        );
    }
    printf("\nTotal         %7d     (%d)\n\n", total_cmd, sizeof(cmds));
}

#endif  /* WIZLIST */

 // These are a number of Timewarp's simul efuns.
 // They are used throught the bim distribution, so I thought
 // I'd better put them in the distribution too.
 // Either use bim.h to inherit them into the files, or add
 // them as simul_efuns.  Your choice.
 // Bubbs.  24/Sep/94.

#include "/include/fn/bim.h"

#if defined(SML_SET_OF)
mixed *set_of(mixed *arr) { return m_indices(mkmapping(arr)); }
#endif


#if defined(SML_SUBST)
string subst(string str, string patt, string repl) {
  string new;
  int index, len1, len2;
  if (str == 0) str = "";
  if (!stringp(str))
   raise_error("subst(): Bad type to argument 1.\n");
  if (!stringp(patt))
   raise_error("subst(): Bad type to argument 2.\n");
  if (!stringp(repl))
   raise_error("subst(): Bad type to argument 3.\n");
  if (strstr(str, patt) == -1)
   return str;
  return implode(explode(str, patt), repl);
  }
#endif


#if defined(SML_SUM_ARRAY)
mixed sum_array(mixed *arr) {
  mixed ret;
  int index, size;
  size = sizeof(arr);
  if (!size) return 0;
  ret = arr[0];
  for(index = 1; index < size; index++)
   ret = ret + arr[index];
  return ret;
  }
#endif


#if defined(SML_TRIMSTR)
varargs string trimstr(string str, int char, int mode) {
  string new;
  if (!str || !stringp(str)) return 0;
  if (str == "") return str;
  if (!char) char = ' ';
  if (!mode) mode = 3;
  new = str;
  if (mode & 1)
   while(new && new != "" && new[0] == char)
    new = new[1..];
  if (mode & 2)
   while(new && new != "" && new[<1] == char)
    new = new[0..<2];
  return new;
  }
#endif


 /**********************************************************************/
 /* eval_path */

string eval_path(string file) {
  string tmp1, tmp2;
  string *path_split;
  int i;

  if(file[0] != '/')
    if((string)this_player()->query_path())
      file = (string)this_player()->query_path() + "/" + file;
  if(sscanf(file,"%s..%s", tmp1, tmp2)) {
    path_split = explode(file,"/");
    while((i = member_array("..",path_split)) != -1) {
      path_split = path_split[0..i-2]+path_split[i+1..sizeof(path_split)-1];
    }
    file = "/" +implode(path_split,"/");
    while(sscanf(file, "%s//%s", tmp1, tmp2)) file = tmp1 + "/" + tmp2;
  }
  return file;
}



#include "/secure/mudtime_sims.c"
