
#include <mudlib.h>
#include <ansi.h>
#include <bar.h>

#define NAME_D "/cmds/admin/nameban"

#define MAX_NAME_LEN 14

status main(string str) {
      string title, name, frag, *ltr;
    title = (string) this_player()->query_title();
    if(!str){ return this_object()->help(); }
    if(title  != "" || this_player()->query_data("NO_TITLE")){
  write("You have a title contact a wizard if you wish to have it removed.\n");
      return 1;
    }
    if(strlen(str) > MAX_NAME_LEN) {
               notify_fail("Try something shorter.\n");
        return 0;
    }
    if(sizeof(ltr = explode(str = lower_case(str),"") - explode("abcdefghijklmnopqrstuvwxyz",""))) {
        notify_fail("Lastnames names may only consist of the letters a-z.\n");
        return 0;
    }

    if(!this_player()->query_days_old()){
      write("You must be one day old to qualify for a lastname.\n");
      return 1;
    }
    if(NAME_D->name_is_banned(str,&frag)) {
        notify_fail(sprintf("That name contains the banned name fragment %O.\nChoose another name and try again.\n",frag));
        return 0;
    }
/*
    if(this_player()->query_money() < 100000){
        write("You cannot seem to afford fee of 100000 coins.\n");
        return 1;
    }
    this_player()->adj_money(-100000);
*/
    log_file("SOCIAL","LASTNAME "+this_player()->query_name()+
             " "+str+" "+ctime()+"\n");
    str = capitalize(str);
    this_player()->set_title(str);
    write("You are now "+this_player()->query_name()+" "+
                         this_player()->query_title()+".\n");
    return 1;
}


status help() {
     write("lastname <what you want>\n");
     write("This will set your lastname, beware you only can set it once\n"+
           "after that you have to contact an admin. Any titles that are\n"+
           "lack good judgement will result in either that player not\n"+
           "getting a lastname or worse..In other words look at what\n"+
           "other players have to see what is acceptable if in doubt\n"+
           "contact a wizard for confirmation of acceptance.\n");
   write("Players must be 24 hours old to qualify for a lastname.\n");

    return 1;
}

