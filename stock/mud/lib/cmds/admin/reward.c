

int main(string who) {
    string *spells;
    mapping ss;
    int s, i;
    object favored;
    if(who)
    favored = find_player(who);
    if(!favored){ write("cant favor.\n"); return 1; }
            favored->set_var("sight",75);
            favored->set_var("sight_time",999);

            favored->set_var("breath",75);
            favored->set_var("breath_time",999);

            favored->set_var("wings",75);
            favored->set_var("wings_time",999);

            favored->set_var("invisibility",75);
            favored->set_var("invisibility_time",999);

            favored->set_var("awareness",75);
            favored->set_var("awareness_time",999);

            favored->set_var("bless",75);
            favored->set_var("bless_time",999);

            favored->set_var("heroism",75);
            favored->set_var("heroism_time",999);

            favored->set_var("absorb",75);
            favored->set_var("absorb_time",999);

            favored->set_var("avenger",75);
            favored->set_var("avenger_time",999);

            favored->set_var("defender",75);
            favored->set_var("defender_time",999);

            favored->set_var("barrier",75);
            favored->set_var("barrier_time",999);

            favored->set_var("reflect",75);
            favored->set_var("reflect_time",999);

            favored->set_var("deflect",75);
            favored->set_var("deflect_time",999);
 
     tell_object(favored,"\nAsh has granted you a favor!\n");
       command("aff",favored);     
    return 1;
}

