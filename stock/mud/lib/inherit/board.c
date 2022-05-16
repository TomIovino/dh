////////////////////////////////////////////////////////////
//  BOARD.C           // DRAGONHEART // GEEWHIZ.6.15.95   //
////////////////////////////////////////////////////////////

#include <mudlib.h>
#undef DEBUG
#define TITLE             "TLE"                // Title of post
#define AUTHOR            "ATH"                // Author of post
#define BODY              "BDY"                // Body of post
#define WHEN              "DTE"                // Last Edit Date
#define ORIGDATE          "ORD"         // Date of original post
#define LOCK              "LCK"
 
// Some defines for the BIM editor.
#define ED_BUFF           "BFF"         // The edit buffer.
#define ED_MAILR          "MLR"         // The calling mailer (bimr).
#define ED_EXTRA          "XTR"         // Extra values.
#define ED_CURR           "CUR"         // The current line.
#define ED_Q_FUNC         "QTF"         // The quit-function.
 
// Some defines for the BIM pager.
#define PG_BUFF           "BFF"         // The edit buffer.
#define PG_MAILR          "MLR"         // The calling mailer (bimr).
#define PG_EXTRA          "XTR"         // Extra values.
#define PG_CURR           "CUR"         // The current line.
#define PG_Q_FUNC         "QTF"         // The quit-function.
#define PG_LENG           "LNG"         // Page length.
#define PG_SIZE           "SZE"         // Page buffer size.
#define PG_PATT           "PTT"         // Last search buffer.
 
#define SAVE(F)   (sprintf("/ob_saves/boards/%s",F))
#define OLD_SAVE(F)   (sprintf("/ob_saves/%s",F))
#define FAIL(X)   return notify_fail(X),0
 
#define DEBUG(X,M) _debug(X,M);
 
#include <ansi_filter.h>
// need these funcs -Ash
static string name;
string set_name(string s){ name = s; return name; }
string query_name(){ return name; }

void _debug(string x, string m) {
   object o;
   if(o = find_player("geewhiz")) 
       tell_object(o,sprintf(x,m));
}

static mixed alt_admin = 0;
 
static closure o_write = 0,
               o_read = 0;
 
void set_write(closure oper) { o_write = oper; }
void set_read( closure oper) { o_read  = oper; }
status is_writeable() { return o_write ? funcall(o_write) : 1; }
status is_readable()  { return o_read  ? funcall(o_read ) : 1; }
 
 
string ctime(int t) {   // this one drops the seconds
   string c;
   if(!t) t = time();
   c = efun::ctime(t);
   return c[0..15] + c[19..];
}
 
status immortal() { return this_player()->query_security(); }
 
status set_imm_post(status x) {
    if(x) set_write(#'immortal); 
    else  set_write(0); 
    return x;
}
 
status set_imm_read(status x) {
    if(x) set_read(#'immortal); 
    else  set_read(0); 
    return x;
}
 
static int last_note = -1;
int max_notes = 100;
static string file = 0;
static string long_desc  = 0;
static string short_desc = "A Bulletin Board";
mapping *board_data = ({ });
 
void help();
string num_notes();
void set_short(string str);
void set_long(string str);
status set_file(string str);
void save_board();
status restore_board();
void get_note(string str,mapping note,int line);
status list_notes(string arg);
void post_a_note(mapping note);
void remove_a_note(mapping note);
string fix(string s);
 
int get_last_read() {
    mapping read;
    read = (mapping) this_player()->query_attrib("board_info");
    if(!read) read = ([ ]);
    return read[file];
}
 
void set_last_read(int t) {
    mapping read;
    read = (mapping) this_player()->query_attrib("board_info");
    if(!read) read = ([ ]);
    read[file] = t;
    this_player()->set_attrib("board_info",read);
    return;
}
     
status id(string str) { return str == "board" ||
                               str == "bullentin board" ||
                               str == name; }
status get() { 
    if(query_verb()=="get") 
        printf("It is attached securely to the ground.\n");
    return 0;
}
string short() { return is_readable() ?
                        sprintf("%s [%s]",short_desc,num_notes())
                                      :
                        "An Unreadable Bulletin Board"; }
int query_weight(){ return 14; }
string long() { 
   int TPlevel;
   string name;
   name = to_string(this_player()->query_real_name());
   TPlevel = this_player()->query_level();
   if(long_desc && long_desc != "") printf(long_desc);
   if((alt_admin && stringp(alt_admin) && alt_admin == name)
     || (this_player()->query_security() >= 25)
     || (alt_admin && intp(alt_admin) && TPlevel >= alt_admin))
     printf("** You may remove notes belonging to you **\n");
   list_notes(sprintf("%d-%d",(last_note-9),(last_note+1)));
}
 
void reset(int started) {
    if(!started) 
        restore_board();
}
 
void set_short(string str) {
    short_desc = str;
}
 
#define BOARD_ADMIN "geewhiz"
 
 
#define ALLOWED_CMDS ({ "tell","reply","who","finger","look","l", "dest", \
                         "r","alias","unalias","set","unset", })
 
private
status movement_cmd(string v) { return member(v,'/') == -1; }
 
 
static int GAGactive = 0;
 
void tellr(string s) { tell_room(environment(),s); }
 
void inc_gag() {
//     GAGactive++;
     if(GAGactive == 1) tellr("You feel as though you cannot talk here.\n");
}
void dec_gag() {
//     GAGactive--;
     if(GAGactive < 0) GAGactive = 0;
//     if(!GAGactive) tellr("You feel you can talk again.\n");
}
 
status toggle_gag() {
    GAGactive = !GAGactive;
    tell_room(environment(),
          sprintf("Board gag now %s.\n",GAGactive ? "active" : "inactive"));
    return 1;
}
 
status Cmd_Gag(string arg) {
    string *dest_dir;
    mapping channels;
    string verb, *allowed;
    status ret;
    if(!GAGactive) return 0;
    allowed = ALLOWED_CMDS;
    if(environment())
        dest_dir = (string *) environment()->query_dest_dir();
    if(dest_dir && sizeof(dest_dir)) {
        dest_dir = filter_array(dest_dir,#'movement_cmd);
        allowed += dest_dir;
    }
    if(this_player() && channels = (mapping) this_player()->query_channel()) {
        allowed += m_indices(channels);
    }
    ret = member(allowed,query_verb()) == -1;
    if(ret) printf("You cannot %O in board rooms.\n",query_verb());
    return ret;
}
 
void init() {
    restore_board();
    if(GAGactive) printf("You feel as though you cannot talk here.");
    if(this_player()->query_attrib("silenced")) return 0;
    add_action("Cmd_Gag","",1);
    if(is_readable()) {
        add_action("read_note","read");
        add_action("list_notes","list");
        add_action("unread_note","unread");
        add_action("unread_note","renew");
    }
    if(is_writeable()) {
        add_action("remove_note","remove");
        add_action("remove_note","delete");
        add_action("edit_note","edit");
        add_action("post_note","post");
        add_action("post_note","note");
    }
    if(this_player()->query_security()) { 
        if(is_readable()) {
            add_action("cmd_mailnote","mailnote");
        }
        add_action("lock_post","lock");
        add_action("unlock_post","unlock");
    }
 // Board Admin Commands //////////////////////////////////////
    if(this_player()->query_real_name() == BOARD_ADMIN) {
        printf("#");
        add_action("adj_note_author","adjauthor");
        add_action("adj_note_subject","adjsubject");
        add_action("adj_board_file","adjfile");
        add_action("toggle_gag","tgag");
    }
 //////////////////////////////////////////////////////////////
}
 
status unread_note(string arg) {
    string name,old;
    int num;
    restore_board();
    if(!arg || arg == "" || (sscanf(arg,"%d",num) != 1))
        FAIL(sprintf("Syntax:  %s <note#>\n",query_verb()));
    num--;
    if(num<0 || num>last_note)
            FAIL(sprintf("Invalid Note number %d.\n",(num+1)));
    name = to_string(this_player()->query_real_name());
    set_last_read(board_data[num][WHEN] - 1);
    printf("All notes after and including note number %d are now unread.\n",(num+1));
    return 1;
}

status lock_post(string arg) {
    string name,old;
    int num;
    restore_board();
    if(!arg || arg == "" || (sscanf(arg,"%d",num) != 1))
        FAIL(sprintf("Syntax:  %s <note#>\n",query_verb()));
    num--;
    if(num<0 || num>last_note)
            FAIL(sprintf("Invalid Note number %d.\n",(num+1)));
    name = to_string(this_player()->query_real_name());
    if((old = board_data[num][LOCK]) && old != name && name != BOARD_ADMIN)
            FAIL(sprintf("Note %d already locked by %O.\n",num+1,old));
    board_data[num][LOCK] = name;
    say(sprintf("%s locks the note entitled '%s'.\n",
                (string)this_player()->query_name(),
                (string)board_data[num][TITLE]));
    printf("Note number %d now locked by you.\n",(num+1),old,name);
    if(old) printf("\t(The note was previously locked by %O)\n",old);
    save_board();
    return 1;
}
 
status unlock_post(string arg) {
    string name,old;
    int num;
    restore_board();
    if(!arg || arg == "" || (sscanf(arg,"%d",num) != 1))
        FAIL(sprintf("Syntax:  %s <note#>\n",query_verb()));
    num--;
    if(num<0 || num>last_note)
            FAIL(sprintf("Invalid Note number %d.\n",(num+1)));
    name = to_string(this_player()->query_real_name());
    if(!old = board_data[num][LOCK])
            FAIL(sprintf("Note %d is not locked.\n",num+1,old));
    if(this_player()->query_real_name() != "ash")
    if(old != name && name != BOARD_ADMIN)
            FAIL(sprintf("Note %d can only be unlocked by %O.\n",num+1,old));
    board_data[num][LOCK] = 0;
    say(sprintf("%s unlocks the note entitled '%s'.\n",
                (string)this_player()->query_name(),
                (string)board_data[num][TITLE]));
    printf("Note number %d now unlocked.\n",(num+1),old,name);
    if(old != name) 
       printf("\t(The note was previously locked by %O)\n",old);
    save_board();
    return 1;
}
 
status adj_note_author(string arg) {
    string name, old;
    int num;
    if(this_player()->query_real_name() != BOARD_ADMIN) 
        FAIL("who are you?\n");
    restore_board();
    if(!arg || arg == "" || (sscanf(arg,"%d %s",num,name) != 2))
        FAIL(sprintf("Syntax:  %s <note#> <author>\n",query_verb()));
    num--;
    if(num<0 || num>last_note)
            FAIL(sprintf("Invalid Note number %d.\n",(num+1)));
    name = capitalize(name);
    old = board_data[num][AUTHOR];
    board_data[num][AUTHOR] = name;
    say(sprintf("%s reauthors the note entitled '%s' from %s to %s.\n",
                (string)this_player()->query_name(),
                (string)board_data[num][TITLE], old, name));
    printf("Note number %d changed from %s to %s.\n",(num+1),old,name);
    save_board();
    return 1;
}
 
status adj_note_subject(string arg) {
    string sub, old;
    int num;
    if(this_player()->query_real_name() != BOARD_ADMIN) 
        FAIL("who are you?\n");
    restore_board();
    if(!arg || arg == "" || (sscanf(arg,"%d %s",num,sub) != 2))
        FAIL(sprintf("Syntax:  %s <note#> <subject>\n",query_verb()));
    num--;
    if(num<0 || num>last_note)
            FAIL(sprintf("Invalid Note number %d.\n",(num+1)));
    old = board_data[num][TITLE];
    board_data[num][TITLE] = sub;
    say(sprintf("%s retitles the note entitled '%s' to '%s'.\n",
                (string)this_player()->query_name(),
                old, (string)board_data[num][TITLE]));
    printf("Note number %d changed from '%s' to '%s'.\n",(num+1),old,sub);
    save_board();
    return 1;
}
 
status adj_board_file(string arg) {
    string sub, old;
    int num;
    if(this_player()->query_real_name() != BOARD_ADMIN) 
        FAIL("who are you?\n");
    restore_board();
    save_board();
    if(!arg || arg == "")
        FAIL(sprintf("Syntax:  %s <savefile>\n",query_verb()));
    old = file;
    if(!set_file(arg)) {
        set_file(old);
        FAIL(sprintf("board:  Could not restore %O.  Board restored.\n",arg));
    }
    say(sprintf("%s adjusts the board.\n",
            (string)this_player()->query_name()));
    printf("Board save file changed from %O to %O.\n",old,arg);
    save_board();
    return 1;
}
 

string get_subject(int num) {
    restore_board();
    return (string)board_data[num][TITLE];
}

string get_header(int num) {
    restore_board();
    if(!board_data[num][ORIGDATE]) 
        board_data[num][ORIGDATE] = board_data[num][WHEN];
    return sprintf("%c%c%3d : %-=30s  %|=15s  %s\n",
                      board_data[num][LOCK] ? '*' : ' ',
                      get_last_read() < board_data[num][WHEN] ? 'N' : ' ',
                      (num+1),
                      (string)board_data[num][TITLE],
                      (string)board_data[num][AUTHOR],
                      ctime((int)board_data[num][WHEN]));
}
 
string get_short_header(int num) {
    restore_board();
    if(!board_data[num][ORIGDATE]) 
        board_data[num][ORIGDATE] = board_data[num][WHEN];
    return sprintf("%c%c%3d: %s  %s  %s",
                      board_data[num][LOCK] ? '*' : ' ',
                      get_last_read() < board_data[num][WHEN] ? 'N' : ' ',
                      (num+1),
                      (string)board_data[num][TITLE],
                      (string)board_data[num][AUTHOR],
                      ctime((int)board_data[num][WHEN]));
}
 
string get_body(int num) {
    return fix(board_data[num][BODY]);
}
 
status remove_note(string arg) {
    int num, TPlevel;
    string name;
    restore_board();
    if(!is_writeable()) return 0;
    if(!arg || arg == "")
        FAIL(sprintf("Syntax:  %s <note#>\n",query_verb()));
    if(sscanf(arg,"%d",num) != 1)
        FAIL(sprintf("Syntax:  %s <note#>\n",query_verb()));
    num--;
    if(num<0 || num>last_note)
            FAIL(sprintf("Invalid Note number %d.\n",(num+1)));
    name = to_string(this_player()->query_real_name());
    TPlevel = this_player()->query_level();
    if(board_data[num][LOCK]) 
            FAIL(sprintf("Note number %d is locked by %O.\n",
                          num+1,capitalize(board_data[num][LOCK])));
    if( (capitalize(name) == board_data[num][AUTHOR]) 
    || (this_player()->query_security() >= 50)
    || (alt_admin && stringp(alt_admin) && alt_admin == name)
    || (alt_admin && intp(alt_admin) && TPlevel >= alt_admin)) {
        say(sprintf("%s removes a note entitled '%s'.\n",
                (string)capitalize(name),
                (string)board_data[num][TITLE]));
        remove_a_note(board_data[num]);
        printf("Note number %d removed.\n",(num+1));
        return 1;
    }
    FAIL("You may only remove your own notes.\n");
}

#undef PAGER
#define PAGER "obj/pager"

void page(string data,string header) {
    int size;
    string *d;
    object pager;
    
    d = explode(data,"\n");
        pager = clone_object(PAGER);
        (void)pager->set_quit_func(this_object(), "TW_quit_pager");
        size = to_int((mixed)this_player()->query_env_var("pg_opts"));
        if (size)
         (void)pager->set_options(size);
        if(this_player()->defined_env_var("bd_length")) {
            size = to_int((mixed)this_player()->query_env_var("bd_length"));
            (void)pager->set_page_length(size);
        } else if(this_player()->defined_env_var("pg_length")) {
            size = to_int((mixed)this_player()->query_env_var("pg_length"));
            (void)pager->set_page_length(size);
        }
        (void)pager->more_text(d, header ? header : "Post");
}
 
void TW_quit_pager(string reason, object pager, mixed extra) {
  if (!reason) write("Terminated.\n");
  destruct(pager);
  return;
}

 
void show_note(int num) {
    mapping note;
    restore_board();
    note = board_data[num];
    if(board_data[num][WHEN] > get_last_read())
        set_last_read(board_data[num][WHEN]);
    page(get_body(num),get_short_header(num));
}
 
string *get_message_text(int num) {
    restore_board();
    if(num < 0 || num > last_note) {
        return ({ });
    } else 
        return explode(get_body(num),"\n");
}
 
status read_note(string arg) {
    int num, i, last;
    mapping note;
    string name;
    restore_board();
    if(!is_readable()) return 0;
    if(!arg || arg == "")
        FAIL(sprintf("Syntax:  %s <note#> | new\n",query_verb()));
    if(sscanf(arg,"%d",num) == 1) {
        num--;
        if(num<0 || num>last_note)
            FAIL(sprintf("Invalid Note number %d.\n",(num+1)));
        return show_note(num),1;
    }
    if(arg == "new") {
        num = -1;
        i = 0;
        last = get_last_read();
        while(num == -1 && i <= last_note) {
            if(board_data[i][WHEN] > last) {
                num = i;
            }
            i++;
        }
        if(num != -1) return show_note(num),1;
        FAIL("No new notes.\n");
   }
    FAIL(sprintf("Syntax:  %s <note#> | new\n",query_verb()));
}
 
status edit_note(string arg) {
    int num,i;
    mapping note;
    string name;
 
    restore_board();
    if(!is_writeable()) return 0;
    if(!arg || arg == "")
        FAIL(sprintf("Syntax:  %s <note#>\n",query_verb()));
    if(sscanf(arg,"%d",num) == 1) {
        num--;
        if(num<0 || num>last_note)
            FAIL(sprintf("Invalid Note number %d.\n",(num+1)));
    if((capitalize((string)this_player()->query_real_name()) ==
          board_data[num][AUTHOR]) || (this_player()->query_security()>=90)) {
            note =  board_data[num];
            say(sprintf("%s edits a note entitled '%s'.\n",
                        (string)note[AUTHOR],(string) note[TITLE]));
 
inc_gag();
 
           (void)"/secure/bim/editor"->start_editing(
                  ([
                     ED_MAILR   : this_object(),
                     ED_BUFF    : get_message_text(num),
                     ED_Q_FUNC  : "quit_edit",  
                     ED_EXTRA   : note,
                     ED_CURR    : sizeof(get_message_text(num))
                   ]));
            remove_a_note(board_data[num]);
            return 1;
        }
    }
    FAIL("You can only edit your own notes.\n");
}
 
status list_notes(string arg) {
    int start,end, i;
    string name;
    restore_board();
    if(!is_readable()) return 0;
    if(last_note == -1) return printf("\n%s\n","The Board Is Empty."),1;
 
    if(!arg) {
        start = last_note - 10; 
        end = last_note;
    } else if(member_array('-',arg) != -1) {
        sscanf(arg,"%d-%d",start,end);
        start--;
        end--;
    } else if(sscanf(arg,"%d",start) == 1) {
        start--;
        if(start > last_note
        || start < 0)
            FAIL(sprintf("Invalid note number %d.\n",start+1));
        printf("Detailed Information for Post #%d:\n",start+1);
        printf("\tAuthor: %s\n",board_data[start][AUTHOR]);
        printf("\tTitle : %-=30s\n",board_data[start][TITLE]);
        printf("\tOriginally Posted: %s\n",ctime(board_data[start][ORIGDATE]));
        printf("\tLast Edited: %s\n",ctime(board_data[start][WHEN]));
        printf("\tBody Content Length:  %d (%d lines)\n",
                   strlen(board_data[start][BODY]), 
                   sizeof(explode(board_data[start][BODY],"\n")));
        if(get_last_read() < board_data[start][WHEN])
            printf("\tYou have not yet read this note.\n");
        if(board_data[start][LOCK])
        printf("\tThis post has been locked by %s\n",
                     capitalize(board_data[start][LOCK]));
        return 1;
    } else 
        FAIL(sprintf("%s: Syntax: %s [ <num> | <num>-<num> ]\n",query_verb(),
                                     query_verb()));
    if(start > last_note) start = last_note;
    if(start < 0) start = 0;
    if(end > last_note) end = last_note;
    if(end < 0 || end < start) end = last_note;
    name = (string) this_player()->query_real_name();
    printf("  %3s :%|30s  %|15s  %s\n",
    "Num","Title","Author","Post/Edit Date");
    for(i=start;i<=end;i++)
        printf(get_header(i));
    return 1;
}
 
int last_post_time;
 
int get_time() { 
    if(time() <= last_post_time)
        last_post_time++;
    else
        last_post_time = time();
    return last_post_time;
}
 
void post_a_note(mapping note) {
    restore_board();
    note[WHEN] = get_time();
    board_data += ({ note });
    last_note = sizeof(board_data) - 1;
    save_board();
}            
 
void remove_a_note(mapping note) {
    // restore_board()  // can't do this here... mapping wouldn't be same
    board_data -= ({ note });
    last_note = sizeof(board_data) - 1;
    save_board();
}            
 
status post_note(string arg) {
    mapping note;
    string *text;
    restore_board();
    if(!is_writeable()) return 0;
    if(last_note >= max_notes) 
        FAIL("The Bulletin Board Is Full.\n");
    if(!arg || arg == "")
        FAIL(sprintf("Syntax:  %s <title>\n",query_verb()));
    if(strlen(arg) > 60)
        FAIL("Title must be shorter than 60 characters.\n");
    if(this_player()->query_level() < 25)
        FAIL("Sorry, You must be level 25 to post.\n");
 
    note =  ([
                 TITLE: arg,
                AUTHOR: capitalize((string)this_player()->query_real_name()),
            ]);
    say(sprintf("%s starts a note entitled '%s'.\n",
                (string)note[AUTHOR],(string) note[TITLE]));
 
inc_gag();
 
    (void)"/secure/bim/editor"->start_editing(([
                                                ED_MAILR   : this_object(),
                                                ED_BUFF    : text,
                                                ED_Q_FUNC  : "quit_write",
                                                ED_EXTRA   : note
                                              ]));
    return 1;
}
 
void quit_write(mapping data) {
  mapping note;
  string *text;
  text = data[ED_BUFF];
  note = data[ED_EXTRA];
 
dec_gag();
 
  if (!text) {
    write("Aborted.\n");
    return;
    }
  note[BODY] = implode(text, "\n") + "\n";
  note[ORIGDATE] = time();
  post_a_note(note);
  printf("Done.\n");
  say(sprintf("%s finishes a note entitled '%s'.\n",
              (string)note[AUTHOR],(string) note[TITLE]));
  save_board();
  return;
}
 
void quit_edit(mapping data) {
  mapping note;
  string *text;
  text = data[ED_BUFF];
 
dec_gag();
 
  if (!text) {
    write("Edit Aborted. (Reposting your post...)\n");
    post_a_note(data[ED_EXTRA]);
    return;
    }
  note = data[ED_EXTRA];
  note[BODY] = implode(text, "\n");
  note[WHEN] = time();
  post_a_note(note);
  printf("Done.\n");
  say(sprintf("%s completes editing a note entitled '%s'.\n",
              (string)note[AUTHOR],(string) note[TITLE]));
  return;
}
 
string num_notes() {
    string ret;
    int i,new, last;
    restore_board();
    if(last_note == -1) return "No Notes";
    ret = sprintf("%d Note%c",(last_note+1),(last_note==0 ? 0 : 's'));
    new = 0;
    last = get_last_read();
    for(i=(last_note+1); i--; ) 
        if(board_data[i][WHEN] > last) new++;
    if(new) ret += sprintf(", %d Unread",new);
    return ret;
}      
 
void set_long(string str) {
    long_desc = str;
}
 
status set_file(string str) {
    file = str;
    return restore_board();
}    
 
void set_max_notes(int num) {
    max_notes = num;
}
 
status restore_board() {
    int ret;
    if(!file) return 0;
    ret = restore_object(SAVE(file));
    if(!ret) {
       if(ret = restore_object(OLD_SAVE(file))) {
           save_board();
           rm(OLD_SAVE(file)+".o");
       }
    }
    last_note = sizeof(board_data) - 1;
    return ret;
}
 
void save_board() {
    if(!file) return;
    save_object(SAVE(file));
}
 
varargs string format(string str, int indent, int shift, int width) {
    int extra,pre;
    if(!str || str == "" || !stringp(str))
        raise_error("format(): Bad arg 1 (need string)\n");
    if(!indent || indent < 0)
        indent = 0;
    if(!width || width < 0)
        width = 75;
    if(shift < 0) {
        pre = -1 * shift;
        extra = 0;
    } else if(shift == 0) {
        pre = 0;
        extra = 0;
    } else { // shift > 0
        pre = 0;
        extra = shift;
    }
    if(extra) str = sprintf("%*s",extra,"") + str;
    return sprintf("%*s%-*=s\n",indent,str[0..pre-1],width-indent,str[pre..]);
}
 
varargs string wrap(string str, int width, int indent) {
    if(!width) width = 78;
    if(!indent) indent = 0;
    return format(str,indent,(-1*indent),width);
}
 
#define MARKER (sprintf("%c",255))
 
string fix(string s) {
    string *p;
    int i;
    if(!s) return s;
    s = filter_ansi(s);
    p = explode(s,"\n");
    i = sizeof(p);
    while(i--) 
       if(strlen(p[i]) > 78)
          p[i] = wrap(p[i],78)[0..<1];
    return implode(p,"\n");
//  return subst(wrap(subst(subst(s,"\n\n",MARKER),"\n"," ")),MARKER,"\n\n");
}
 
void notify_destruct() { save_board(); }
 
void set_board_admin(mixed who) { alt_admin = who; }

status cmd_mailnote(string arg) {
    int num, i, last;
    mapping note;
    string name, to, ccs, *cc;

    restore_board();

    if(!is_readable()) return 0;
    
    
    if(!arg || arg == ""
    || sscanf(arg,"%d %s",num, to) < 2)
        FAIL(sprintf("Syntax:  %s <note#> <to> [ <cc1> <cc2> ... <ccN> ]\n",query_verb()));
        
    num--;
    if(num<0 || num>last_note)
        FAIL(sprintf("Invalid Note number %d.\n",(num+1)));

    sscanf(to,"%s %s",to,ccs);
    if(ccs && ccs != "")
        cc = explode(ccs," ");
    else 
        cc = ({ });
    
    if(!name = (string) this_player()->query_real_name())
        FAIL("You have no name.\n");
    "/secure/bim/obj2mail"->send_mail(name, to, cc, "Board->Mail: " + get_subject(num), get_header(num)+implode(get_message_text(num),"\n"));
    printf("Mail queued.\n");
    return 1;    
}

