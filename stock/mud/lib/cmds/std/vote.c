#define TP this_player()
#define ISSUE "eqzapspl"
#define VLV 50

vote(string how);

status main(string vote){
 write(
 "All players over alt level "+VLV+" may vote once!\n"+
 "Do you want a EQUIPMENT disintergration SPELL in the game?\n"+
 "Do you want a EQUIPMENT disintergration SKILL in the game?\n"+
 "yes - equipment loss is fun.\n"+
 "no  - equipment loss is not fun.\n"+
 "vote [yes/no]\n>");
 input_to("vote");
 return 1;
}

vote(string how){
 if(how == "yes") how = "yes";
 if(how == "no") how = "no";
 if(how != "yes" && how != "no"){
 write("Invalid vote choice, your vote was NOT recorded.\n");
 write("You may try to vote again.\n");
 return 1;
 }
 if(TP->query_alt_level() < VLV){
  write("Must be level "+VLV+"+ to vote.\n");
  return 1;
 }
 if(TP->query_attrib("issue") == ISSUE){
  write("You have already voted on this issue.\n");
  return 1;
 }
  TP->set_attrib("issue",ISSUE);
  log_file("VOTE", TP->query_real_name()+" "+ISSUE+" "+how+
  "[d:"+TP->query_member()+"] "+
  "[L:"+TP->query_level()+"] "+
   ctime()+"\n");
 
 write(
 "You voted "+how+" to "+ISSUE+" topic.\n");
 return 1;
 }


