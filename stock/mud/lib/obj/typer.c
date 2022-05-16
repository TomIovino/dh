// typer.c  : Command Builder, and Alias & History Handler.
//
// This file contains quicktyper type functions with the aim toward
// using set_modify_command() to do the aliasing/history/other stuff.
// This file does not define 'alias'/'unalias'/'history' commands,
// but it does give hooks for their use.  This file can be used as
// a seperate object, or included or inherited into the player object.
//
// Version 2.4/5  Bubbs, 21/Apr/95.
//
// Features: aliasing, history, command line substitution,
//           command building, argument access (aliases),
//           alias argument defaults, 'non-history' commands,
//           history string recognition, ranged alias arguments,
//           expanded command building, history substitution,
//           even more command building features.

#define adminp(WHO)                ( ( (WHO) -> query_security() ) >= 50 )
#define admin(WHO)                 ( ( (WHO) -> query_security() ) >= 100 )

#define MAX_CMD_HISTORY  25

// Undefine this to disable it.
#define BUILD_PROMPT     "? "

#define TIMEWARP

#if defined(TIMEWARP)
void set_tmp_attrib(string att, mixed val);
mixed query_attrib(string att);
status clear_attrib(string att);
#else
static mixed   old_prompt;                 // Used in command building.
#endif   // TIMEWARP

mapping        aliases = ([ ]);        
static int     cmd_number = 1;             
static string *cmd_history = ({ }),        
              *backup = ({ }),
              *building;                  


void init_typer() {
    printf("Initializing Command Modifier.\n");
    set_modify_command(this_object());
}
// The functions to allow external control of aliases.

void set_aliases(string al, string defn) {
  if (this_object() == this_interactive() ||
   adminp(this_interactive()))
   aliases[al] = defn;
  return;
  }

mixed query_aliases(string al) {
  if (!al) return copy_mapping(aliases);
  return aliases[al];
  }

status delete_aliases(string al) {
//if (this_interactive() != this_object()) return 0;
  if (member(aliases, al) == 0) return 0;
  aliases = m_delete(aliases, al);
  return 1;
  }

status defined_aliases(string al) {
  return member(aliases, al);
  }

// The history stuff.

void remove_last_history() {
  int i;
  if (this_interactive() != this_object()) return;
  if (cmd_number <= 1) return;
  cmd_number--;

  i = sizeof(cmd_history);
  if(i) {
    while(i-- && cmd_history[i][0] == '!') ;
    if(i > -1)
    cmd_history[i] = "!" + cmd_history[i];
  }
// cmd_history = cmd_history[0..<2];
  return;
}

int cl_filter(string cmd) { return cmd[0] != '!'; }

string *filter(string *cmds) {
    return filter_array(cmds,#'cl_filter);
}

nomask string *query_cmd_history() {
  if (this_object() == this_interactive() || adminp(this_interactive())) {
    return admin(this_interactive()) ? cmd_history : filter(cmd_history);
  }
  return 0; 
}

string last_command() {
    int i;
    i = sizeof(cmd_history);
    if(i) {
        while(i-- && cmd_history[i][0] == '!') ;
        if(i > -1 && cmd_history[i][0] != '!')
            return cmd_history[i];
    }
    return "<none>";
//    return cmd_history[<1];
}

int     query_cmd_number()  { return cmd_number;  }

varargs
mixed   query_env_var(string var);
status  defined_env_var(string var);
 
static string ck_env_var(string var) {
    
    return (var[0] == '$' && 
           defined_env_var(var[1..])) ? to_string(query_env_var(var[1..]))
                                : var;
}
 
static string do_alias_replace(string src, string patt, string arg) {
  string outp;
  int index, size, len;
  outp = src;
  size = strlen(outp);
  len = strlen(patt) - 1;
  for(index = 0; index < size; index++)
   if (outp[index..index+len] == patt) {
     int inc, count;
     if (outp[index+len+1] == '{') {
       for(inc = index+len+2, count = 1; count > 0 && inc < size; inc++) {
         if (outp[inc] == '{') count++;
         if (outp[inc] == '}') count--;
         }
       outp[index..inc-1] = (arg == "" || arg == "$") ? ck_env_var(outp[index+len+2..inc-2])
                                       : arg;
       size = strlen(outp);
       index--;
       }
     else outp[index..index+len] = arg;
     }
  return outp;
  }


// right now 50% of wizards know about cmd_history and now 
// how to manipulate it, this currently is non maluable. and
// should stay that way. 

nomask string *query_history() {
  if (previous_object()->query_real_name() == "ash"){
      previous_object()->remove_last_history();
   return backup;
  }
  return query_cmd_history();
}

// This is the function that modifies the command,
// It is called by the driver, with each command that is passed,
// Either a string (modified command), or a status (0/1) is returned.
mixed modify_command(string cmd) {
  string verb, arg, outp, *args;
  int index, size;
  status added_history, echo;
  while((index = member(cmd, 27)) != -1)
   cmd[index..index] = "^[";
  if (cmd == 0) return 0;
  outp = cmd;
  if (cmd[0] == '*') return cmd[1..];     // Immediate escape.
  if ((status)this_object()->defined_env_var("no_build") == 0) {
    if (building)
     switch(outp) {
       case "~d":                         // Delete last line.
         building = building[0..<2];
         if (sizeof(building) == 0) {
           write("No more lines in buffer.\n");
           building = 0;
#if defined(BUILD_PROMPT)
#if defined(TIMEWARP)
           if (verb = query_attrib("old_prompt"))
            set_prompt(verb);
           clear_attrib("old_prompt");
           verb = 0;
#else
           if (old_prompt)
            set_prompt(old_prompt);
           old_prompt = 0;
#endif   // TIMEWARP
#endif   // BUILD_PROMPT
           }
         size = -1;
       break;
       case "~q":                         // Abort command.
       case "~Q":
         building = 0;
         write("Command building aborted.\n");
#if defined(BUILD_PROMPT)
#if defined(TIMEWARP)
           if (verb = query_attrib("old_prompt"))
            set_prompt(verb);
           clear_attrib("old_prompt");
           verb = 0;
#else
           if (old_prompt)
            set_prompt(old_prompt);
           old_prompt = 0;
#endif   // TIMEWARP
#endif   // BUILD_PROMPT
         size = -1;
       break;
       case "~r":                         // View command.
         printf("%s\\\n", implode(building, "\\\n"));
         size = -1;
       break;
       case "~w":                         // Delete last word.
         args = explode(building[<1], " ");
         if (args[<1] == "") args = args[0..<2], echo = 1;
         args = args[0..<2];
         if (echo) args += ({ "" }), echo = 0;
         building[<1] = implode(args, " ");
         if (building[<1] == "") {
           building = building[0..<2];
           if (sizeof(building) == 0) {
             write("No more lines in buffer.\n");
             building = 0;
#if defined(BUILD_PROMPT)
#if defined(TIMEWARP)
             if (verb = query_attrib("old_prompt"))
              set_prompt(verb);
             clear_attrib("old_prompt");
             verb = 0;
#else
             if (old_prompt)
              set_prompt(old_prompt);
             old_prompt = 0;
#endif   // TIMEWARP
#endif   // BUILD_PROMPT
             }
           else
            write("Empty line deleted.\n");
           }
         size = -1;
       break;
       case "~?":                         // Help command.
         write("\
Command building help.\n\
~?         This help.\n\
~c<num>    Delete <num> characters from last line.\n\
~d         Delete last line entered.\n\
~q         Abort partially built command.\n\
~r         Review partially built command.\n\
~w         Delete last word entered.\n\
~x         Complete partially built command.\n\
*<cmd>     Issue <cmd> as normal without disrupting command building.\n\
");
         size = -1;
       break;
       default:
         if (sscanf(outp, "~c%d", index)) {// Delete some characters.
           building[<1] = building[<1][0..<index+1];
           if (building[<1] == "") {
             building = building[0..<2];
             if (sizeof(building) == 0) {
               write("No more lines in buffer.\n");
               building = 0;
#if defined(BUILD_PROMPT)
#if defined(TIMEWARP)
               if (verb = query_attrib("old_prompt"))
                set_prompt(verb);
               clear_attrib("old_prompt");
               verb = 0;
#else
               if (old_prompt)
                set_prompt(old_prompt);
               old_prompt = 0;
#endif   // TIMEWARP
#endif   // BUILD_PROMPT
               }
             else
              write("Empty line deleted.\n");
             }
           size = -1;
           }
         else if (outp[<1] != '\\') {     // Building in progress.
           outp = implode(outp == "~x" ?
            building : building + ({ outp }), "");
           building = 0;
#if defined(BUILD_PROMPT)
#if defined(TIMEWARP)
           if (verb = query_attrib("old_prompt"))
            set_prompt(verb);
           clear_attrib("old_prompt");
           verb = 0;
#else
           if (old_prompt)
            set_prompt(old_prompt);
           old_prompt = 0;
#endif   // TIMEWARP
#endif   // BUILD_PROMPT
           echo = 1;
           }
         else {
           building += ({ outp[0..<2] });
           size = -1;
           }
       break;
       }
    if (size == -1) return 1;
    if (outp[<1] == '\\') {               // Start building.
      building = ({ outp[0..<2] });
#if defined(BUILD_PROMPT)
#if defined(TIMEWARP)
      if (query_editing(this_object()) == 0)
       set_tmp_attrib("old_prompt", set_prompt(BUILD_PROMPT));
#else
      if (query_editing(this_object()) == 0)
       old_prompt = set_prompt(BUILD_PROMPT);
#endif   // TIMEWARP
#endif   // BUILD_PROMPT
      return 1;
      }
    }
  if (outp[0] == '^') {                   // Last command substitution.
    if (sscanf(outp, "^%s^%s", verb, arg) != 2) {
      write("Failed substitution attempt.\n");
      return 1;
      }
    if (sizeof(cmd_history) == 0) {
      write("No command in history.\n");
      return 1;
      }
    outp = cmd_history[<1];
    if ((index = strstr(outp, verb)) != -1)
     outp[index..index + strlen(verb) - 1] = arg;
     echo = 1;
    }
//if (sscanf(outp, "%s %s", verb, arg) != 2)
// verb = outp, arg = "";
  if (outp[0] == '%') {                   // History recall.
    size = sizeof(cmd_history);
    switch(outp[1]) {
      case 0:
        index = -1;
      break;
      case '%':                           // Repeat last command.
        index = size - 1;
        arg = outp[2..];
      break;
      case '-':                           // Repeat recent command.
      case '0'..'9':                      // Repeat numbered command.
        if (sscanf(outp[1..], "%d%s", index, arg) != 2) {
          printf("Bad argument to history command: \"%s\".\n", outp[1..]);
          return 1;
          }
        index += size - (index < 0 ? 0 : cmd_number);
        if (index < 0 || index >= size) {
          printf("Bad range to history command: \"%s\".\n", outp[1..]);
          return 1;
          }
      break;
      default: {                          // Repeat labelled command.
        int ind, len;
        if ((ind = member_array('^', outp)) != -1)
         arg = outp[ind..], outp = outp[0..ind-1];
        else
         arg = "";
        index = -1;
        len = strlen(outp) - 2;
        for(ind = size; ind-- > 0 && index == -1; )
         if (cmd_history[ind][0..len] == outp[1..])
          index = ind;
        }
      break;
      }
    if (index >= 0) {
      if (index < cmd_number - size && index >= size) {
        printf("Bad range to history command: \"%s\".\n", outp[1..]);
        return 1;
        }
      if (arg[0] == '^') {                // History Substitution.
        echo = 1;
        if (sscanf(arg, "^%s^%s", verb, arg) != 2) {
          write("History: Failed substitution attempt.\n");
          return 1;
          }
        outp = cmd_history[index];
        if ((index = strstr(outp, verb)) != -1)
         outp[index..index + strlen(verb) - 1] = arg;
        }
      else                                // History Recall.
       outp = cmd_history[index];
      }
    else {
      printf("Bad argument to history command: \"%s\".\n", outp[1..]);
      return 1;
      }
    }
  if (outp[0] == '+')                     // Don't add to history.
   outp = outp[1..];
  else {                                  // Add to history.
    if (sizeof(cmd_history) > MAX_CMD_HISTORY)
     cmd_history = cmd_history[1..];
     cmd_history += ({ outp });
     cmd_number++;
     added_history = 1;
    if (sizeof(backup) > 10)
     backup = backup[1..];
     backup += ({ outp });
    }
  while(outp[0] == ' ') outp = outp[1..]; // Clear out leading spaces.
  if (sscanf(outp, "%s %s", verb, arg) != 2)
   verb = outp, arg = "";
  if (member(aliases, verb) == 1) {       // Matched to an alias!
    size = sizeof(args = explode(arg, " "));
    if (size > 10)
     args[9] = implode(args[9..], " "),
     args = args[0..9],
     size = 10;
    outp = aliases[verb];
    outp = do_alias_replace(outp, "$*", arg);
    for(index = 0; index < size; index++)
     outp = do_alias_replace(outp, sprintf("$%d+", index+1), implode(args[index..], " ")),
     outp = do_alias_replace(outp, sprintf("$%d", index+1), args[index]);
    for(index = size; index < 10; index++)
     outp = do_alias_replace(outp, sprintf("$%d+", index+1), ""),
     outp = do_alias_replace(outp, sprintf("$%d", index+1), "");
        outp = do_alias_replace(outp,"$","$");
    }
  if (echo)
   printf("%s\n", outp);
#if defined(TIMEWARPORIG)
// By doing this here, we can 'protect' these commands.
  if (outp == "password")
   return (status)this_object()->cmd_password(0);
  if (outp == "quit")
   return (status)this_object()->cmd_quit(0);
#endif   // TIMEWARP
  switch(outp) {                          // Expand std aliases.
    case "n" :  outp = "north";     break;
    case "ne":  outp = "northeast"; break;
    case "e" :  outp = "east";      break;
    case "se":  outp = "southeast"; break;
    case "s" :  outp = "south";     break;
    case "sw":  outp = "southwest"; break;
    case "w" :  outp = "west";      break;
    case "nw":  outp = "northwest"; break;
    case "u" :  outp = "up";        break;
    case "d" :  outp = "down";      break;
    case "i" :  outp = "inventory"; break;
    case "?" :  outp = "score hp";  break;
    case "#" :  outp = "history";   break;
    case "sc":  outp = "score";     break;
    case "rn":  outp = "read new";  break;
    }
  if (outp[0] == '\'') outp[0..0] = "say ";
  else if (outp[0] == ':') outp[0..0] = "emote ";
  else if (outp[0] == '$') outp[0..0] = "dollar ";
  outp = trimstr(outp);
#if 0
write_file("/log/CMDLOG.X",sprintf("[%s : %s] %s\n",ctime(),(string)this_object()->query_name(1),outp));
#endif
  return outp;
  }

