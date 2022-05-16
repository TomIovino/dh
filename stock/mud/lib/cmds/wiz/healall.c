
#include <ansi.h>

int main() {
    object *u;
    int s, i;
    i = sizeof(u = users());
    while(i--) {
    if(this_player()->query_real_name() == "ash")
            tell_object(u[i],NORM+YELLOW_F+
            "\nAsh flies though the sky.\n"+NORM);
    else if(this_player()->query_real_name() == "leela")
            tell_object(u[i],NORM+RED_F+
            "\nLeela drops rose petals down from heaven.\n"+NORM);
    else return 1;
            u[i]->adj_hp(2000);
            u[i]->adj_mp(2000);
            u[i]->adj_sp(2000);
   }
    return 1;
}

