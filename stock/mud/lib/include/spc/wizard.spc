status edit(string file);
static status promote(string str);
static status load(string str);
static status update(string str, status no_reload);
static status Update(string str);
static status cd(string str);
static status makedir(string str);
static status removedir(string str);
static static dest(string str);
static status snoop_on(string str);
static status local_commands();
static status force_player(string str);
static status zap(string str);
static status heal(string str);
static status stat(string str);
static status shut_down_game(string str);

static status list_peoples(string arg);
static status clone(string file);
static status echoall(string str);
static status echoto(string str);
static status wiz(string str);
static status home();
static status wizard_wizlist(string arg);

static status Delete(string arg);
static status delete(string arg, status arg2);
static void delete_files_prompt(string arg);
static status dir(string str);
static void print_files(string arg);
static void print_verbose(string arg);

static status plural_filter(object ob, string object_id);
static status present_filter(object ob, object who);
static status unique_filter(object ob, mixed *ob_list);
static status dot_filter(string file);
static status pattern_filter(string str, string pattern);
static status directory_filter(string file, string path);
static status file_filter(string file, string path);

string set_edtitle(string s);
string query_path();
string query_msgtrans();
string query_msghome();
string query_mclone();
string query_mdest();

status toggle_edit(status silently);
string parse_msg(string msg, object ob);

string string_array(mixed *arr);
string plural_to_single(string plural);
static varargs object *FIND_OBJECT(string str, object who);
static object *PROCESS_ARG(string arg);
object *PROCESS_ARG_OB(mixed *ob_list, object *ob);
void copy_file(string in, string out, status overwrite);
static void add_date(string file);
static string read_date(string file);
string parse_msg(string msg, object ob);
static status edit(string file);
static status load(string str);
static status clone(string file);
static status show_main_log();
static status show_log(string who);
static status show_debug_log();  
static status makedir(string str);
static status removedir(string str);
static status snoop_on(string str);
static status force_player(string str);
static status zap(string str);
static status heal(string str);
static status stat(string str);
static status shut_down_game(string str);
static status Delete(string str);
varargs static status delete(string str, status prompt_off);
static void delete_files_prompt(string answer);
static void delete_all_files(string arg);
static status dir(string str);
static void print_files(string answer);
static void print_verbose(string answer);
static status duplicate(string str);
static status clean(string str);
static status bomb();
static status dest_ob(string arg);
static status goin(string str);
static status goto(string str);
static status Put(string str);
static status move(string str);
static status patch(string str);
varargs static status Reset_ob(string arg, status reset_arg);
static status reset_ob(string arg);
static status set_lighting(string str);
static status grab(string str);
static status Cat_file(string str);
varargs static status copy(string str, status overwrite, status mv);
static status copy_with_overwrite(string str);
static status deltree(string path);
static void answer_deltree(string ans);
static void delete_dir(string path);
static status find_file(string str);
static string search_pattern(string pattern, string path, status flg);
static status path_tree(string path);
static status Path_tree(string path);
static void answer_tree_prompt(string ans);
static void tree_prompt(string path, int *path_status);
static void skip();
static string *file_tree(string path, int *path_status);

string wildcard_format(string file);
int format_number(string format);
string match_format(string file,string format_in,string format_out);
void do_copy(string arg);
status domain(string arg);

/* wizard message sets */

status wizard_msghome(string str);

status wizard_invis_name(string str);
status wizard_msgtran(string str);
status wizard_mdest(string str);
status wizard_mclone(string str);
status wizard_msgout(string str);
status wizard_msgin(string str);
status wizard_mmsgout(string str);
status wizard_mmsgin(string str);
status wizard_edtitle(string str);
status wizard_title(string str);
status wizard_align(string str);
status wizard_pretitle(string str);
status wizard_review();
status filter_zeros(string str);
/* tamsyn */
status lpc(string str);
varargs void show_results( mixed val, string label, int no_label,int no_newline);