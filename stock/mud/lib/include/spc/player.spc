#ifndef PLAYER_H
#define PLAYER_H


void do_log_me(string sitc);
void add_standard_commands();

status put(string str);
status pick_up(string str);
status drop_thing(string str);
status give_object(string str);
status quit(mixed arg);
status quit_ld(mixed arg);
status quit_2(mixed arg);
static void redo();
static void heart_beat();
static void wiz_commands();
static void wiz_commands2();
static void get_new_player_object();
static void time_out();
static status logon();
static status ansi_on();
status is_player();

static status query_deaths();
static status query_afk();
static void check_password(string p);
static void new_password(string p);
static void logon20(string str);
static void logon30(string str);
static void logon40(string str);
static void logon50(string str);
static void logon53(string str);
static void logon56(string str);
static void logon59(string str);
static void logon60(string str);
static void logon63(string str);
static void logon66(string str);
static void logon69(string str);
static void logon70(string str);
static void logon75(string str);
static void logon71(string str);
static void logon72(string str);
static void logon73(string str);
static void logon74(string str);
static void logon75(string str);
static void logon77(string str);
static void logon78(string str);
static void logon79(string str);
static void logon80(string str);
static void logon81(string str);
static void logon82(string str);
static void logon83(string str);
static void logon84(string str);
static void logon85(string str);
static void logon86(string str);
static void logon87(string str);
static void logon88(string str);
static void logon89(string str);
static void logon90(string str);
static void logon93(string str);
static void logon95(string str);
static void logon99(string str);
static void logon100(string str);
static void destruct_all_inventory(object ob);
static status wimpy(string str);
static status change_password();
static void change_password1(string str);
static void change_password2(string str, string new);
static void pass1(string str);
static status describe();
static status get_desc(string arg);
static status score();
static status hp();
static status change_plan();
static status fix_it(string str);
static status kill(string str);
static status access(string dir, string file, string mode);
static status show_mainlog();
static status security(int sec_level);
static int FILE_SIZE(string file);

static status filter_users(object ob);
static status filter_wizards(object ob);
static status by_name(object a, object b);
static status by_level(object a, object b);
static status by_age(object a, object b);
static status by_environment(object a, object b);
static status by_level(object a, object b);
static string query_ip();
static status by_ip_number(object a, object b);
static status by_sec_level(object a, object b); 
static status command_centre(string str);
status do_cmd(string str);
static status echo(string str);
static status more(string str);


/* sets */

string set_enter_room(string str);
int set_intoxicated(int i);
int set_stuffed(int i);
int set_soaked(int i);
int set_headache(int i);
int set_scar(int i);
string set_quests(string *arr);
string set_guild_string(string s);
string set_email(string s);
string set_description(string s);
string set_plan(string s);
string set_time_to_save(int i);
status set_edit_on(status i);
status set_informed(status i);
status set_start();

/* querys */

string query_last_login();
string query_enter_room();
status query_edit();
status query_prevent_shadow();
string query_email();
string query_title();
string query_desc();
int query_total_exp();
int query_level();
int query_intoxication();
int query_stuffed();
int query_soaked();
object query_other_copy();

/* toggles */

status toggle_brief(status silently);
status toggle_no_wimpy();
status toggle_ghost();


/* adds */

int add_exp(int e);
int add_intoxication(int i);
int add_stuffed(int i);
int add_soaked(int i);


varargs void shout_tell(string str, string lang);
void show_scar();
status save_character();
void save_me(status value_items);
status restore_me();
status quit(mixed arg);
status church();
status inventory();
status examine(string str);
status look(string str, status extra);
status help(string what);
void second_life();
int drink_alcohol(int drink_strength);
status drink_soft(int drink_strength);
status eat_food(int food_strength);
string valid_write(string str);
string valid_read(string str);
status tail_file(string path);
status cat_file(string path);

status secure(int sec_level);
int query_security();
int query_security_level();
int set_security_level(string domain);
string make_path(string file);
status pwd();
status toggle_invis();
int compute_values(object ob);
void illegal_patch(string what);
void load_auto_obj();
void compute_auto_str();

int get_alias_index(string str);
status add_alias(string str);
void remove_alias(string str);
status log_for_players(string file, string str);
status who();
status emote(string str);
status tell(string str);
status whisper(string str);
status shout_to_all(string str);
static void move_or_destruct_inventory(object arg);
status valid_file_name(string arg);
void get_party();
status toggle_hp( string str);

#endif /* PLAYER_H */
