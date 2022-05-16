#include <ansi.h>
#include <mudlib.h>
#include <levels.h>
#include <bar.h>


int more(string what) {
    "/cmds/wiz/more"->main(what);
    return 1;
}

int file_size(string file) {
    return (int) __MASTER_OBJECT__->master_file_size(file);
}



status main(string what) {
    string tmp1, tmp2;
    string pre,rest;
    int i;
    if(!what
    || sscanf(what,"%s/%s",pre,rest)
    || sscanf(what,"%s.%s",pre,rest)) {
      what = "help";
    }
    notify_fail(sprintf("No help for %O\n",what));
    what = subst(what," ","_");
    if(file_size("/doc/help/"+what+"") > 0){
        return more("/doc/help/"+what+"");
    }
    if(file_size("/doc/help/cmds/"+what+"") > 0){
        return more("/doc/help/cmds/"+what+"");
    }
    if(file_size("/doc/help/gameinfo/"+what+"") > 0){
        return more("/doc/help/gameinfo/"+what+"");
    }
    if(file_size("/doc/help/misc/"+what+"") > 0){
        return more("/doc/help/misc/"+what+"");
    }
    if(this_player()->query_security() 
        && file_size("/doc/help/dev/"+what) > 0){
        return more("/doc/help/dev/"+ what );
    }
    if(file_size("/doc/help/news/"+what+"") > 0){
        return more("/doc/help/news/"+what+"");
    }
    if(file_size("/doc/help/policy/"+what+"") > 0){
        return more("/doc/help/policy/"+what+"");
    }
    if(file_size("/doc/help/skills/"+what+"") > 0){
        return more("/doc/help/skills/"+what+"");
    }

    if(file_size("/doc/help/theme/"+what+"") > 0){
         return more("/doc/help/theme/"+what+"");
    }
    if(file_size("/doc/help/pointers/"+what+"") > 0){
         return more("/doc/help/pointers/"+what+"");
    }
    return notify_fail(sprintf("No help for %O\n",what)),0;
}

