#define TP this_player()
#include <mudlib.h>
#include <ansi.h>
inherit ENV;

reset (arg) {
    add_object("xuma","d/inn/guard/xuma");
  if (arg) return;
     set_short("Upstairs in the Pleasant Inn");
   set_long(RED_F+
"The white walled hallway of the inn attests to the fact that\n"+
"despite the heavy use of its patrons the owners keep the place in\n"+
"very good shape. Many doors lead directly to the various rooms here.\n"+
"The stairs leads down to the inn proper.\n"+
   NORM);
   set_light(2);
   set_items(({
   "wall#walls","Well maintained white walls.\n",
   "window#outside","Always a beautiful day in Twilite.\n",
   "door#doors","The doors lead to many rooms. You may 'enter' doors.\n",
   }));
   set_exits (({
   "/d/inn/code/COMMON","down",
   }));

}

init(){
   ::init();
}

int enter(string what){
   if(file_size("/d/inn/"+what+".c") == -1){
   write("That room does not exist.\n");
   return 1;
   }
   if(what != TP->query_real_name() &&
      what != TP->query_spouse() &&
      what != TP->query_house() &&
      TP->query_data(what) != 1){
   if(present("xuma")){
   write("You dont own that room or have an invitation.\n");
   write("Xuma blocks the door to that room.\n");
   return 1;
    }
   } 
   if(what != TP->query_real_name() &&
      what != TP->query_spouse() &&
      what != TP->query_house() &&
      TP->query_data(what) != 1){
      log_file("TRESSPASS",
      TP->query_name()+ " entered "+what+" on "+ctime()+"\n");   
   }
   this_player()->move_player(what+" inn room","/d/inn/"+what+".c");
   return 1;
}

