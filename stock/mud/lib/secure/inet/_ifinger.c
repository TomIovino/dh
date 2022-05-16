status _ifinger(string arg) {
    return "/secure/socketd/_ifinger.c"->main(arg);
}
status help() { return "/secure/socketd/_ifinger"->help(),1; }
