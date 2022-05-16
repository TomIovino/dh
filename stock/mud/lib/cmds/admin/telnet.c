status _telnet(string arg) {
    return "/secure/socketd/_telnet"->main(arg);
}
status help() { return "/secure/socketd/_telnet"->help(),1; }
