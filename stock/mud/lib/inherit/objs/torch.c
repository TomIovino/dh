
#include <mudlib.h>
#include <ansi.h>

inherit TREASURE;


#define NAME (string)this_player()->query_name()

int lit;

void reset(status arg) {
  if(arg) return;
  set_name("torch");
  set_short(BROWN_F+"wooden torch"+NORM);
  set_long("This is a bundle of thatch tied together, with the end "+
           "soaked in oil.\n");
  set_weight(2);
  set_value(1);
}


void init() {
  add_action("light", "light");
  add_action("extinguish","unlight");
  add_action("extinguish", "extinguish");
}






status light(string str) {
  object ob;
  status current_light;

  if(!str) {
    notify_fail("light what?\n");
    return 0;
  }
  if(!(ob = present(str, this_player()))) {
    if(!(ob = present(str, environment(this_player())))) {
      if(!environment(this_player())->id(str)) {
        write("There is no "+ str +" here.\n");
        return 1;
      }
    }
  }
  if(ob != this_object()) return 0;
  if(lit) {
    write("It is already lit.\n");
    return 1;
  }
  current_light = (set_light(0) > 0) ? 2 : 0;
  set_value(0);
  adj_light(3);
  lit = 1;
  if(!current_light && set_light(0) > 0) {
    write("You can see again.\n");
    say(NAME +" lights a "+ query_name() +".\n");
  } 
  else {
    write("The torch is lit.\n");
  }
  set_short(BROWN_F"wooden torch "RED_F"("YELLOW_F"lit"NORM+RED_F")"NORM);
  return 1;
}


status extinguish(string str) {
  object ob;
  status current_light;

  if(!str) {
    notify_fail("extinguish what?\n");
    return 0;
  }
  if(!(ob = present(str, this_player()))) {
    if(!(ob = present(str, environment(this_player())))) {
      if(!environment(this_player())->id(str)) {
        write("There is no "+ str +" here.\n");
        return 1;
      }
    }
  }
  if(ob != this_object()) return 0;
  if(!lit) {
    write("There "+ query_name() +" is not lit!\n");
    return 1;
  }
  current_light = (set_light(0) > 0) ? 2 : 0;
  adj_light(-3);
  set_value(10);
  lit = 0;
  if(current_light && set_light(0) < 2) {
    write("It turns dark.\n");
    say(NAME +" extinguishes the only light source.\n");
  }
  else {
    write("The torch goes out.\n");
  }
  set_short(GREY_F+"wooden"+BROWN_F+" torch"+NORM);
  return 1;
}




