#define BAN "/secure/banish_d"
int main(string arg) {
    string ip;
    int subnet;
    string *parts;
    string rest;
    
     if(arg == "show"){
   BAN->show();
      return 1;
     }
    if(sscanf(arg,"-sub %s",rest)) {
        subnet++;
        arg = rest;
    }

    if(sizeof(explode(arg,".")) == 1) {
        catch(ip = (string) "/secure/player_d"->query_ip(arg));
        if(ip && stringp(ip))
            arg = ip;
        else
            return notify_fail(sprintf("Player %O doesn't exist.\n"));
    }
    if(subnet) {
       parts = explode(arg,".");
       parts[<1] = "*";
       arg = implode(parts,".");
    }
    printf("Banning %O...",arg);
    BAN->BanIPNumber(arg);
    printf("done.\n");
    return 1;
}

