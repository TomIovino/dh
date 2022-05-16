 // By Bubbs.

#include <mudlib.h>
#undef PAGER

#define PAGER "/secure/bim/pager"
#define MAX_ALIASES 65

#include <wildcard.h>
closure view_aliases;

void reset(status arg) {
  if (!arg) {
    view_aliases = lambda(({ 'al, 'als }),
     ({ #'sprintf,
       "%-20s  %-55=s\n",
       'al,
       ({ #'[, 'als, 'al })
     }));
    }
  return;
  }

status main(string str) {
  string var, val;

    mapping aliases;
    string *keys, header, *outp;
    int a, b;
    aliases = (mapping)this_player()->query_aliases();
    a = sizeof(aliases);
    keys = m_indices(aliases);
     if (query_once_interactive(this_player()) == 0) return 0;
     if (!this_player()->query_security_level())
     if (sizeof(aliases) >= MAX_ALIASES)
   {
    if(str){
write("You have too many aliases, please remove some before making more.\n");
    return 1;
       }
    }

    if (!str) str = "-w";
  if (str == "-w" || str[0..2] == "-w " ||
      str == "-m" || str[0..2] == "-m ") {

    if (str[0..2] == "-m " || str[0..2] == "-w ") {
      keys = set_of(sum_array(map_array(explode(str[3..], " "),
       #'match_wildcard,
       keys)));
      }
    b = sizeof(keys = sort_array(keys, #'>));
    header = sprintf("%s alias%s defined%s.\n",
     a ? to_string(a) : "No",
     a == 1 ? "" : "es",
     b == a ? "" : sprintf(" (%d displayed)", b));
    outp = map_array(keys, view_aliases, aliases);
    if (str[0..1] == "-m") {
      object pager;
      pager = clone_object(PAGER);
      (void)pager->set_quit_func(this_object(), "quit_more");
      a = to_int((string)this_player()->query_env_var("page_opts"));
      if (a)
       (void)pager->set_options(a);
      a = to_int((string)this_player()->query_env_var("page_leng"));
      if (a)
       (void)pager->set_page_length(a);
      (void)pager->more_text(map_array(outp, #'[..<], 0, 2),
       sprintf(MUD_NAME": %s", header[0..<2]));
      }
    else {
      write(header);
      map_array(outp, #'write);
      }
    return 1;
    }
  if (sscanf(str, "%s %s", var, val) != 2) {
    if ((status)this_player()->defined_aliases(str))
     printf("%-20s  %s\n",
      str, (string)this_player()->query_aliases(str));
    else
     printf("No such alias: \"%s\".\n", str);
    return 1;
    }
  if (strstr(val, "$") == -1)
   val = sprintf("%s $*", val);
  printf((status)this_player()->defined_aliases(var) ?
   "Alias \"%s\" changed to \"%s\".\n" :
   "Alias \"%s\" set to \"%s\".\n",
    var, val);
  (void)this_player()->set_aliases(var, val);
  return 1;
  }

void help() {
  write("Usage: alias [<alias> [<defn>] | -m [<fmt>] | -w <fmt>]\n"
        "View, or define an alias.\n"
        "The options '-m' and '-w' will display aliases defined.\n"
        "In both cases, a format can be given, which may include\n"
        "the wildcards '*' and '?'.  The '-m' option will display\n"
        "the output through the pager.\n"
        "When setting aliases, there are a number of variables\n"
        "that are handled by the alias processor.\n"
        "  $*  is replaced by the whole argument given to the alias,\n"
        "  $<num>  is replaced by the <num>th argument,\n"
        "  $<num>+ is replaced by all arguments from <num> onwards.\n"
        "\n"
        "If no argument code is given in the alias, then \" $*\"\n"
        "is appened to it.  Defaults can be given to any missing\n"
        "arguments like so:\n"
        "  $3{3rd}    (means third argument, default to \"3rd\")\n"
        "Examples:\n"
        "  alias gsl grin $1 smile $1 laugh $1\n"
        "  alias x look at $*\n"
        "  alias g grin $*{mischievously}\n"
        "  alias yas say $5 $4 $3 $2 $1 $6+\n"
        "  alias gf get $1{all} from $2{corpse} $3+\n"
        "  alias t tell          (is defined as \"tell $*\")\n"
        "\n"
        "See also: unalias, handy hint.\n");
  return;
  }

void quit_more(string reason, object pager, mixed *extra) {
  write("Ok.\n");
  destruct(pager);
  return;
  }

int clean_up(int arg) { destruct(this_object()); }
