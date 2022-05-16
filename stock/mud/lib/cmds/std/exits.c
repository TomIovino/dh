#include <target.h>
#include <color.h>

string *query_cmd_aliases() 
{       return ({
        "exits","town","o","out","gate",
        "forest","trail","enter","x",
        "cave","hole","door",
        "north","east","south","west","down","up",
        "nw","sw","ne","se",
        "northwest","southeast","southwest","northeast",
        "n","e","w","s","u", 
        }); 
}
void print_exit(string exit, string path)
{    if(exit && path)
       write(exit+"\t");
}
void move_exit(string exit, string path)
{   
//  object env;
//  env = find_object(path);
//  env->wake();

  if(exit && path)
   if(find_object(path))
    {  write("moving -> "+exit+"\n");
       move_object(this_player(),path);
    } else {
       clone_object(path);
       move_object(this_player(),path);
       write("init/move-> "+exit+"\t");
    }

}
int main(string path)
{    mapping exits;
     object find, *inv;
     int size,i;
     string x,y;
     path = query_verb();
     find = this_object()->target();


    switch(path){
     case "x": path = "enter"; break;
     case "o": path = "out"; break;
     case "d": path = "down"; break;
     case "n": path = "north"; break;
     case "nw": path = "northwest"; break;
     case "ne": path = "northeast"; break;
     case "e": path = "east"; break;
     case "w": path = "west"; break;
     case "s": path = "south"; break;
     case "sw": path = "southwest"; break;
     case "se": path = "southeast"; break;
     default: break;
    }

  // write("p:"+path+"\n");

  if(sscanf(this_player()->query_current_room(), "%s/%s", x,y) ==2
     && x != "env") 
   { //  write("old");
      environment(this_player())
       ->ready_to_move(this_player(),path);
      return 1;
   }


   exits = find->query_data("exits");

   if(path == "exits")
   { if(exits)
     { write(NM+"Exits: ");
       walk_mapping(exits, #'print_exit);
       return 1;
     } else
     { write("No exits!\n"); 
       return 1;
     }
   } 
   if(exits) 
   { if(path && exits[path])
      { move_exit(path, exits[path]); 
      } else 
      { write("You cant go that way from here!\n");
      }
    }
return 1;
}


