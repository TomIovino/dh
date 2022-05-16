 /***************************************************************************/
 /* goto: goto the room of a player, otherwise goin object */
#include <mudlib.h>
#include <levels.h>

main(str){
  object ob, env;
  string sh;
  string file;
  int clone_number;


  if(!str){
    write("Usage: Goto <object>\n\n"+
          "Notes: Goto the environment of the <object>, otherwise go into\n"+
          "       the <object>. If you find yourself inside a monster,\n"+
          "       then the monster is loaded, but not cloned ;).\n");
    return 1;
  }
  catch(call_other(eval_path(str), "???"));
  ob = find_player(str) ? find_player(str) : find_living(str) ? find_living(str) : find_object(eval_path(str));
  if(!ob)
    return printf("Can not locate %s.\n", str),1;

  sscanf(file_name(ob),"%s#%d",file, clone_number);

  sh = (ob->short()) ? (string)ob->short() : file_name(ob);

  if(!file) {
    if(ob->query_npc()) {
      write(sh+" is loaded, but not cloned.\n"+
        "Cannot move into a living object that is not cloned.\n");
      return 1;
    }
    this_player()->move_player("X",ob);
    return 1;
  }

  env = environment(ob);
  if(!env){
     write("Could not find environment of "+sh+".\n"+
           "Moving into "+sh+".\n");
     this_player()->move_player("X",ob);
     return 1;
  }

  if(env == this_player()) {
     write("You cannot goto "+sh+"\n"+
           sh+" is in your inventory.\n");
     return 1;
  }
  write("You goto "+sh+"\n");
  this_player()->move_player("X",env);
  return 1;
}

help() {
    write("Usage: Goto <object>\n\n"+
          "Notes: Goto the environment of the <object>, otherwise go into\n"+
          "       the <object>. If you find yourself inside a monster,\n"+
          "       then the monster is loaded, but not cloned ;).\n");
    return 1;
}

