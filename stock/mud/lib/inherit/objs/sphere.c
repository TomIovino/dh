
// 'Crimsons' Exploding Sphere.
// Fax::Dragonheart::11-22-95
// ::This is the result of the Exploding Sphere spell. It will allow the
// ::player to drop it in the current room or throw it in a direction.
// ::It has to be charged, and if it is held to long, it will explode.

#include <ansi.h>
#include <mudlib.h>
inherit TREASURE;

int charged;

status cast_spell(object caster, object target, object prev, int level) {
  set_name("sphere");
  set_alt_name("spell");
  set_alias("spellsphere");
  set_short("a clear sphere");
  set_long("\
The sphere fits easily into the palm of your hand. It appear to made \n\
of some sort of glass, yet it feels much to light for that.\n");
  set_value(0);
  charged = 0;
  tell_object(target, "A sphere magically appears before you.\n");
  return 1;
}

drop() {
  if(charged) return 0;
  else {
    printf("\
The sphere is fragile and dropping it would cause it to break.\n");
    return 1;
  }
}

dest_me() {
  tell_object(this_player(), sprintf("\
The sphere flashes brightly and then vanishes.\n"));
  destruct(this_object());
}

init() {
  add_action("do_charge", "charge");
  add_action("do_throw", "throw");
}

no_sphere(){  // called in a room init for no sphere rooms  - Ash
  rto();
   }
do_charge(string str) {
  object tp;

  tp = this_player();
  if(!str) return 0;
  if(str != "sphere") return 0;
  if(charged) {
    printf("\
The sphere is already active, and charging it more could be fatal.\n");
    return 1;
  }
  if(tp->query_stellar_points() < 5) {
    printf("\
You do not have the energy to charge the sphere.\n");
    return 1;
  }
  tp->adj_stellar_pts(-5);
  printf("\
%sYou stare deeply into the sphere, channelling magical energies into it, \n\
causing it to glow.%s\n", MAGENTA_F, NORM);
  tell_room(environment(tp), sprintf("\
%s%s stares into %s sphere and it begins to glow.%s\n", MAGENTA_F,
  tp->query_name(), tp->query_possessive(), NORM), ({ tp }));
  set_short(B_MAGENTA_F+"a glowing sphere"+NORM);
  set_long("\
The sphere glows with a translucent magenta light. It is warm to the \n\
touch and fits easily into the palm of your hand.\n");
  charged = 1;
  call_out("pre_blast", 60);       // Dont hold it too long.
  return 1;
}

do_throw(string str) {
  int i;
  string *exits;
  object tp;

  tp = this_player();

  if(!str) return 0;
  if(str == "sphere") {
    if(!present(this_object(), this_player())) return 0;
    if(!charged) {
      printf("\
The sphere is not charged, and throwing it might break it.\n");
      return 1;
    }
    printf("\
You drop the sphere into the center of the room.\n");
    tell_room(environment(tp), sprintf("\
%s drops a glowing sphere.\n", tp->query_name()), ({ tp }));
    move_object(this_object(), environment(tp));
    call_out("do_blast", 5);
    return 1;
  }
  sscanf(str, "sphere %s", str);
  if(!present(this_object(), tp)) return 0;
  exits = (string *)environment(tp)->query_open_exits();
  for(i = 0; i < sizeof(exits); i++) {
    if(str == exits[i]) {
      if(!charged) {
        printf("\
The sphere is not charged, and throwing it might break it.\n");
        return 1;
      }
      printf("\
You throw the sphere %s.\n", exits[i]);
      move_object(this_object(), exits[i-1]);
      tell_room(exits[i-1], sprintf("\
A small glowing sphere falls into the center of the room.\n"));
      call_out("do_blast", 5);
      return 1;
    }
  }
  printf("You can't throw it in that direction.\n");
  return 1;
}

pre_blast() {
  printf("%sThe sphere begins to throb.%s\n", MAGENTA_F, NORM);
  call_out("do_blast", 10);
}

do_blast() {

  int damage;                     // Quick hack for now.

  int i;
  string *inv;
  string *exits;

  damage = this_player()->query_stellar() * 2;
  if(living(environment())) {     // Still want it to damage whole room.
    move_object(this_object(), environment(environment()));
  }
  tell_room(environment(), sprintf("\
The sphere explodes into a thousand tiny fragments!\n"));
  inv = all_inventory(environment());
  for( i = 0; i < sizeof(inv); i++ ) {
    if(living(inv[i])) {
      tell_object(inv[i], sprintf("\
%sYou are struck by the fragments of the sphere.%s\n", RED_F, NORM));
      inv[i]->hit_player(damage);
    }
  }
  exits = (string *)environment()->query_open_exits();
  for(i = 0; i < sizeof(exits); i++) {
    if(find_object(exits[i])) {
      tell_room(exits[i], sprintf("\
The ground shakes from a nearby explosion.\n"));
    }
  }
  destruct(this_object());        // Fragmentation - Later.
}

void dispel_magic() {
  tell_object(environment(), "The sphere flashes brightly and vanishes.\n");
  destruct(this_object());
}


