 
#include <bar.h>
#include <mudlib.h>
#include <ansi.h>
#include <ansi_filter.h>
#include <udp.h>

 
object this_player();      // returns this_object() if this_player() == 0
 
string print_counts(int i,int g, int e, int flags);
string query_flags(object u);
varargs string who_list(int flags);
varargs string do_immortals(object *u, int cnt, int flags, int scnt);
varargs string do_mortals(object *u,int pcnt,int gcnt,int ecnt,int flags,int scnt);
string get_who_string();
void page(string data);
 
status sort_by_security(object a, object b);
status sort_by_age(object a, object b);
status sort_by_level(object a, object b);
int sort_by_alignment_rank(object a, object b);
status is_immortal(object u);
status is_mortal(object u);
status is_player(object u);
 
#define F_CLEAR     0x000                       // Clear
#define F_SHORT     0x001                       // Short Who Listing
#define F_IMMORTALS 0x002                       // security > 0
#define F_MORTALS   0x004                       // security == 0
#define F_PLAYERS   0x008                       // level <  LL
#define F_LEVELSORT 0x080                       // Sort mortals by level
#define F_LEVELS    0X100                       // Display Player levels
 
#define F_NORMAL   (F_CLEAR|F_IMMORTALS|F_MORTALS|F_PLAYERS) // Standard
 
#define FLAG(B,FLAGS) ((FLAGS) & (B))
#define SET(FLAGS,B) FLAGS |= B
#define CLEAR(B,FLAGS) FLAGS ~= B
 
status main(string arg) {
  string mud, line;
  int flags;
  flags = F_CLEAR;
  if(arg && sscanf(arg,"@%s", mud) == 1) {
        if (line = (string)INETD->send_udp(mud,
            ([ REQUEST: "who",
               SENDER: (string)this_player()->query_real_name() ]), 1)) {
                return notify_fail(line),0;
        }
        return printf("inetd:  Request Transmitted\n"),1;
  }
  if(stringp(arg) && arg != "") {
    string *argv, opts;
    int argc, i;
    argv = explode(arg," ");
    argc = sizeof(argv);
    while(argc--) {
        if(argv[argc][0] == '-')  {
            if(strlen(argv[argc]) == 1) {
                printf("who: Null option ignored.\n");
                continue;
            }
            i = strlen(opts = argv[argc][1..]);
            while(i--) {
                switch(opts[i]) {
                    case 's': 
                        SET(flags,F_SHORT);
                        break;
                    case 'i': 
                        SET(flags,F_IMMORTALS);
                        break;
                    case 'm': 
                        SET(flags,F_MORTALS);
                        break;
                    case 'p': 
                        SET(flags,F_PLAYERS);
                        break;
                    case 'E': 
                        if(this_player()->query_security()) {
                            SET(flags,F_LEVELSORT);
                            break;
                        }
                        printf("who: Unknown option '%c' ignored\n",opts[i]);
                        break;
                    case 'L':  // implies short
                        if(this_player()->query_security()) {
                            SET(flags,F_LEVELS|F_SHORT);
                            break;
                        } 
                        /* fall through */
                    default:
                        printf("who: Unknown option '%c' ignored\n",opts[i]);
                        break;
                }
            }
        } else
            printf("who: Unknown arg %O ignored\n",argv[argc]);
    }
  }
  switch(flags) { 
      case F_SHORT:
      case F_SHORT|F_LEVELSORT:
      case F_CLEAR:
      case F_LEVELSORT:
      case F_LEVELS:
      case F_LEVELS|F_LEVELSORT:
      case F_SHORT|F_LEVELS:
      case F_SHORT|F_LEVELSORT|F_LEVELS:
          SET(flags,F_NORMAL);
          break;
      default:
          break;
  }
//  printf("Flags = %2d (0x%02x)\n",flags, flags);
  page(sprintf("\n%s\n",who_list(flags))); 
  return 1;
}
 
string get_who_string() {
  return sprintf("%s\n%|78s\n",filter_ansi(who_list(F_NORMAL|F_SHORT)),
             sprintf("%s is located at %s %d",
                       MUD_NAME,query_host_name(),query_mud_port()));
 
}
 
status filter(object ob, int flags) {
    return ob
        && ob->is_player()
        && ((FLAG(F_IMMORTALS,flags) && is_immortal(ob)) 
        ||  (FLAG(F_MORTALS,  flags) && is_mortal(ob)  ) 
        ||  (FLAG(F_PLAYERS,  flags) && is_player(ob)  ) );
}

 
object *filter_users(int flags) {
    return filter_array(users(),#'filter,flags);
}    
 
varargs string who_list(int flags) {
  object *imm, *mort, *u;
  int gct, ect, ict, scnt;
  string line;
  if(!(u = filter_users(flags)) || !sizeof(u)) return "Nothing to show.\n";
  imm = filter_array(u, #'is_immortal);
  mort = u - imm; 
  scnt = 0;
  line = PRINTBAR +"\n";
  if(!FLAG(F_SHORT,flags))
  line += do_mortals(mort,&gct,&ect,flags,&scnt);
  if(FLAG(F_SHORT,flags) && scnt) {
      if(FLAG(F_LEVELS,flags))
       write("");
      else 
          while(scnt) scnt++,scnt > 3 ? (scnt=0,line+="\n") : (line += "               |  ");
  }
/*
  if(ict || gct || ect || hct) line += PRINTBAR + "\n"; 
*/
  line += print_counts(ict,gct,ect,flags);
  line += PRINTBAR;
//  printf("Strlen(line) = %d\n",strlen(line));
  return line;
}
 
#define DELIM (s--,(ret == "" ? "" : (s > 1 ? ", " : " & ")))
string print_counts(int i,int g, int e, int flags) {
    string ret;
    status si, sm, sh;
    int s, m;
    m = g + e;
    si = !!(FLAG(F_IMMORTALS,flags));
    sm = !!(FLAG(F_PLAYERS,flags));
    s = si + sm;
    ret = "";
    return sprintf("%|67s\n%-33s %33s\n",ret,
/*
    (string)"/cmds/wiz/_mudstat"->who_func_str(),
*/
    NORM,
    NORM,
    ctime()[0..<6]);
}
 
string query_flags(object u) {
    string f;
    status Idle, Editing, Busy, Dead, Snooped;
    Idle = query_idle(u) > 300;
    Editing = 1 && query_editing(u);
    Busy = 1 && query_input_pending(u);
    Dead = 1 && u->query_ghost();
    Snooped = 1 && query_snoop(u);
    if(this_player()->query_security())
    return sprintf("%s%c%c%c%c%c%s",B_RED_F,
                Idle ? 'I' : ' ',Editing ? 'E' : ' ',
                Busy ? 'B' : ' ',   Dead ? 'D' : ' ',
             Snooped ? ' ' : ' ',NORM);
    return "    ";
}
 
varargs string do_mortals(object *u,int gcnt,int ecnt,int flags,int j) {
  string s;
  int i, lev, level, secure, noble;
  status inv, short, Levels, hero;
  string t;
  s = "";
  u = sort_array(u,FLAG(F_LEVELSORT,flags) ? 
           #'sort_by_level : #'sort_by_alignment_rank);
  gcnt = ecnt = 0;
  i = sizeof(u);
  level = this_player()->query_level();
  secure = this_player()->query_security();
  if(short = FLAG(F_SHORT,flags)) Levels = FLAG(F_LEVELS,flags);
  while(i--) {
    if(!u[i] || !environment(u[i])) continue;
    if(inv = u[i]->query_invis() ) {
        if(!this_player() || (!secure && this_player() != u[i]))
            continue;  
    }
    lev = u[i]->query_level();
    if(!short) {
    noble = 0;
     s += sprintf("%s\n",NORM);
     s += sprintf("%-1s",u[i]->do_cc(""));
     s += sprintf("%-15s",u[i]->query_name());
     s += sprintf("%-10s",u[i]->query_pretitle());
     s += sprintf("%-15s",u[i]->query_house());
     s += sprintf("%-12s",u[i]->query_class());
     s += sprintf("%-10s",u[i]->query_race());
     s += sprintf("%-8s",u[i]->query_alignment()+NORM);
     s += query_flags(u[i]);

    } else {  // Short version
        if(secure && Levels)
s += sprintf("%-12.12s[%3d] %c",capitalize(u[i]->query_name(1)),
          lev,(j++,j > 3 ? (j=0,'\n') : 0));
        else
            s += sprintf("%-11.11s %c%s%s",
               capitalize(u[i]->query_name(1)),' ',
                 B_BLUE_F,(j++,j > 3 ? (j=0,"\n") : " |  ")); 
    }
  }
  return s;
}
 
status sort_by_security(object a, object b) {
    return a->query_security() > b->query_security();
}
 
status sort_by_age(object a, object b) {
    return a->query_nothing() < b->query_northing();
}
 
status sort_by_level(object a, object b) {
    return a->query_level() > b->query_level();
}
 
// Some defines for the BIM pager.
#define PG_BUFF           "BFF"         // The edit buffer.
#define PG_Q_FUNC         "QTF"         // The quit-function.
#define PG_LENG           "LNG"         // Page length.

#undef PAGER
#define PAGER "obj/pager"

void page(string data) {
    int size;
    string *d;
    object pager;
    
    d = explode(data,"\n");
        pager = clone_object(PAGER);
        (void)pager->set_quit_func(this_object(), "TW_quit_pager");
        size = to_int((mixed)this_player()->query_env_var("pg_opts"));
        if (size)
         (void)pager->set_options(size);
        if(this_player()->defined_env_var("who_length")) {
            size = to_int((mixed)this_player()->query_env_var("who_length"));
            (void)pager->set_page_length(size);
        } else if(this_player()->defined_env_var("pg_length")) {
            size = to_int((mixed)this_player()->query_env_var("pg_length"));
            (void)pager->set_page_length(size);
        }
        (void)pager->more_text(d, "DragonHeart Who List");
}
 
void TW_quit_pager(string reason, object pager, mixed extra) {
  if (!reason) write("Terminated.\n");
  destruct(pager);
  return;
}


 
status help() {
   printf("\
Command:  who [-imphges]\n\
\n\
The who command lists all the Immortals and Players logged on\n\
DragonHeart.  The output is paged though 'More'.  You may set your page\n\
length with 'set pg_length <numlines>'  A <numlines> of 0 will disable\n\
paging.\n\
\n\
Options:\n\
    i = Immortals   p = Players\n\
    s = Short who list\n\
Legend:\n\
    The following flags will sometimes be found in the right margin:\n\
             I = Idle (more than 5 minutes)\n\
             B = Busy (reply, mail, more, describe, etc)\n\
             E = Editing (inside ed)\n\
             D = Dead (player is ghost)\n\
    If a players race/alignment is in angle < > brackets then that player\n\
    is invis (Higher level players can see lower level invis players)\n\
");
   return 1;
}
 
object this_player() {
    return efun::this_player() ? efun::this_player() : this_object();
}
 
status is_immortal(object u) { return u->query_security(); }
status is_mortal(object u) { return  !is_immortal(u); }
status is_player(object u) { return  !is_immortal(u); }


mapping a_title_ranks = 
([
                            "King"         : 1,
                            "Queen"        : 1,
                            "Prince"       : 2,
                            "Princess"     : 2,
                            "Duke"         : 3,
                            "Duchess"      : 3,
                            "Count"        : 4,
                            "Countess"     : 4,
                            "Baron"        : 8,
                            "Baroness"     : 8,
                            "Lord"         : 15,
                            "Lady"         : 15,
                            "Knight"       : 17,
                            ""             : 50,
                            0              : 50,
                            ]);
                        
int a_rank(object u) {
    return a_title_ranks[(string) u->query_pretitle()];
}

int sort_by_alignment_rank(object a, object b) {
   return a_rank(a) < a_rank(b);
}


