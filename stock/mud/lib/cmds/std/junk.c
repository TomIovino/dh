#include <ansi.h>
#include <mudlib.h>

status main(string arg) {
    object junk;
    if(!arg) {
	write("Junk what?\n");
	return 1; }
    junk=present(arg,environment(this_player()));
    if(!junk) junk=present(arg,this_player());
    if(!junk) {
	printf("What "+arg+"?\n");
	return 1; }
    if(this_player()->query_primary_attack()){ return 1; }
    if(junk->query_invis()){
 	printf("What "+arg+"?\n");
	return 1; }
    if(junk->query_wielded() || junk->query_worn()){
	printf("You must remove the item first.\n");
	return 1; }
    if(!junk->query_value()){
        write("That cant be junked.\n");
        return 1; } 
    if(living(junk)) {
	write("Uh huh.\n");
	return 1; }
    if(junk->drop()) {
	write("You can't drop it!\n");
	return 1; }
    if(junk->is_readable()) {
	write("Heh, no.\n");
	return 1; }

    if(junk->query_owner() != ""   &&  ( junk->query_owner()) != 0){
         write("Cannot junk owned items, trying to junk owned items"
               " is a violation of mud policy and subject to discipline.\n");
         return 1;
     }
    log_file("JUNK",
    this_player()->query_real_name() + " JUNKED "+
    file_name(junk)+ " at "+ctime()+"\n");  

    destruct(junk);

    this_player()->recalc_carry();
    write("You junk the "+arg+".\n");
    say(this_player()->query_name()+" junks a "+arg+".\n");
    return 1;
}

int help(){
   write("junk <what>\n");
   return 1;
}
