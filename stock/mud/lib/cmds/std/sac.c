#define SAC_TO "/inherit/objs/churchchest"

string *query_cmd_aliases() { return ({ "donate","don","sacrifice", }); }

#include <valid.cfg>
#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
#define TP this_player()
status main(string arg) {
   object don;
   object chest;
    if(!arg){ write("sacrifice WHAT?\n"); return 1; }
        if(!don = present(arg, this_player())) {
            printf("You dont have that.\n");
            return 1;
        }
    if(don->query_wc() == 0 && don->query_ac() == 0){
     write("You cant donate that.\n");
    return 1;
}
    if(this_player()->query_primary_attack()){ return 1; }
   if(don->query_wielded() || don->query_worn()){
         printf("Remove that item first.\n");
      return 1;
    }
   if(!don->query_data("donater")){
    log_file("SLOG.dat", this_player()->query_name()+
       " donated "+don->query_name()+"\n");
   don->set_data("donater",this_player()->query_name());
   don->set_value(1);
    }
   if(don->drop()){
       write("You cant drop it!\n");
       return 1;
   }
   move_object(don,SAC_TO);
   this_player()->recalc_carry();
   this_player()->recalc_wc();
   this_player()->recalc_ac();
   
  write(this_player()->do_cc("")+capitalize(this_player()->query_alignment())+
    " accepts your donation.\n"+NORM);
  return 1;
}

