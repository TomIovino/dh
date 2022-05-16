#include <ansi.h>

int main() {
    string *spells;
    object *u;
    mapping ss;
    int s, i;
    string X;
    X = this_player()->query_name();
    i = sizeof(u = users());
    s = sizeof(spells = 
    m_indices(ss = ([ 
                       "blind" : 100; 25, ])));
    while(i--) {
        s = sizeof(spells);
        while(s--) {
            u[i]->set_var(spells[s],ss[spells[s],0]);
            u[i]->set_var(spells[s]+"_time",ss[spells[s],1]);
            tell_object(u[i],NORM+MAGENTA_F+"\nA " + spells[s] +
           " has been bestowed on you by "+X+".\n"NORM);
        }
    }
    return 1;
}

