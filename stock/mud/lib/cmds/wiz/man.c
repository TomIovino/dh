#include <mudlib.h>
 
string *man_path = ({ "applied", "LPC", "efun", 
    "master", "concepts", "driver", "socket", "mudlib" });
 
mapping index = ([ ]);
closure find, ifind; 
 
void fill_index(string dir);
status rehash();
status main(string arg);
status help();       

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
        if(this_player()->defined_env_var("mn_length")) {
            size = to_int((mixed)this_player()->query_env_var("mn_length"));
            (void)pager->set_page_length(size);
        } else if(this_player()->defined_env_var("pg_length")) {
            size = to_int((mixed)this_player()->query_env_var("pg_length"));
            (void)pager->set_page_length(size);
        }
        (void)pager->more_text(d, header ? header : "Manual Page");
}
 
void TW_quit_pager(string reason, object pager, mixed extra) {
  if (!reason) write("Terminated.\n");
  destruct(pager);
  return;
}


string basename(string path) {
    return explode(path,"/")[<1];
}
 
varargs
void more(string msg, string section, string file) {
    string header;

    if(section && file) {
        header = sprintf("%s%s",basename(file),section ? sprintf("(%s)",section) : "");
        msg = sprintf("SECTION\n\t%s (%s)\n\n",section,file) + msg;    
    } else {
        header = sprintf("%s (%s)",basename(file),file);
    }
    page(msg,header);
}
 
void reset(int arg) {
    find = lambda( ({ 'doc }),
            ({ #'?,
                ({ #'mapping_contains, ({ #'&, 'file }), ({ #'&, 's }), 
                                                    ({ #'index }), 'doc}),
                ({ #',, ({ #'more, ({ #'read_file, 'file }), 's, 'file }), 1 }), 0 
            }));
    ifind = lambda( ({ 'doc }),
            ({ #'?,
                ({#'mapping_contains,({#'&,'file}),({#'&,'s}),
                     ({#'index}),'doc}),'file, 0
            }));
    rehash();
}
 
void rehash() {
    int i;
    index = ([ ]);
    if(i = sizeof(man_path))
        while(i--) call_out("fill_index",i,man_path[i]);
}
 
void fill_index(string dir) {
    int i;
    string *files, file, doc;
    if(!mappingp(index)) index = ([ ]);
    files = get_dir(sprintf("/doc/%s/*",dir));
    if(!files || !(i = sizeof(files))) return;
    while(i--) {
        doc = files[i];
        if(doc[0] == '.') continue;
                file = sprintf("/doc/%s/%s",dir,doc);
        index += ([ doc : file ; dir ]);
    }
}
 
status match_keyword(string key, string match_str) {
    return key[0..strlen(match_str)-1] == match_str;
}
 
varargs
mixed keyword_list(string key, status flag) {
    string *names;
    int size;
    size = sizeof(names = filter_array(m_indices(index),"match_keyword",
                  this_object(),key));
    if(size == 1 && flag) return funcall(find,names[0]);
    if(!size) names = ({ "<None Found>" });
    names = sort_array(names,#'>);
    more(
    sprintf("%-#79s\n",implode(names,"\n")),0,sprintf("%s Available Topic%s Matching: '%s'",
                     (size ? capitalize(to_string(size)) : "No"),
                     (size == 1 ? "" : "s" ), key));
    
    return 1;
}
    
varargs
string imud_keyword_list(string key) {
    string *names;
    int size;
    size = sizeof(names = filter_array(m_indices(index),"match_keyword",
                  this_object(),key));
    if(size == 1) return read_file(funcall(ifind,names[0]));
    if(!size) names = ({ "<None Found>" });
    names = sort_array(names,#'>);
    return sprintf("%|79s\n%-#79s\n",
                   sprintf("%s Available Topic%s Matching: '%s'",
                   (size ? capitalize(to_string(size)) : "No"),
                   (size == 1 ? "" : "s" ), key),implode(names,"\n"));
}
    
status dumpindex() {
    return keyword_list("");
}
 
string get_man_string(string key) {
    string file, data;
    file = funcall(ifind,key);    
    if(!file)
       return "ManServer@DragonHeart:\n"+imud_keyword_list(key)+"\n";
    else
       return "ManServer@DragonHeart:\n"+read_file(file)+"\n";
}
 
status main(string arg) {
    string *argv;
    int ReHashed;
    if(!arg || !stringp(arg)) return help();
    argv = explode(arg," ");
    switch(argv[0]) {
        case "-l" :
            return dumpindex(); break;
        case "-k" :
            return keyword_list(argv[1]); break;
        case "-r" :
            write("Rehashing Doc Table...");
            rehash();
            write("Done.\n");
            return 1;
            break;
    }
    if(!funcall(find,argv[0])) keyword_list(argv[0],1);        
    return 1;
}
 
status help() {
    printf("Command: man\nSyntax: man <keyword> | -r | -l | -k <key>\n\n");
    printf("-r will Rehash the Doc Listing.\n-l will Dump the doc listing.\n");
    printf("-k <key> will list all Docs matching the prefix <key>\n");
    return 1;
}
