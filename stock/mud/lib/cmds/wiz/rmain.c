#define SUCCESS (printf("clear: Error Log Removed.\n"),1)
#define FAILURE notify_fail("clear:  No Error Log To Remove.\n")
 
status main(string arg) {
    string name;
    status ret;
    name = (string) this_player()->query_name(1);
    (ret = rm(sprintf("/log/log",name))) ? SUCCESS : FAILURE;
    return ret;
}
 
