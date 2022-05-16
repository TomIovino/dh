#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
#define TP this_player()
status main(string str) {
 
  TP->log_for_players("TYPO",str);
  if(str)
  write("thank you!\n");
  if(!str)
  write("what is the idea?\n");
  return 1;
}
