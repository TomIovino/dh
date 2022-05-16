#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

int f,a,e,w;
int open;

void reset(status arg){
    if(arg) return;

    set_name("claws");
    set_short(B_WHITE_F+"glowing "+B_RED_F+"cr"+YELLOW_F+"ys"B_BLUE_F+
             "ta"+B_WHITE_F+"l c"+B_RED_F+"la"+YELLOW_F+"ws"+NORM);
    set_alias("claw");
    set_long(RED_F+
  "A beautiful crystaline gauntlet with four crystal claws.\n"+
  "Each claw has a socketed gem in it, which shines with energy.\n");
    set_wc(15);
    set_type("tear");
    set_length(10);  
    set_weight(10);
   set_light(2);
    set_value(2000);
    set_hit_func(this_object());
}

int weapon_hit(object attacker) {
        int r;
        r = random(100);
        if(r < 5){
  write(B_WHITE_F+"Air stuns "+attacker->query_name()+"!\n"+NORM);
  say(B_WHITE_F+"Air stuns "+attacker->query_name()+"!\n"+NORM);
        return 5;
        }
        if(r < 10){
  write(B_CYAN_F+"Ice freezes "+attacker->query_name()+"!\n"+NORM);
  say(B_CYAN_F+"Ice freezes "+attacker->query_name()+"!\n"+NORM);
        return 5;
        }
        if(r < 15){
  write(B_BROWN_F+"Earth crushes "+attacker->query_name()+"!\n"+NORM);
  say(B_BROWN_F+"Earth crushes "+attacker->query_name()+"!\n"+NORM);
        return 5;
        }
        if(r < 20){
  write(B_RED_F+"Fire burns "+attacker->query_name()+"!\n"+NORM);
  say(B_RED_F+"Fire burns "+attacker->query_name()+"!\n"+NORM);
        return 5;
        }
  return 0;
}

