#define FILE "/doc/help/news/member"

#include <ansi.h>
#include <mudlib.h>
status main() 
   {

   if(!this_player()->query_member() ){
      write("Your not a founder. =)\n");
      return 1;
    }

  cat(FILE);
   return 1;
   }
help()
   { 
   cat(FILE);
   return 1;
   }
