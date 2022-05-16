#include <mudlib.h>
#include <udp.h>
#include <wildcard.h>

#define M_DISPLAY  0x01
#define M_MORE     0x02
#define M_PINGALL  0x04
#define M_STATUS   0x08

static closure get_mud_info;

void reset(status arg) {
  if (!get_mud_info)
   get_mud_info = lambda(({ 'mud, 'hosts, 'count_up }),
    ({ #',,
      ({ #'=, 'time, ({ #'[, ({ #'[, 'hosts, 'mud }), HOST_STATUS }) }),
      ({ #'?,
        ({ #'>, 'time, 0 }),
        ({ #'+=, 'count_up, 1 }),
      }),
      ({ #'sprintf, "%-18s %s %s\n",
        ({ #'[, ({ #'[, 'hosts, 'mud }), HOST_NAME }),
        ({ #'?, 'time,
          ({ #'extract,
            ({ #'ctime,
              ({ #'?,
                ({ #'>, 'time, 0 }),
                'time,
                ({ #'negate, 'time }),
              }),
            }),
            11, 15
          }),
          "     ",
        }),
        ({ #'?,
          ({ #'>, 'time, 0 }), "UP",
          ({ #'<, 'time, 0 }), "DOWN",
          "UNKNOWN",
        }),
      }),
    }));
  }

status main(string str) {
  mapping hosts;
  string *muds, *outp, header, opts, fmt;
  int options, count_up, size;
  if (str && str[0] == '-') {
    if (sscanf(str, "-%s %s", opts, fmt) != 2)
     opts = str[1..];
    for(; opts != ""; opts = opts[1..])
     switch(opts[0]) {
       case 'd': options |= M_DISPLAY;                            break;
       case 'm': options |= M_MORE;                               break;
       case 'p': options |= M_PINGALL;                            break;
       case 's': options |= M_STATUS;                             break;
       default : printf("muds: Unknown option '%c'.\n", opts[0]); break;
       }
    }
  else if (str)
   fmt = str;
  if (!opts)
   options = M_DISPLAY;
  if ((options & M_PINGALL) && this_player()->query_security_level()) {
    (void)INETD->startup();
    write("Sending \"ping\" request to all known muds.\n");
    }
  count_up = 0;
  hosts = (mapping)INETD->query("hosts");
  size = sizeof(muds = m_indices(hosts));
  outp = map_array(sort_array(muds, #'>),
   get_mud_info, hosts, &count_up);
  header = sprintf(LOCAL_NAME" is connected to %s mud%c (%s up).",
   size ? to_string(size) : "no",
   size == 1 ? 0 : 's',
   count_up == size ?
    "all" : (count_up == 0 ? "none" : to_string(count_up)));
  if ((options & M_STATUS) || sizeof(muds) == 0) {
    printf("%s\n", header);
    return 1;
    }
  if (fmt) {
    muds = set_of(sum_array(map_array(explode(fmt, " "),
     #'match_wildcard,
     muds)));
    outp = map_array(sort_array(muds, #'>),
     get_mud_info, hosts, 0);
    }
  if (options & (M_DISPLAY | M_MORE)) {
    string output;
    output = sprintf("%-80#s", implode(outp, ""));
    if (options & M_DISPLAY)
     printf("%s\n%s\n", header, output);
    else {
      object pager;
      pager = clone_object("obj/pager2");
      size = to_int((string)this_player()->query_env_var("page_opts"));
      if (size)
      (void)pager->set_options(size);
      size = to_int((string)this_player()->query_env_var("page_leng"));
      if (size)
      (void)pager->set_page_length(size);
      (void)pager->set_quit_func(this_object(), "quit_pager");
      (void)pager->more_text(explode(output, "\n"), header);
      }
    }
  return 1;
  }

void help() {
  write("Usage: muds [-dmps] [<mud name format>]\n"
        "List connected muds and up/down status.\n"
        "Options available are:\n"
        "    d    Display output normally.\n"
        "    m    Display output through pager.\n"
         "    p    Ping all muds on the host list.\n"
        "    s    Show status line only.\n");
  return;
  }

void quit_pager(string reason, object pager, mixed extra) {
  if (pager)
   destruct(pager);
  write(reason == "END" ? "End of muds.\n" : "Ok.\n");
  return;
  }
