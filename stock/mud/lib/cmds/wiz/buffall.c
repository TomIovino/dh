#include <ansi.h>

int main() {
    string *spells;
    object *u;
    mapping ss;
    int s, i;
    i = sizeof(u = users());
    s = sizeof(spells = 
    m_indices(ss = ([ "agility" : 10; 300, "strength" : 10; 300, 
                     "stamina" : 10; 300, "dexterity" : 10; 300, ])));
    while(i--) {
        s = sizeof(spells);
        while(s--) {
            u[i]->set_var(spells[s],ss[spells[s],0]);
            u[i]->set_var(spells[s]+"_time",ss[spells[s],1]);
             tell_object(u[i],NORM+MAGENTA_F+capitalize(spells[s])+
             " has been bestowed on you by the Phoenix.\n"NORM);

        }
    }
    return 1;
}

