// Tell command. Bubbs. 12/Aug/93. My Dads birthday.
// 12/Sept/93. Added intermud support. Bubbs.
// 95-98 updated security
// jun98 added soul support -Ash
// jul98 improved soul support -Geewhiz

#define SOULD "/secure/soul/emote_d"
 
#define adminp(WHO)                ((WHO)&&((WHO)->query_security())>=90)
#define wizardp(WHO)               ((WHO)&&((WHO)->query_security())>=10)
 
#define COST              0
#define I_COST            0
 
#define IDLE_TIME         300     // 5 minutes.
#define MAX_TELL_HISTORY  20
 
#include <udp.h>
#include <ansi.h>
// #include <defns.h>
 
// Tell history mapping.
mapping tell_history = ([ ]);
 
status disallow_tell(object dest, object sender);
string do_tell(object user, string from, string message, status is_reply);
 
string *query_cmd_aliases() { return ({ "t" }); }
status notify_fail(string s) {
    return efun::notify_fail(s),0;
}
 
status user_exists(string name) {
    return call_other("/secure/bim/user_exists","user_exists",name);
}
 
status main(string str) {
  object user;
  string name, mudname, mesg;
  int tlevel, mlevel;
  status free;
   if(this_player()->query_attrib("silenced")) return 1;
  if (!str) {
    string *th;
    user = this_player();
    name = (string)user->query_real_name();
    name = (name ? capitalize(name) : (string)user->query_name());
    th = tell_history[lower_case(name)];
    mlevel = sizeof(th);
  if (!mlevel)
     printf("%s: No messages in tell history.\n",
      ctime(time())[11..15]);
    else
     printf("%s: %d message%c in tell history.\n%s\n",
      ctime(time())[11..15],
      mlevel,
      mlevel == 1 ? 0 : 's',
      implode(th, "\n"));
    return 1;
    }
  if (sscanf(str, "%s %s", name, mesg) != 2)
   return
    notify_fail("Usage: tell [<user>[@<mud>] <message>]\n");
  name = lower_case(name);
  mlevel = (int)this_player()->query_security_level();
   
  if (sscanf(name, "%s@%s", name, mudname) == 2) {
    string error;
    if(lower_case(mudname) == "crimsonesti") {
     printf("Helllllllo!\nYou *are* on DragonHeart silly.\nWhy don't you try 'tell %s' instead.\n",name);
     return 1;
    }
    error = (string)INETD->send_udp(mudname, ([
     REQUEST  : "tell",
     SENDER   : (string)this_player()->query_real_name(),
     RECIPIENT: name,
     DATA     : mesg,
     ]), 1);
    write(error ? error : "Message transmitted.\n");
#if I_COST != 0
    if (!wizardp(this_player()) && !error)
     (void)this_player()->restore_spell_points(-I_COST);
#endif
    return 1;
    }
  user = find_player(name);
  if (!user) user = find_player(name);
  if (!user) {
    notify_fail(
     sprintf(
  user_exists(name) ? "User %O is not available.\n" :
 
                          "The user named %O has never played here.\n",
      capitalize(name)));
    return 0;
    }
  tlevel = (int)user->query_security_level();
  if (wizardp(user) && mlevel < tlevel &&
    (string)user->short() == 0 &&
   (status)user->query_env_var("invis_tells") == 0)
    return
     notify_fail(sprintf("User %O is not available.\n",
      capitalize(name)));
  if (disallow_tell(user, this_player()))
    return notify_fail(sprintf("User %O is not available.\n",
      capitalize(name)));
#if COST != 0
  if (wizardp(user) || wizardp(this_player()) ||
   (status)user->query_ghost())
    free = 1;
  if (!free && (int)this_player()->query_spell_points() < COST)
   return notify_fail("Too low on power.\n");
  if (!free)
   (void)this_player()->restore_spell_points(-COST);
#endif
  write(do_tell(user,
   capitalize((string)this_player()->query_real_name()),
   mesg, 0));
  return 1;
  }
 
  int help(){
  write(
   "Usage: tell [<user>[@<mud>] <message>]\n"
   "usage: tell <name> <;emote> <arg>\n"
   "Tell a user a message.  For players, this costs a small\n"
   "amount of spell points. Tells \n"
   "through the intermud can take  some time,and may be repeated.\n"
   "Intermud tells can be blocked, with the environmental \n"
   "variable \"itell_block\".  With no argument, \"tell\" will\n"
   " display up to the last " + MAX_TELL_HISTORY + " tells you have received.\n"
   "\n"
   "See also: \"reply\", \"muds\".\n");
  return 1;
  }
 
status disallow_tell(object dest, object sender) {
    mixed val;
    string *ignore, tmp, mud;
    int index, size, lo, hi, wlev;
    status disallow;
 
    if((int)dest->query_level() == 0)
       return 1;
 
// Can't block tells from Admin 
    if(adminp(sender))
        return 0;
 
// Players can't block tells from wizards
    if(wizardp(sender) && !wizardp(dest))
        return 0;
 
    val = dest->query_env_var("tell_block");
    if(dest->query_spouse() == this_player()->query_real_name())
           val = 0;

 
    if(intp(val)) {
        if(val == 0) {
            return 0;
        } else {
            if(wizardp(dest)) {
                    return 0;
                } else {
                    return 1;
                }
        }
    } else {
       tmp = to_string(val);
    }
 
    ignore = explode(tmp, " ");
    size = sizeof(ignore);
    tmp = (string)sender->query_real_name();
    mud = (string)sender->query_mud_name();
    wlev = (int)sender->query_security();
 
    for(index = 0; (index < size) && !disallow; index++) {
        switch(ignore[index]) {
            case "any": case "all":
                if(wizardp(dest))
                    disallow = 0;
                else 
                    disallow = 1;
                    break;
            case "imud": case "intermud":
                disallow = sender->is_intermud_tell();
                break;
            case "mortals": case "mor": case "players":
                disallow = !wizardp(sender);
                break;
            case "wizards": case "wiz":
                disallow = wizardp(sender);
                break;
            default:
                switch(sscanf(ignore[index], "%d-%d", lo, hi)) {
                    case 2: disallow = (wlev >= lo && wlev <= hi); break;
                    case 1: disallow = (wlev == lo);               break;
                    case 0: disallow = (tmp == ignore[index]) 
                                    || (mud == ignore[index]);     break;
                }
                break;
       }
    }
    return disallow;
  }
 
string do_tell(object user, string from, string message, status is_reply) {
  string name, mesg, outp, *f_info, tp_mesg;
  mixed o;
  outp = "";
 
  if (message[0] == ';') {
    if(catch(f_info = (string *)SOULD->get_feeling(message[1..],"tell",1,user)) || !f_info) 
      // do nothing
      f_info = 0;
    else {
      message = f_info[1];
      tp_mesg = f_info[0];
      if(message[<1] == '\n')
        message = message[0..<2];
      if(tp_mesg[<1] == '\n')
        tp_mesg = tp_mesg[0..<2];
    }
  }

  if(!wizardp(this_player()) && member(message,'\a') > -1) {
      message = subst(message,"\a","*BEL*");
      log_file("TELL_BEEPS",sprintf("[%s] From:%-12s To:%O Message:%O this_player()==%O\n",ctime(),from,user,message,this_player())); 
      outp += "\nWarning.  BEL characters removed."; 
  }
  name = (string)user->query_real_name();
  name = (name ? capitalize(name) : (string)user->query_name());
  if (query_once_interactive(user)) {
    if (interactive(user) == 0)
     outp = sprintf("\nWarning. %s is net-dead.", name);
    if (query_editing(user))
     outp += sprintf("\nWarning. %s is editing.", name);
    if ((object)user->query_silenced())
     outp += sprintf("\nWarning. %s will not hear you while editing.", name);
    if (query_input_pending(user))
     outp += sprintf("\nWarning. %s is busy.", name);
if (interactive(user) && query_idle(user) > IDLE_TIME)
     outp += sprintf("\nWarning. %s is idle.", name);
    if ((status)user->query_ghost() == 1)
     outp += sprintf("\nWarning. %s is dead.", name);
    }
  if (this_player())
   (void)user->set_tmp_attrib("reply_to", this_player());
  else
   (void)user->set_tmp_attrib("reply_to", from);
  if(!f_info) {
  mesg = sprintf("%s %s : %-=*s", //ash
   from,
   is_reply ? "replies" : "tells you",
   67 - strlen(from),
   message);
  } else {
    mesg = sprintf("<%s> %-=*s",
   is_reply ? "R" : "T",
//   from,
   67 - strlen(from),
   message);
  }
  tell_object(user,
   sprintf("%s%s%s\n",
   ((string)this_player()->do_cc("") ?
   (string)this_player()->do_cc("") : ""),
    mesg,
   ((string)this_player()->do_cc("") ?
     NORM : "")));
  if (member(tell_history, lower_case(name)) == 0)
   tell_history[lower_case(name)] = ({ });
  if (sizeof(tell_history[lower_case(name)]) >= MAX_TELL_HISTORY)
   tell_history[lower_case(name)] = tell_history[lower_case(name)][1..];
  tell_history[lower_case(name)] +=
   ({
     sprintf("%s: %s",
      ctime(time())[11..15],
      mesg)
   });
  if (this_player() == 0)
   name = sprintf("%s@"LOCAL_NAME, name);

  if(!f_info) {
    outp = sprintf("%s%s\nYou %s %s: %-=*s\n",
                   this_player()->do_cc(),
                   outp,
                   is_reply ? "reply to" : "tell",
                   name,
                   65 - strlen(name),
                   message);
  } else {
    outp = sprintf("%s%s\n<%s:%s> %-=*s\n",
                   this_player()->do_cc(),
                   outp,
                   is_reply ? "R" : "T",
                   name,
                   65 - strlen(name),
                   tp_mesg);
  }
  return outp;  
  }
 
string *history(mixed n) {
  string name;
object who;
  if(!n) raise_error("tell::history():  Bad arg 1\n");
  if(stringp(n)) name = n;
  if(objectp(n)) name = to_string( n->query_real_name() );
  if((int)this_interactive()->query_security_level() < 100) {
    if(who = find_player(name)) {
        tell_object(who,sprintf("** %s just peeked at your tell history!",
                    capitalize((string)this_player()->query_real_name())));
    }
  }
  return tell_history[lower_case(name)];
}
