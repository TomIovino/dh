status main(string arg) {
    string mona, monb;
    object ma, mb, en;
    sscanf(arg,"%s %s",mona, monb);
    en = environment(this_player());
    ma = present(mona,en);
    mb = present(monb,en);
    ma->add_secondary_attacker(mb);
    mb->add_secondary_attacker(ma);
    printf("%s <-> %s\n",mona, monb);
    return 1;
}
