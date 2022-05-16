#include "find_ob.h"

#define LOG_CALLS    "$CALL_CMD"

#define CALL_SILENT  0x001

#define USAGE        "Usage: call [-s] <obj> <func> [<args...>]\n"
#include <mudlib.h>

status main(string str) {
  mixed retval, *args;
  object ob;
  string opts, obdesc, func, argdesc, retfmt;
  int options;
  if (this_interactive() != this_player()) {
    printf("Illegal (forced) attempt to 'call %s' by %s.\n",
     str ? str : "",
     capitalize((string)this_interactive()->query_real_name()));
    return 1;
    }
  if (!str)
   return
    notify_fail(USAGE);
  if (sscanf(str, "-%s %s", opts, str) != 2)
   opts = "";
  if (sscanf(str, "%s %s", obdesc, func) != 2)
   return
    notify_fail(USAGE);
  ob = find_ob(obdesc);
  if (!ob)
   return
    notify_fail(sprintf("call: Could not find object \"%s\".\n", obdesc));
  for(; opts != ""; opts = opts[1..])
   switch(opts[0]) {
     case 's':  options |= CALL_SILENT;                           break;
     default :
       printf("call: Unknown option '%c'.\n", opts[0]);
     break;
     }
  if (sscanf(func, "%s %s", func, argdesc) == 2) {
    string *strargs;
    int index, size;
    strargs = explode(argdesc, ", ");
    size = sizeof(strargs);
    args = allocate(size);
    for(index = 0; index < size; index++)
     switch(strargs[index][0]) {
       case '"': args[index] = (string)strargs[index][1..];   break;
       case '#': args[index] = (int)strargs[index][1..];      break;
       case '.': args[index] = to_float(strargs[index][1..]); break;
       case '*':
       case '@': args[index] = find_ob(strargs[index][1..]);  break;
       default:
         if ((int)strargs[index] || strargs[index] == 0)
          args[index] = (int)strargs[index];
         else
          args[index] = strargs[index];
       }
    }
  else
   argdesc = "", args = ({ });
   log_file("/MAIN/CALLS",
        " "+
    (string)this_player()->query_real_name()+" "+
     ob+" "+
    str+"\n");
  retval = (mixed)apply(#'call_other, ob, func, args);
  if (options & CALL_SILENT)
   return 1;
  }

void help() {
  write(USAGE
        "Call a function in an object.\n");
  help_find();
  write("Any number of arguments can be passed, seperate them\n"
        "by \", \".  Unless otherwise declared, the type\n"
        "of argument is assumed to be an int (if numerical)\n"
        "or a string (otherwise).  To force the type to\n"
        "something other than the default, prepend the\n"
        "argument with a code from the following list:\n"
        "\"<arg>       Argument <arg> is a string.\n"
        "#<arg>       Argument <arg> is a number.\n"
        ".<arg>       Argument <arg> is a number (floating point).\n"
        "@<arg>       Argument <arg> is an object (see above).\n"
        "If the \"-s\" option is given, the result is not printed.\n"
        "There is an environmental variable 'call_fmt' that\n"
        "controls the format of the output from this command.\n"
        "The codes %O, %F, %A and %R are used for the\n"
        "filename of the called object, the function called,\n"
        "the arguments, and the return value respectively.\n"
        "The default is \"OBJ(%O)->%F(%A) == %R\".\n");
  return;
  }
