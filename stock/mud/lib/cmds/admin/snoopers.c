
int main(string arg) {
    mapping snoopers;
    object *u, who;
    int i, j;

    if(this_player() != this_interactive()
    || this_interactive()->query_security_level() < 90) {
        this_interactive() != this_player() && tell_object(this_interactive(),"Nope sorry. No dice.\n");
        tell_object(this_player(),"Nope sorry. No dice.\n");
        return 1;
    }

    i = sizeof(u = users());

    snoopers = ([ ]);

    while(i--)
        if(who = query_snoop(u[i]))
            snoopers[who] = u[i];

    j = i = sizeof(u = m_indices(snoopers));

    if(i) {
        while(i--)
            if(!u[i]->query_attrib("DNDs")) 
                printf("%-20s -> %-20s\n",capitalize((string)u[i]->query_real_name()),capitalize((string)snoopers[u[i]]->query_real_name()));
            else
                j--;
    } else
        return write("snoopers:  No snoops currently active.\n"),1;

    printf("snoopers: %d snoop%c currently active.\n",j,j == 1 ? 0 : 's');
    return 1;
}

int help(string arg) {
    write("Command:  snoopers\n"
          "Command Class:  Admin\n"
          "\n"
          "Syntax:  snoopers\n"
          "\n"
          "Purpose:  Shows who is snooping whom.\n");
    return 1;
}

