mapping RegistrationInfo = ([ ]); 
string called_from_ip, ip_number, email;
 
status _restore(string name);
 
status main(string arg) {
    if(!arg || !stringp(arg))
        return notify_fail("reginfo:  Usage:  reginfo <name>\n"),0;
    if(_restore(arg)) {
        printf("Status of %O:\n\t",arg);
        switch(RegistrationInfo[0]) {
           case 0:
               printf("Not Checked.\n");
               break;
           case 1:
               printf("Approved.\n");
               break;
           case 2:
               printf("Requires Registration Password %O\n",
                   stringp(RegistrationInfo[1]) ? RegistrationInfo[1] :
                                                  "??????");
            printf("\tFrom %s (%s)\n\tEmail:  %s\n",called_from_ip,ip_number,email);
               break;
           default:
               printf("Unknown flag %d\n",RegistrationInfo[0]);
               break;
        }
        return 1;
    } else
        return notify_fail(sprintf("reginfo:  Player %O doesn't exist.\n",arg)),0;
}
 
#include <player.cfg>
 
status _restore(string name) {
  name = lower_case(name);
  return restore_object(SAVE_NO_BANISH+name)
      || restore_object(SAVE_WIZARD+name)
      || restore_object(GET_SAVE_PLAYER(name))
      || restore_object(SAVE_FIRST +name);
}
