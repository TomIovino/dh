#include <find_ob.h>

status main(string str) {
  object where;
  string *inhs;
  int size;
  if (!str)
   return
    notify_fail("Usage: inhs <object>\n");
  where = find_ob(str);
  if (!where)
   return
    notify_fail("inhs: Object not found.\n");
  size = sizeof(inhs = inherit_list(where));
  if (!size)
   printf("\"%s\" <%O> doesn't inherit any file.\n", str, where);
  else
   printf("\"%s\" <%O> inherits %d file%c:\n%s\n",
    str, where, size, size == 1 ? 0 : 's', implode(inhs, "\n"));
  return 1;
  }

void help() {
  write("Usage: inhs <object>\n"
        "List what files an object inherits/is cloned from.\n");
  help_find();
  return;
  }
