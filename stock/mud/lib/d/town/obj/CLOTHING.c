// object replacer (ash)
// this is easy code to mess up make sure you know how to use it.
// before you do..


#define PATH  "/d/town/obj/clothing/"

#define CLONE    ({  \
 "cloak_blue",      "cloak_yellow",     "leg_white",    "tunic_red", \
 "cloak_cyan",      "leg_blue",         "leg_yellow",   "tunic_white", \
 "cloak_green",     "leg_cyan",         "tunic_blue",   "tunic_yellow", \
 "cloak_magenta",   "leg_green",        "tunic_cyan",   \
 "cloak_red",       "leg_magenta",      "tunic_green",  \
 "cloak_white",     "leg_red",          "tunic_magenta",})


init(){
  int c;
  c            = random(sizeof(CLONE));
  move_object(clone_object(PATH+CLONE[c]),this_player());
  command("wear all");
  destruct(this_object());
}
