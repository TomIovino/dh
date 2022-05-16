#include <mudlib.h>
#include <ansi.h>

inherit ROOM;

reset (arg) {
    if (arg) return;
    set_short(RED_F"Twilite Bounty Agency"NORM);
   set_light(10);
    set_long(
      RED_F+
      "This shop is surrounded by walls of dark grey granite. There is\n"+
      "not a lot in here except a desk strewn with papers, a small wooden chair\n"+
      "set behind it, and a bulletin board mounted on the south wall. Above the\n"+
      "desk hangs a tarnished copper sign.\n"+
      NORM);
    set_items(({
	"d#down#floor","The floor is made of smooth grey granite.\n",
	"u#up#ceiling","The ceiling is made of smooth grey granite.\n",
	"wall#walls","The walls are made of smooth grey granite.\n",
	"sign","It seems to have some writing on it.\n",
	"board","Maybe try reading it.\n",
	"desk","It's a wide beaten up old wooden desk strewn with a clutter of papers.\n",
	"papers#paper","A variety of randomly strewn papers clutter the desk.\n",
	"chair","A small wooden chair is set behind the desk.\n"
      }));
    set_smell("The scent of mildewed paper hangs lightly in the air.\n");
    set_listen("All is very quiet in here.\n");
    set_light(1);
    set_exits(({
     "d/town/market/ave5","north",
      }));
}
init() {
    ::init();
    add_action("read","list");
    add_action("read","read");
    add_action("place","place");
    add_action("collect","collect");
    add_action("help","help");
}
read(str) {
    int i;
    mixed *bounty;
    if(!str || str == "sign"){
        write(read_file("doc/help/bounty_help"));
	return 1; }
    if(str=="board") {
	bounty=get_dir("/open/bounty/*",0x21);
	for(i=0;i<sizeof(bounty);i++) {
	    write(bounty[i]);
	    write("\n"); }
	write("..\n.\n");
	return 1; }
    if(!read_file("/open/bounty/"+capitalize(str))) {
	write("\nThere is no bounty on anyone by that name.\n");
	return 1; }
  write(capitalize(str)+" wanted dead, bounty "+read_file("/open/bounty/"+str) 
  +" coppers, no questions asked.\n");
    return 1;
}
place(str) {
    string a;
    int b;
    int new,old;
    if(!str) return 0;
    if(!present("kilagin")) {
	write("The bounty agent does not seem to be here right now.\n");
	return 1; }
    if(sscanf(str,"bounty on %s for %d",a,b)!=2) {
	write(CYAN_F+"\nKilagin asks: Huh?\n"+NORM);
	return 1; }
//    if(read_file("/open/bounty/"+a)) {
//        write("Im sorry that person already has a bounty on them!\n");
//        return 1; }
    if(b<0) {
	present("kilagin")->set_primary_attack(this_player());
	write(CYAN_F+"\nKilagin growls threateningly.\n"+
      "Kilagin exclaims: Get out of here before I kill you, knave!\n"+NORM);
	return 1; }
    if(b<11111) {
    write(wrap(CYAN_F+"\nKilagin says: I'm sorry but the minimum bounty is"
    +" ten gold... after the processing fee, of course.\n"+NORM),78);
	return 1; }
    if(b>1111111) {
    write(wrap(CYAN_F+"\nKilagin says: I'm sorry but the maximum bounty is"
    +" one thousand gold... after the processing fee, of course.\n"+NORM),78);
	return 1; }
    if(this_player()->query_money()<b) {
	write("\nKilagin says: You don't have that much money.\n"+NORM);
	return 1; }
    this_player()->adj_money(-b);
    log_file("BOUNTY",
this_player()->query_name()+" placed a bounty of "+b+" coins on "+capitalize(a)+".\n");
    b=b-(b/10);
    if(!read_file("/open/bounty/"+a)) {
	write(
	  CYAN_F+"\nKilagin says: Ok, deducting our 10% processing fee... that puts a bounty of "+b+" coppers on the head of "+capitalize(a)+".\n"+
	  "\nKilagin smiles happily.\n"+
	  "\nKilagin says: Thank you for your business, do come again, and have a nice day.\n"+NORM);
	write_file("/open/bounty/"+a,to_string(b));
	return 1; }
    write(
      CYAN_F+"\nKilagin says: Ok, deducting our 10% processing fee... that adds "+b+" coppers to the bounty on the head of "+capitalize(a)+", I guess "+capitalize(a)+" is a real popular person.\n"+
      "\nKilagin grins.\n"+
      "\nKilagin says: Well, thank you for doing business with us, have a nice day, and do call again.\n"+NORM);
    new = 0;
    old = 0;
    new = b;
    old = to_int(read_file("/open/bounty/"+a));
    rm("/open/bounty/"+a);
    b += old;
    write_file("/open/bounty/"+a,to_string(b));
    return 1;
}
collect(str) {
    mixed *quests;
    int money,i;
    if(!str) return 0;
    if(!present("kilagin")) {
	write("\nThe bounty agent doesn't seem to be here right now.\n");
	return 1; }
    if(!read_file("/open/bounty/"+capitalize(str))){
	write(CYAN_F+"\nKilagin says: There is no bounty on the head of "+capitalize(str)+".\n");
	return 1; }
    if(!present("head",this_player())) {
	write(CYAN_F+"\nKilagin says: You must have the victim's head as proof that the deed was done, or I can not pay you.\n"+NORM);
	return 1; }
    if(!present(str+" head",this_player())){
	write(CYAN_F+"\nKilagin says: That is not "+capitalize(str)+"'s head.\n"+NORM);
	return 1; }
    money=to_int(read_file("/open/bounty/"+str));
    if(present("head",this_player())->query_alias()=="kassandra head") {
	quests=this_player()->query_done_quests();
	for(i=0;i<sizeof(quests);i++) {
	    if(quests[i]=="bounty hunter") {
		write(CYAN_F+"\nKilgain takes the head from you and looks at it.\n"+
		  CYAN_F+"\nKilagin says: Nice work, kid, but I ain't paying you for the same job twice.\n"+NORM);
		destruct(present("head",this_player()));
		return 1; } }
	write(CYAN_F+"\nKilagin takes the head from you and looks at it.\n"+
	  "\nKilagin says: Yep, that was "+capitalize(str)+" alright, here's your "+money+" coppers.\n"+
	  "\nKilagin grins.\n"+
	  "\nKilagin says: Nice job, "+this_player()->query_name()+".\n"+NORM);
	this_player()->adj_money(money);
        log_file("BOUNTY",this_player()->query_name()+" collected "+money+
          " coins on "+str+"'s bounty.\n");
	this_player()->set_quest("bounty hunter");
	this_player()->finish_quest("bounty hunter",1);
	this_player()->set_quest("None");
	this_player()->set_quest_hint("Find a quest.\n");
	destruct(present("head",this_player()));
	return 1; }
    if(this_player()->query_id()==str) {
	write(CYAN_F+"\nKilagin chuckles politely.\n"+
	  "Kilagin says: Well, I guess there's no rule against collecting a bounty on yourself... but you sure must be desperate for money to kill yourself for it...\n\n"+
	  NORM+"\nKilagin gives you "+money+" copper coins.\n"+
	  CYAN_F+"\nKilagin snickers.\n"+
	  "\nKilagin says: Thanks.\n"+
	  "\nKilagin takes the head from you and tosses it over his left shoulder where it rattles unpleasantly into a trash barrel.\n"+NORM);
	log_file("BOUNTY", this_player()->query_name()+" collected "
        +money+" coins on "+str+"'s bounty.\n");
	this_player()->adj_money(money);
	destruct(present("head",this_player()));
	rm("/open/bounty/"+str);
	return 1; }
    write(CYAN_F+"\nKilagin takes the head from you and looks at it.\n"+
      "\nKilagin says: Yep, that was "+capitalize(str)+" alright, here's your "+money+" coppers.\n"+
      "\nKilagin grins.\n"+
      "\nKilagin says: Nice job, "+this_player()->query_name()+".\n"+NORM);
    this_player()->adj_money(money);
    log_file("BOUNTY",
this_player()->query_name()+" collected "+money+" coins on "+str+"'s bounty.\n");
    destruct(present("head",this_player()));
    rm("/open/bounty/"+str);
    return 1;
}
help(str) {
    if(!str) return 0;
    if(str!="bounty") return 0;
    write(read_file("/doc/help/bounty_help"));
    return 1;
}
