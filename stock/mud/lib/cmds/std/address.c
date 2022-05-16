#include <mudlib.h>
#include <ansi_filter.h>
#include <bar.h>
#define TP this_player()

status help();

status main(string addy){ 
       if(!addy){
           write("Your current address is :\n"+TP->query_address()+"\n");
           return 1;
       }
       if(strlen(addy) > 75){
           write("You address is to long to be entered.\n");
           return 1;
       }
       if(addy == "clear"){
           TP->set_address(0);
           write("You address has been cleared.\n");
           return 1;
       }
        addy = filter_ansi(addy);
        TP->set_address(addy);
        log_file("ADDRESS",
            this_player()->query_real_name()+":::"+addy+"\n");
        write("Your current address is now :\n"+TP->query_address()+"\n");
        write("Type 'address clear' to remove all information.\n");
        help();
    return 1;
}      



int help(){
        write("This space is provided for placing your public\n");
        write("email, icq, aim information, anyone can access it.\n");
        write("Although having a public address is a choice,\n");
        write("providing false information is not permited.\n");
        write("Type: Address <what|clear> to set your information.\n");
        return 1;
}
