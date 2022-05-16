
// Rewritten by Bubbs.  11/Jul/94.

static closure print_hb,
               match_creator;

#include <find_ob.h>
#include <mudlib.h>

void reset(status arg) {
  if (!arg) {
    print_hb = lambda(({ 'ob }),
     ({ #'printf, "%-38s  Creator: %s\n",
       ({ #'sprintf, "[%O]", 'ob }),
       ({ #'?,
         ({ #'=, 'cre, ({ #'creator, 'ob }) }),
         ({ #'capitalize, 'cre }),
         "LPmud"
       })
     }));
    match_creator = lambda(({ 'ob, 'cre }),
     ({ #'==, 'cre, ({ #'creator, 'ob }) }));
    }
  return;
  }

status main(string str) {
  object *obs;
  int s;
  obs = heart_beat_info();
  s = sizeof(obs);
  if (str) {
    if (str[0..1] == "-c") {
      string cre;
      cre = lower_case(trimstr(str[2..]));
      if (cre == "me")
       cre = (string)this_player()->query_real_name();
      cre = (cre == lower_case(MUD_NAME) || cre == "lib" ? 0 : cre);
      obs = filter_array(obs, match_creator, cre);
      }
    else {
      object ob;
      ob = find_ob(str);
      if (!ob)
       return
        notify_fail(sprintf("hbinfo: Failed to find object %O.\n", str));
      else
       obs = obs & ({ ob });
      }
    }
  printf("%s: %s object%c with heart_beats on "MUD_NAME".\n",
   ctime(time())[11..15],
   s ? to_string(s) : "No",
   s == 1 ? 0 : 's');
  map_array(obs, print_hb);
  return 1;
  }

void help() {
  write("Usage: hbinfo [-c <creator> | <object>]\n"
        "No argument gives a list of all objects with heart_beats,\n"
        "The \"-c <creator>\" variant will show all objects created\n"
        "the named wizard with heart beats.  Otherwise, find the\n"
        "object described by <object>, and check that for a heart\n"
        "beat.\n");
  return;
  }
