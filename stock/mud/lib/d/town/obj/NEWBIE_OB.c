// object replacer (ash)
// this is easy code to mess up make sure you know how to use it.
// before you do..


#define PATH  "/d/town/obj/"
#define CLONE    ({ "helm","boots","gloves","sleeves","pants",\
                  "shield","armour","belt","amulet","ring",\
                  "cloak","floating","slash","thrust",\
                  "cleave","crush","wrists","ankles", })

init(){
  int c;  
  c            = random(sizeof(CLONE));
  move_object(clone_object(PATH+CLONE[c]),this_player());
  command("wear all");
  destruct(this_object());
}
