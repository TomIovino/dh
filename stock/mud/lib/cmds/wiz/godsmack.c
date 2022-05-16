

int main(string who){
     object v;
     v = find_player(who);
     v->set_mp(1);
     v->set_sp(1);
     v->set_hp(1);
     return 1;
}
