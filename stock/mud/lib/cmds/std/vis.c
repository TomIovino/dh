#include <ansi.h>
#include <mudlib.h>

int main(string what);
int help();
int result();


int main(string what){
  if(this_player()->query_invis())
  {
    this_player()->toggle_invis();
    return 1;
  }
  write("You are already visible.\n");
   return 1;
}

int result(){
         write("result\n");
     return 1;
}

int help(){
         write("help\n");
     return 1;
}

