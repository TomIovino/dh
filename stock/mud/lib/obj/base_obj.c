  ////////////////////////////////////////////////////////////
  // BASE_OBJ.C      // DRAGONHEART    // ASH.12.12.99      //
  ////////////////////////////////////////////////////////////
  // Inherited base object.

#pragma strict_types
#pragma save_types
#pragma combine_strings

#include <mudlib.h>
#include <process.h>

#include <base_obj.spc>
#include <base_obj.cfg>
#include <ansi.h>
#include <parse_msg.h>
#include <ansi_write.h>
#include <colorme.h>
#include <bonus_check.h>


string query_object_type() { return "base"; }
string query_create_room() { return create_room; }
object query_environment() { return this_object() ? 
                             environment(this_object()) : 0; }


varargs string query_name(status arg) { return name; }
string query_id()            { return name;     }
string query_alias_name()    { return alias_name;  }
string query_alias()         { return alias_name;  }
string query_alt_name()      { return alt_name;    }

string query_short()         { return short_desc;  }
string query_alt_short()     { return living_env_short; }
string query_inv_short()     { return by_living_env_short; }

string query_long()          { return long_desc+"\n"; }
string query_alt_long()      { return living_env_long;  }

string query_extra_long()    { return extra_long;  }
string query_alt_extra_long(){ return alt_extra_long; }

string query_info()          { return info;        }

string query_extra_info()    { return extra_info;  }
string query_examine()       { return extra_info;  }

string query_read()          { return read_msg;    }
string query_listen()        { return listen_msg;  }
string query_smell()         { return smell_msg;   }

int query_weight()           { return weight;      }
int query_donated()          { return donated;     }
int query_value(){ if(value > 10000) value = 10001; return value; }
int query_real_light()       { return set_light(0); }   /* light available */
int query_light_value()      { return light_value; }    /* light by this */
void rto()                   { destruct(this_object()); }
int query_ego()              { return ego;        }
status query_modified()      { return modified;    }
status query_quest_item()    { return quest_item;  }
status query_domain_safe()   { return domain_safe; }
status query_sell_destruct() { return sell_destruct; }
int query_no_save()          { return no_save;       }
varargs mixed query(string fun, mixed arg1, mixed arg2) {
  return (mixed)call_other(this_object(),"query_"+fun, arg1, arg2);
}



 /************************************************************/
 /* general sets */

string set_name(string s)          { return name = s;     }
string set_id(string s)            { return name = s;     }
string set_alias_name(string s)    { return alias_name = s;  }
string set_alias(string s)         { return alias_name = s;  }
string set_alt_name(string s)      { return alt_name = s;    }

string set_short(string s)         { return short_desc = s;  }
string set_alt_short(string s)     { return living_env_short = s; }
string set_inv_short(string s)     { return by_living_env_short = s; }

string set_long(string s)          { return long_desc = s;   }
string set_alt_long(string s)      { return living_env_long = s; }

string set_extra_long(string s)    { return extra_long = s;  }
string set_alt_extra_long(string s){ return alt_extra_long = s; }

string set_info(string s)          { return info = s;        }
string set_read(string s)          { return read_msg = s;    }
string set_listen(string s)        { return listen_msg = s;  }
string set_smell(string s)         { return smell_msg = s;   }
string set_extra_info(string s)    { return extra_info = s;  }
string set_examine(string s)       { return extra_info = s;  }
int set_no_save(int i)             { return no_save = i;     }
int set_weight(int i)              { return weight = i;      }
int set_value(int i)               { return value = i;       }
int set_ego(int i)                 { return ego = i;         }
int set_donated(int i)             { return donated = i;     }
status set_modified(status s)      { return modified = s;    }
status set_quest_item(status s)    { return quest_item = s;  }

status set_sell_destruct(status s) { return sell_destruct = s; }
status set_domain_safe(status s)   { return domain_safe = (s) ? 1 : 0; }

varargs mixed set(string fun, mixed arg1, mixed arg2) {
  return (mixed)call_other(this_object(),"set_"+fun, arg1, arg2);
}

 /**************************************************************/
 /* if this is used light you can keep an eye on light sources */

int adj_light(int i) { 
  set_light(i); 
  return light_value = light_value + i;
}



 /************************************************************/
 /*  general fn for all objects */


 /* called by present() efun */

static string __IDfilename = explode(file_name(this_object()),"#")[0];
string query_IDfile() { return __IDfilename; }

status id(string str) {
  return ((name       && str == lower_case(name)) ||
          (name       && str == name) ||
          (alias_name && str == alias_name) ||
          (alt_name   && str == alt_name) ||
          (__IDfilename && str == __IDfilename));
}

string short(status wiz) {
  object env;

   if(!short_desc && this_player()->query_security_level()){
    if(name) {  
      return "invis-obj-> "+name+"-> "+file_name(this_object());
    }
    else {
      return "invis-obj-> "+file_name(this_object());
    }
  }
  else {
    env = environment();  
    if(env && (living_env_short || by_living_env_short) && living(env)) {
      if(by_living_env_short && this_player() == env) 
        return process_msg(by_living_env_short);
      else if(living_env_short)
        return process_msg(living_env_short);
      else if(short_desc)
      return process_msg(short_desc);
    }
    else {
      if(short_desc)
        return process_msg(short_desc);
      else return "";
    }
  }
}


void set_no_steal_flag(int i)  { no_steal_flag = i; }
status query_no_steal_flag()   { return no_steal_flag; }

void long(status wiz) { 
  object env;

  if(!long_desc) {
    if(!short_desc) {
      if(wiz) 
        write("No Description: "+file_name(this_object())+"\n");
    }
    else {
        write((string)this_object()->short(wiz)+"\n");
    }
  }
  else {
    env = environment();  
    if(env && living_env_long && living(env)) 
      write(process_msg(living_env_long));
    else 
    write(WHITE_F+process_msg(long_desc)+NORM);
  }
}



 /* added to long of container|room */

void extra_long(status wiz) { 
  if(alt_extra_long && environment(this_object()) == this_player()) {
    write(process_msg(alt_extra_long));
  }
  else if(extra_long) {
    write(BROWN_F+process_msg(extra_long)+NORM);
  }
}



void extra_info(status wiz) {
  if(extra_info) {
    write(process_msg(extra_info));
  }
  else {
    this_object()->long(wiz);
  }
}

status listen(string str) {
  if(str && !id(str)) return 0;
  if(!str && (string)this_object()->query_object_type() != "room") return 0;
  write(process_msg(listen_msg));
  return 1;
}

status smell(string str) {
  if(str && !id(str)) return 0;
  if(!str && (string)this_object()->query_object_type() != "room") return 0;
  write(process_msg(smell_msg));
  return 1;
}


void init() {
  object ob;

  if(!create_room) {
    while(1) {
      if(ob) {
        if(environment(ob)) {
          ob = environment(ob);
        }
        else {
          create_room = file_name(ob);
          break;
        }
      }
      else {
        if(environment(this_object())) {
          ob = environment(this_object());
        }
        else {
          create_room = file_name(this_object());
          break;
        }
      }
    }
  }
  if(read_msg) add_action("read","read");
  if(listen_msg) add_action("listen","listen");
  if(smell_msg) add_action("smell","smell");
}



 /***********************************************************************/
 /* show inventory */

#define MAX_INV_ITEMS 100
varargs status show_inventory(object ob) {
  string pad, list, *e_ob, *sh, sht;
  object *inv;
  int size, i, len, mon;
  status wiz;

   /* list of contents */

  if(!ob) ob = this_object();
  wiz = (this_player()->query_security_level()) ? 1 : 0;
  pad = "";
  list = YELLOW_F+((string)ob->query_object_type() == "room")
       ? ""
       : (!ob->query_name())
       ? "Inventory: "
       : (!living(ob))
       ? "The "+ (string)ob->query_name() +" contains: "
       : (ob == this_player()) 
      ? "You are carrying:\n\n"
       : capitalize((string)ob->query_name())+" is carrying:\n\n" + NORM;

  len = strlen(list);
  inv = all_inventory(ob);
  size = sizeof(inv);
  if(size > MAX_INV_ITEMS) size = MAX_INV_ITEMS;
  for(i = 0; i < size; i++) {
    if(inv[i] == this_player()) continue;
    if(!inv[i]) continue;
    inv[i]->extra_long();      /* between long and inventory */
    if((sht = (string)inv[i]->short(wiz))) {
      sh = explode(sht," ");
      if(sizeof(sh)) {
        sht = implode(sh," ");      
      if(!inv[i]) continue;
      if(inv[i]->query_worn() || inv[i]->query_wielded()) continue;
      list += (strlen(list) == len)
             ? sht +"\n"
             : sht +"\n";
      }

      else if(this_player()->query_security_level()) {
        list += (strlen(list) == len)
             ? "no short->"+ file_name(inv[i]) +"\n"
             : "no short->"+ file_name(inv[i]) +"\n";
      }
    }
      else if(this_player()->query_security_level()) {
      list += (strlen(list) == len)
           ? "invis-> "+file_name(inv[i]) +"\n"
           : "invis-> "+file_name(inv[i]) +"\n";
    }
  }

  if((e_ob = (string *)ob->query_extra_objects())) {
    for(i = 0; i < sizeof(e_ob); i += 2) {
      list += size + capitalize(e_ob[i]);
    }
  }
  ob->exits();  /* between extra-long and inventory */
  if(this_player() == ob) {
    mon = (int)ob->query_money();
    write(((!mon)
           ? "Your money pouch is empty.\n"
           : "You have "+
         ((MONEY->convert(mon)) ? (string)MONEY->convert(mon) : (mon +"")) +
           " coins in your pouch\n"));
  }
  write(((strlen(list) != len) ? list 
  : (ob == this_player()) ? YELLOW_F"You aren't carrying anything.\n"NORM : ""));
  if(size != sizeof(inv)) write("**** Truncated ***\n");
  return 1;
}

 /***********************************************************************/
 /* show equipment */


varargs status show_equipment(object ob) {
  string pad, list, *e_ob, *sh, sht;
  object *inv;
  int size, i, len, mon;
  status wiz;

   /* list of contents */

  if(!ob) ob = this_object();
  wiz = (this_player()->query_security_level()) ? 1 : 0;
  pad = "";
  list = (ob == this_player()) ? RED_F"Your inventory listing is:\n\n"
          : RED_F+capitalize((string)ob->query_name())+"'s equipment listing:\n\n";
  list += NORM;
  len = strlen(list);
  inv = all_inventory(ob);
  size = sizeof(inv);

  if(size > 40) size = 40;
  for(i = 0; i < size; i++) {
    if(inv[i] == this_player()) continue;
    if((sht = (string)inv[i]->short(wiz))) {
      sh = explode(sht," ");
      if(sizeof(sh)) {
        sht = implode(sh," ");      
      if(inv[i]->query_worn() || inv[i]->query_wielded())
      list += (strlen(list) == len)
             ? sht +"\n"
             : sht +"\n";
      } else
          if(inv[i]->query_worn() || inv[i]->query_wielded())
            list += "<Something> (worn somewhere)\n";
    } else 
        if(inv[i]->query_worn() || inv[i]->query_wielded())
          list += "<Something> (worn somewhere)\n";
  }

  write(((strlen(list) != len) ? list 
  : ((ob == this_player()) ? RED_F"You aren't wearing any equipment.\n"NORM 
    : capitalize((string)ob->query_name()) + " is not wearing any equipment.\n")));
  if(size != sizeof(inv)) write("**** Truncated ***\n");
  return 1;
}



void set_living(string str) {
  if(file_name(previous_object()) == SIMUL_EFUN
  || file_name(previous_object()) == SPARE_SIMUL_EFUN_FILE) {
    set_living_name(str);
  }
}



