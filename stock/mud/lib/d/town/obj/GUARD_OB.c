
// object replacer (ash)
// this is easy code to mess up make sure you know how to use it.
// before you do..


#define PATH  "/d/town/obj/"
#define CLONE    ({ "helm","boots","gloves","sleeves","pants",\
                  "shield","armour","belt","amulet","ring",\
                  "cloak","floating","slash","thrust",\
                  "cleave","crush","wrists","ankles", })

init(){
  string name;
  int c;  
  c            = random(sizeof(CLONE));
  name = call_other(PATH+CLONE[c]+"_2","query_name");
  move_object(clone_object(PATH+CLONE[c]+"_2"),this_player());
  command("wear "+name,this_player());
  command("wield "+name,this_player());
  destruct(this_object());

 }
