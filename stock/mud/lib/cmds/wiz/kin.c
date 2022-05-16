#include <ansi.h>
object me;
string name;

string *query_cmd_aliases() { return ({"elderkin", }); }

status main(){
     me = this_player();
     name = me->query_alt_name();
     if(me->query_attrib("elderkin")){
        write("You are already in elderkin form.\n");
        return 1;
     }

   if(!this_player()->query_security_level())
     if(this_player()->player_race_ranking() > 3){
          write("Your power is too weak.\n");
          return 1;
     }
/*
     if("secure/realm_d"->query_member(me->query_alignment(),
          me->query_real_name()) < 7){
          write("Your bloodline is too weak.\n");
          return 1;
     }
*/
     write("You channel intense power and call on the powers\n"
           "of your ancestors to aid you in your struggles.\n");
     me->set_tmp_attrib("elderkin",1);
     me->adj_apb(5);
     me->adj_cpb(5);
     me->adj_dpb(5);
     me->adj_spb(5);
     return 1;
}
