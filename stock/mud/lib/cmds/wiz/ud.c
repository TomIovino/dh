// Written by Bubbs.  Summer/93.
// -s, -l, -r flags added.      16/Mar/94.
// front end rewritten,
// -u option added.              9/Jan/95.
// -z option added.             14/Jan/95.
// Merged 'update' and 'udin'.   3/Feb/95.

string *query_cmd_aliases() { return ({ "update" }); }

#define UD_INHERITED 0x001    // -i
#define UD_ROUGH     0x002    // -r
#define UD_STRICT    0x004    // -s
#define UD_UNLOAD    0x008    // -u
#define UD_LAZY      0x010    // -z

#define VOID "/secure/obj/void"

varargs void update_obj(object udob, int options);
string *get_files(object *obs);
varargs string trimstr(string str, int char, int mode);

status main(string str) {
  object *obs;
  string opts, fspec, *files;
  int index, size, options;
  str = (str ? trimstr(str) : to_string(environment(this_player())));
  if (str[0] == '-') {
    if (sscanf(str, "-%s %s", opts, fspec) != 2)
     opts = str[1..], fspec = to_string(environment(this_player()));
    }
  else {
    opts = (string)this_player()->query_wiz_env_var("ud_opts");
    fspec = str;
    }
  if (opts)
   for(; opts != ""; opts = opts[1..])
    switch(opts[0]) {
      case 'i':  options |= UD_INHERITED;         break;
      case 'l':  options -= options & UD_STRICT;  break;
      case 'r':  options |= UD_ROUGH;             break;
      case 's':  options |= UD_STRICT;            break;
      case 'u':  options |= UD_UNLOAD;            break;
      case 'z':  options |= UD_LAZY;              break;
      case '0':  options = 0;                     break;
      default :
        printf("update: Unknown option '%c'.\n", opts[0]);
      break;
      }
  files = explode(fspec, " ");
  size = sizeof(files);
  files = map_array(explode(fspec, " "),
   symbol_function("make_path", this_player()));
  obs = map_array(files, #'find_object);
  size = sizeof(files);
  for(index = 0; index < size; index++)
   if (!obs[index]) {
     printf("update: No such object: %O.\n", files[index]);
     files = files[0..index-1] + files[index+1..];
     obs = obs[0..index-1] + obs[index+1..];
     index--;
     size--;
     }
  if (!size)
   return
    notify_fail("update: No objects found to update.\n"),0;
  if (options & UD_INHERITED) {
    files = get_files(obs);
    obs = map_array(files, #'find_object);
    obs -= ({ 0 });
    }
  filter_array(obs, #'update_obj, options);
  return 1;
  }

varargs void update_obj(object udob, int options) {
  object *conts;
  string file, error;
  int index, size;
  file = sprintf("/%s.c", file_name(udob));
  if ((options & UD_LAZY) &&
      (file_time(file) < program_time(udob))) {
    printf("Object not updated: %s.\n", file);
    return;
    }
  conts = all_inventory(udob);
  if (options & UD_STRICT)
   conts = filter_array(conts, #'query_once_interactive);
  map_array(conts, #'move_object, VOID);
  destruct(udob);
  if (options & UD_UNLOAD)
   printf("Object unloaded: %s.\n", file);
  else {
    if (options & UD_ROUGH) {
      (void)file->XXX(1);
      error = 0;
      }
    else
     error = catch((void)file->XXX(0));
    if (error)
     printf("Error in loading %s : %s", file, error[1..]);
    else {
      printf("Object reloaded successfully: %s.\n", file);
      udob = find_object(file);
      conts -= ({ 0 });
      map_array(conts, #'move_object, udob);
      }
    }
  return;
  }

void help() {
  write("Usage: ud [-ilrsuz0] [<file>] [<extra files>]\n"
        "This command will unload the image of a file (or files)\n"
        "from memory.  It will also attempt to reload those files\n"
        "afterwards.  If no file is given, the current location\n"
        "is used.  The objects in the updated object are replaced\n"
        "after the reload is done, unless there is a problem, when\n"
        "they are left in the void (/"VOID").  The default\n"
        "options can be set with the \"ud_opts\" environmental\n"
        "variable.  The options listed are:\n"
        "    i    'inherited';  Update all inherited objects also.\n"
        "    l    'lax';        All objects are replaced.\n"
        "    r    'rough';      Useful to catch errors in creation.\n"
        "    s    'strict';     Only player objects are replaced.\n"
        "    u    'unload';     The object is not reloaded,\n"
        "                       moved objects are not replaced.\n"
        "    z    'lazy';       Don't update if loaded since file last modified.\n"
        "    0    'zero';       Clear out any previously selected options.\n");
  return;
  }

// Used for the '-i' option.
string *get_files(object *obs) {
  mixed *inhs;
  string *order;
  int index, size;
  inhs = map_array(obs, #'inherit_list);
  size = sizeof(inhs);
  order = ({ });
  for(index = 0; index < size; index++) {
    int i;
    for(i = sizeof(inhs[index]); i-- > 0; )
     if (member_array(inhs[index][i], order) == -1)
      order = order + inhs[index][i..i];
    }
  return order;
  }

int clean_up(int arg) { destruct(this_object()); }

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
