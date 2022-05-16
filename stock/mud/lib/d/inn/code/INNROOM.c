#define TP this_player()
#include <mudlib.h>
#include <ansi.h>
inherit ROOM;

reset (arg) {
  if (arg) return;
   set_short("Inn Room");
   set_long(
GREY_F
"A white walled room above the inn proper. A window on the west wall\n"
"overlooks the central square of Twilite. The east wall contains a\n"
"fireplace and a nice storage closet. On the north wall a dresser sits\n"
"in the far corner near a wood framed bed. West is a wooden door.\n");
   set_no_magic(1);
   set_no_steal(1);
   set_no_fight(1);
   set_no_track(1);
 add_object("movement rune","/cmds/skills/movement");
 add_object("mana rune","/cmds/skills/mana");
 add_object("health rune","/cmds/skills/health");
   set_light(2);
   set_items(({
   "wall#walls","Well maintained white walls. A symbol hangs on the wall.\n",
   "symbol","A magical symbol of protection hangs on the wall.\n",
   "window#outside","Always a beautiful day in Twilite.\n",
   "fireplace","Burning softly it keeps the room warm.\n",
   "dresser", "Its a nice wooden one.\n",
   "closet","Its a nice spacious closet\n",
   "bed","A comfortable bed for sleeping after a hard day killing.\n",
   "door","It leads back to the Inn Commons.\n",
   }));
   set_items(({
   "wall#walls","Well maintained white walls. A symbol hangs on the wall.\n",
   "symbol","A magical symbol of protection hangs on the wall.\n",
   "window#outside","Always a beautiful day in Twilite.\n",
   "fireplace","Burning softly it keeps the room warm.\n",
   "dresser", "Its a nice wooden one.\n",
   "closet","Its a nice spacious closet\n",
   "bed","A comfortable bed for sleeping after a hard day killing.\n",
   "door","It leads back to the Inn Commons.\n",
   }));
   set_exits (({
   "/d/inn/code/COMMON","west",
   }));


}

init(){
   ::init();
   add_action("read","read");
   add_action("enter","enter");
   add_action("enter","upto");
   add_action("invite","invite");
   add_action("uninvite","uninvite");
   add_action("purchase","purchase");
}

int read(string what){
    if(what != "sign") return 0;
write("--------------------------------------------------------------\n");
  write("commands for the Inn are:\n");
  write("upto <room name>\n");
  write("invite <person>    (only lasts till they relog)\n");
  write("uninvite <person>  (wont allow them back in the room)\n");
  write("purchase <personalroom> (at a cost of 10000 gold)\n");
  write("No admitance except spouses in personal rooms.\n");
  write("With the except of those who are invited.\n");
write("--------------------------------------------------------------\n");
   return 1;
}

int invite(string player){
    object who;
  if(!player){ return 0; }
    who = find_player(player);
    if(!who || who->query_invis()){
      write("No such player online.\n");
      return 1;
    }
    who->set_tmp_attrib(TP->query_real_name(),1);
    write("INVITED "+player+"\n");
    return 1;
    }
int pinvite(string player){
    object who;
  if(!player) return 0;
    who = find_player(player);
    if(!who || who->query_invis()) {
    write("No such player online.\n");
      return 1;
    }
    who->set_tmp_attrib(TP->query_real_name(),1);
    write("INVITED "+player+"\n");
    return 1;
  }
int uninvite(string player){
    object who;
   if(!player) return 0;
    who = find_player(player);
    if(!who || who->query_invis())  
    { 
    write("No such player online.\n");
    return 1; }
    who->set_tmp_attrib(TP->query_real_name(),-1);
    write("UNINVITED "+player+"\n");
    return 1;
  }
int purchase(string type){
    string room;
    if(this_player()->query_no_save()){
       write("Try again when the mud is open.\n");
    return 1;
    }
    if(type == "personalroom") room = TP->query_real_name();
   if(!room){ write("Invalid selection, read the sign!!\n");
              return 1; }
   if(this_player()->query_money() < 1000000){
        write("NO Loans, 10000 gold to purchase a room!!\n");
              return 1; }
   if(file_size("/d/inn/"+room+".c") == -1){
     call_other("/secure/master","make_inn_room",room);
     this_player()->adj_money(-1000000);
     write("purchased room, type: upto "+room+"\n");
   return 1;  }
   write("That room exists!\n");
   return 1;
}

int enter(string what){
   if(file_size("/d/inn/"+what+".c") == -1){
   write("That room does not exist.\n");
   return 1;
   }
   if(TP->query_position() != "standing" &&
      TP->query_position() != "sneaking"){
    write("You need to stand to enter a room!\n");
    return 1;
  }
   if(what != TP->query_real_name() &&
      what != TP->query_spouse() &&
      TP->query_attrib(what) != 1){
   write("You dont own that room or have an invitation.\n");
   return 1;
   }
   this_player()->move_player("inn room","/d/inn/"+what+".c");
   return 1;
}

