#include <find_ob.h>

#define READ_CHUNK 200
#define PAGER "obj/pager"

main(string str) {
  object pager;
  string txt, *text;
  int ind, sze;
  if (this_interactive() != this_player()) {
    printf("Illegal (forced) attempt to 'more %s' by %s.\n",
     str ? str: "",
     capitalize((string)this_interactive()->query_real_name()));
    return 1;
    }
  if (!str)
   return notify_fail("Usage: more [<file> | -o<object>]\n");
  else if (str[0..1] == "-o") {
    object ob;
    ob = find_ob(str = trimstr(str[2..]));
    if (!ob)
     return
      notify_fail(sprintf("more: Object not found %O.\n", str));
    str = sprintf("/%s.c", base_file(ob));
    }
  else
   str = this_player()->make_path(str);
  if (!((string)this_player()->valid_read(str)))
   return
    notify_fail("Cannot read file.\n");
  sze = file_size(str);
  if (sze < 0)
   return
    notify_fail(sze == -2 ?
     "more: Cannot read a directory.\n" :
     "more: File unreadable or non-existant.\n");
  if (query_editing(this_player()))
   return
    notify_fail("more: You are editing.\n");
  if ((status)this_player()->query_attrib("in_pager") ||
   query_input_pending(this_player()))
    return
     notify_fail("more: You are too busy.\n");
 //txt = read_file(str);
 //text = explode(txt, "\n")[0..<2];
  text = ({ });
  while(txt = read_file(str, 1 + READ_CHUNK * (ind++), READ_CHUNK)) {
    text += explode(txt, "\n");
    if (text[<1] == "") text = text[0..<2];
    }
  if (!sizeof(text))
   return
    notify_fail("more: Empty file.\n");
  pager = clone_object(PAGER);
  (void) this_player()->set_tmp_attrib("in_pager",pager);
  (void)pager->set_quit_func(this_object(), "quit_more");
  sze = to_int((string)this_player()->query_env_var("more_opts"));
  if (sze)
   (void)pager->set_options(sze);
        if(this_player()->defined_env_var("more_length")) {
            sze = to_int((mixed)this_player()->query_env_var("more_length"));
            (void)pager->set_page_length(sze);
        } else if(this_player()->defined_env_var("pg_length")) {
            sze = to_int((mixed)this_player()->query_env_var("pg_length"));
            (void)pager->set_page_length(sze);
        }
  (void)pager->more_text(text, str);
  return 1;
  }

void quit_more(string reason, object pager, mixed extra) {
  write(reason == "END" ? "<End of File>\n" : "Ok.\n");
  destruct(pager);
}

void help() {
  write("Usage: more <file> | -o<object>\n"
        "Print a file to screen, page by page.\n"
        "If the \"-o<object>\" option is used, then that\n"
        "objects filename is looked at.  A number of options\n"
        "are available from within \"more\", type \"?\" for a help page.\n"
        "The environmental variables \"more_opts\", and \"pg_length\"\n"
        "are used, to set the options automagically.\n");
  return;
}
