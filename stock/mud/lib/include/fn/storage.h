 // Storage.h For Saving Equipment of Characters
 // Designed and Created By:
 // Dax of Dragon's Heart 7-21-95
 //
 // Optimizations By:  Geewhiz@DragonHeart 960504
 
 
#if 0
void _tell_me(string mesg) {
    object me;
    if(me = find_player("geewhiz"))
        tell_object(me,mesg);
}
#define TELL_ME(MSG) _tell_me(MSG)
#else
#define TELL_ME(MSG)
#endif
 
 
string *inventory;            /* String of Filenames for stored inventory. */
string *backup_inv;           /* Saves on Give, Drop, Get, Take. */
static object *delete_list;   /* List of Deleted Rent Items */
 
int scan_object(object ob);
void delete_inventory();
 
string query_saved_inventory() { return implode(inventory, ", "); }
string query_backup_inv() { return implode(backup_inv, ", "); }
 
string *clear_backup_inv() { return backup_inv = ({ }); }
string *clear_inventory() { return inventory = ({ }); }
 
int store_inventory() {
  return scan_object(this_object());
}
 
int restore_inventory(object player) {
  string iname;
  object obj;
  int s, i;
 
  if(!sizeof(inventory) && !sizeof(backup_inv)) {
    printf("No Inventory to retrieve!\n");
    return 1;
  }
  s = sizeof(inventory);
  if(!s) {
   printf("Restoring your inventory from backup information.\n");
    s = sizeof(backup_inv);
    for(i = 0; i < s; i++) {
      if(!catch(obj = clone_object((string)backup_inv[i])))
        move_object(obj, this_object());
    }
  } else
  for(i = 0; i < s; i++) {
    if(!catch(obj = clone_object((string)inventory[i])))
      move_object(obj, this_object());
  }
  inventory = ({ });
  return 1;
}
 
int scan_object(object ob) {
  object *obs, obj;
  int i, s;
  string filler, file;
 
  if(!ob) return 0;  // Wouldn't you like to be a pepper too?
 
  obs = deep_inventory(ob); // No Recursion Needed.  Get's *EVERYTHING*!
 
  s = 0;
  i = sizeof(obs);
  inventory = delete_list = 0;
  inventory = allocate(i);
  delete_list = allocate(i);
  while(i--) {
    obj = obs[i];
 
    if(!obj                    ||
       living(obj)             ||
       obj->id("mailer")       ||
//        obj->id("soul")         ||
       obj->id("spell")        ||
       obj->id("unique")       ||
       obj->query_auto_load()  ||
       obj->query_no_save()) {
        TELL_ME(sprintf("Skipped %O\n",obj));
        continue;  // at top of while()
    } else {
      sscanf(file_name(obj), "%s#%s", file, filler);
      inventory[s++] = file;
//      inventory[s] = file;
//      delete_list[s++] = obj;
    }
    TELL_ME(sprintf("destruct(%O);\n",obj));
  }
 
  inventory -= ({ 0 });
 
  TELL_ME(sprintf("Just saved %d things in your inventory... should be %d.\n",
                                   sizeof(inventory),s));
  TELL_ME(sprintf("%O\n",inventory));
  return 1;
}
 
int backup_scan_object(object ob) {
  object *obs, obj;
  int i, s;
  string filler, file, line;
 
  if(!ob) return 0;  // Wouldn't you like to be a pepper too?
 
  obs = deep_inventory(ob); // No Recursion Needed.  Get's *EVERYTHING*!
 
  s = 0;
  i = sizeof(obs);
  backup_inv = 0;
  backup_inv = allocate(i);
  while(i--) {
    obj = obs[i];
 
    if(!obj                    ||
       living(obj)             ||
       obj->id("mailer")       ||
//        obj->id("soul")         ||
       obj->id("party_object") ||
       obj->id("spell")        ||
       obj->id("unique")       ||
       obj->query_auto_load()  ||
       obj->query_no_save())
        continue;  // at top of while()
    else {
      sscanf(file_name(obj), "%s#%s", file, filler);
      backup_inv[s++] = file;
    }
  }
  backup_inv -= ({ 0 });
  TELL_ME(sprintf("Just saved %d things in your backup_inventory... should be %d.\n",
                                   sizeof(backup_inv),s));
  TELL_ME(sprintf("%O\n",backup_inv));
  return 1;
}
 
int do_storage_backup() {
  return backup_scan_object(this_object());
}
 
