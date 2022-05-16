// By Bubbs.

#include <wildcard.h>

status main(string str) {
  string *keys;
  int index, size;
  if (!str) {
    mapping aliases;
    aliases = (mapping)this_player()->query_aliases();
    keys = sort_array(m_indices(aliases), #'>);
    size = sizeof(keys);
    printf("%s alias%s set.\n",
     size ? to_string(size) : "No", size == 1 ? "" : "es");
    for(index = 0; index < size; index++)
     printf("%-20s  %s\n", keys[index], aliases[keys[index]]);
    return 1;
    }
  else if (str[0..2] == "-w ") {
    keys = set_of(sum_array(map_array(explode(str[3..], " "),
     #'match_wildcard,
     m_indices((mapping)this_player()->query_aliases()))));
    size = sizeof(keys);
    map_array(keys, symbol_function("delete_aliases", this_player()));
    printf("%s alias%s deleted.\n",
     size ? to_string(size) : "No",
     size == 1 ? "" : "es");
    return 1;
    }
  keys = explode(str, " ");
  size = sizeof(keys);
  for(index = 0; index < size; index++)
   printf((status)this_player()->delete_aliases(keys[index]) ?
    "Alias \"%s\" unset.\n" : "No such alias: \"%s\".\n",
    keys[index]);
  return 1;
  }

void help() {
  write("Usage: unalias [<alias list> | -w <alias-spec>]\n"
        "Delete one or more aliases.\n"
        "If the \"-w\" flag is passed, then the\n"
        "wildcards \"*\" and \"?\" can be used.\n"
        "See also: alias, history.\n");
  return;
  }

int clean_up(int arg) { destruct(this_object()); }
