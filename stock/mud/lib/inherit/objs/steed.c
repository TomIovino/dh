/* A Paladin's Steed
 * This object is present as a kind of follower for a paladin of
 * reasonable level. It can be mounted, and when this is done,
 * the steed is destructed and the paladin's short/long/etc is
 * shadowed to make it look like he is riding it.
 */

#define STEED            "obj/shadows/steed"
#define FOLLOW           "obj/shadows/follow"
#define NAME             (string)this_player()->query_name()

#include <mudlib.h>

string owner;
object follow;


void check_for_owner();

inherit MONSTER;

void reset(status arg) {
  ::reset(arg);
  if(arg) return;

  set_short("A fine steed");
  set_alias("horse");
  set_name("warhorse");
  set_long("A fine heavy warhorse of excelent quality and breeding.\n");
  set_level(10);
  set_race("horse");

  /* other stats are taken care of themselves */

  set_msgin("gallops in");
  set_msgout("gallops out");
  set_gender(2);
  set_dead_ob(this_object());
  load_chat(2,({ "Warhorse neighs.\n", "Warhorse looks around.\n", }));
  load_a_chat(5,({ "Warhorse kicks hard.\n", "Warhorse neighs.\n", }));
  set_attack_msg(({
    "bites", "gently",
    "grazes", "softly",
    "kicks",  "lightly",
    "bites",  "meanacingly",
    "kicks",  "with a crushing blow",
    "bites viciously",  "with fervour",
    "kicks powerfully", "with its rear legs",
  }));
}

void monster_died(object obj) {
  if(owner == (string)this_player()->query_name()) {
    tell_object(this_player(),
      "Such an act is looked down upon by the gods.\n"+
      "Lightning strikes you from the heavens!\n");
    this_player()->death();    /* yes...that bad! */
  }
}

void init() {
  add_action("horse_mount", "mount");

  /* taken from the charm_ob.c funs ...paladin doesnt need to persuade it */
  add_action("horse_commands", query_name(1));
  add_action("horse_commands", query_name());
  check_for_owner();
  ::init();
}

void check_for_owner() {
  if(!present(owner, environment()))
    destruct(this_object());
}

status horse_commands(string str, status forced) {
  string arg;
  object ob, *obj;
  int i;

  if(!forced && (string)this_player()->query_name() != owner) return 0;

  if(!str) {
    notify_fail(query_name(1) +" what?\n");
    return 0;
  }
  sscanf(str, "%s %s", str, arg);

  switch(str) {
    case "kill":
      if(!arg) {
        write(this_object()->query_name() +" looks aggressive.\n");
        say(this_object()->query_name() +" puffs up and looks aggressive.\n");
      }
      else if(!(ob = present(arg, environment(this_player())))) {
        write(this_object()->query_name() +" looks behind you and doesn't "+
          "see "+ arg +".\n");
        say(this_object()->query_name() +" starts looking for "+ arg +".\n");
      }
      else if(!living(ob)) {
        tell_room(environment(this_player()),this_object()->query_name()+
          " picks up the "+arg+" gives it a shake, then drops it.\n");
      }
      else if(this_object()->query_secondary_attacker(ob)) {
        write(this_object()->query_name()+
               " gives an extra aggressive attack.\n");
      }
      else {
        tell_room(environment(this_player()),
            this_object()->query_name()+" leaps upon "+ arg +".\n");
        this_player()->add_secondary_attacker(ob);
        this_object()->add_secondary_attacker(ob);
      }
    break;

    case "get":
      if(!arg) {
        tell_room(environment(this_player()),
            this_object()->query_name() +" drops to the ground.\n");
      }
      if(arg == "all") {
        obj = all_inventory(environment());
        for(i=0; i<sizeof(obj); i++) {
           if(obj[i]->short())
             horse_commands("get "+obj[i]->query_name(), 1);
        }
      }
      else if((ob = present(arg, environment(this_object())))) {
        if(ob->get()) {
          tell_room(environment(this_player()),
            this_object()->query_name() +" gets the "+ arg +".\n");
          move_object(ob, this_object());
          this_object()->add_weight((int)ob->query_weight());
        }
        else {
          write(this_object()->query_name() +" trys but can't get it.\n");
        }
      }
      else {
        tell_room(environment(this_object()),this_object()->query_name() + 
            " looks around for the "+arg+".\n");
      }
    break;


    case "drop":
      if(!arg) {
        tell_room(environment(this_player()),
            this_object()->query_name() +" drops to the ground.\n");
      }
      if(arg == "all") {
        obj = all_inventory(environment());
        for(i=0; i<sizeof(obj); i++) {
           if(obj[i]->short())
             horse_commands("drop "+obj[i]->query_name(), 1);
        }
      }
      else if(ob = (present(arg, this_object()))) {
        if(!ob->drop()) {
          tell_room(environment(this_player()),
              this_object()->query_name()+" drops the "+ arg +".\n");
          move_object(ob, environment(this_object()));
          this_object()->add_weight((int)ob->query_weight()*(-1));
        }
        else {
          write(this_object()->query_name() +" refuses to drop it.\n");
        }
      }
      else {
        tell_room(environment(this_object()),
               this_object()->query_name() +" looks around for the "+
                 arg +".\n");
      }
    break;

    case "follow":
      follow = clone_object(FOLLOW);
      follow->follow(this_player(), this_object());
      write(this_object()->query_name() +" waits for you to leave.\n");
      say(this_object()->query_name() +" watches "+ NAME +" closely.\n",
          this_player());
    break;

    case "stay":
      if(!follow) {
        write(this_object()->query_name() +" is not following you.\n");
      }
      else {
        destruct(follow);
        tell_room(environment(this_player()),
               this_object()->query_name() +" gives up the chase.\n"); 
      }
    break;

    case "wear":
      write(this_object()->query_name()+" can't wear anything!\n");
    break;

    case "wield":
      write(this_object()->query_name()+" can't wield anything!\n");
    break;

    case "leave":
      say(this_object()->query_name()+" gallops off into the distance.\n");
      destruct(this_object());
    break;

    default:
      if(!command(str +" "+ arg, this_object())) {
        write(this_object()->query_name() 
               +" looks at you in a confused mannor.\n");
        say(this_object()->query_name() +
          " looks at "+ NAME +" in a confused mannor.\n", this_player());
      }
    break;
  }
  return 1;
}

string set_owner(string str) {
  return owner = str;
}

status horse_mount() {
  int i;
  object *obj;

  if(owner != (string)this_player()->query_name()) {
    write(this_object()->query_name()+" wearily moves away from you.\n");
    say(this_object()->query_name()+" wearily moves away from "+
    this_player()->query_name()+".\n", this_player());
    return 1;
  }

  if(first_inventory(this_object()) &&
    !first_inventory(this_object())->id("soul")) {
    write(this_object()->query_name()+" can't carry you as well as all "+
          "these items.\n");
    obj = all_inventory(this_object());
    for(i=0; i<sizeof(obj); i++) {
      if(obj[i]->short())
        say(this_object()->query_name()+" drops "+obj[i]->query_name()+"\n");
    }
  }

  write("You mount your steed.\n");
  say(this_player()->query_name()+" mounts "+this_player()->query_possessive()
      +" steed.\n", this_player());
  clone_object(STEED)->shadow_horse();
  destruct(this_object());
  return 1;
}
