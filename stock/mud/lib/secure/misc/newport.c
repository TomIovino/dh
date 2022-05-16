void logon() {
    write("-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-\n"
          " DragonHeart III     DragonHeart III     DragonHeart III     DragonHeart III\n"
          "-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-=*=-\n"
          "\n"
          "    We have moved (slightly)! You will now find us running on port 8888\n"
          "\n"
          "                            mud.calbbs.com 8888\n"
          "                                           ~~~~\n"
          "\n"
          "    If you have *any* problems connecting to us let Geewhiz know ASAP.\n"
          "\n"
          "    Thank you!\n"
          "\n"
          "-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - --\n"
          "                                           (Geewhiz <atgraham@ptdprolog.net>)\n"
          "-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - --\n");                                           
    write("<hit enter to disconnect> ");
    call_out("time_out",120);
    input_to("time_out");
    return;
}

void time_out(string trash) {
    write("\nDisconnecting...\n");
    write("Remember:\n"
          "\t* mud.calbbs.com 8888\n");
    destruct(this_object());
}

