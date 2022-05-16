/* scrying eye controller */

object eye;

void init() {
  add_action("dispel", "dispel");
  add_action("eye","eye");
}

status eye(string str) {
  if(!eye) {
    destruct(this_object());
    return 0;
  }
  if(str == "dispel"){ this_object()->dispel("eye"); return 1;}
  if(str == "n") str = "north";
  if(str == "s") str = "south";
  if(str == "w") str = "west";
  if(str == "e") str = "east";
  if(str == "sw") str = "southwest";
  if(str == "nw") str = "northwest";
  if(str == "ne") str = "northeast";
  if(str == "se") str = "southeast";
  eye->new_room(str);
  return 1;
}


status dispel(string str) {
  if(str != "eye") return 0;
  if(!eye) {
    destruct(this_object());
    return 0;
  }
  eye->time_out();
  return 1;
}

void set_wizard_eye(object ob) { eye = ob;   }
object query_wizard_eye()      { return eye; }

status id(string str) {
  return str == "spell"
      || str == "eye controller" || str == "wizard eye";
}

