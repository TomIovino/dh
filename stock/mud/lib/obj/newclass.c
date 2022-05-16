/*
  Warrior   ...complete warrior.........
  Rogue     ...complete rogue...........
  Cleric    ...complete cleric..........
  Magician  ...complete mage............
  Paladin   ...half cleric/warrior......
  Bard      ...half mage/rogue..........
  Mercenary ...half warrior/rogue.......
  Druid     ...half mage/cleric.........
  Ranger    ...half rogue/cleric........
  Battlemage...half warrior/mage........
*/

#include <ansi.h>
 
        mapping classes      = ([ ]);
varargs mixed   query_class(string var);
        status  set_class(string var, mixed setting);
        status  query_class(string var);
        status  adj_class(string var, mixed setting);
        status  unset_class(string var);
        string profession;

string query_profession()  { 
        if(!profession || profession == "") profession = "adventurer";
        return profession; }
string set_profession(string x){ profession = x; return x; }

status start_class(string var){
          set_class(var,10);

       return 1;
}

status fix_class(){
                     
         int w,m,c,d,r;
         w = this_player()->query_class("warrior");
         m = this_player()->query_class("magician");
         c = this_player()->query_class("cleric");
         r = this_player()->query_class("rogue");
         d = this_player()->query_class("dragon");
        
             set_profession("adventurer");
       if(d > 50){
             set_profession("dragon"); }
       if(m > 50){
             set_profession("magician"); }
       if(w > 50){
             set_profession("warrior");  }
       if(c > 50){
             set_profession("cleric");  }
       if(r > 50){
             set_profession("rogue");  }

   return 1;
}

status set_class(string var, mixed setting) {
       classes[var] = setting;
       return 1;
}
status adj_class(string var, mixed setting) {
       classes[var] += setting;
       return 1;
}


int query_total_classes(){ 
    int x;
    x = query_class("warrior");
    x += query_class("rogue");
    x += query_class("cleric");
    x += query_class("magician");
    return x;
 }

status unset_all_classes() { classes = ([]); }
status unset_class(string var) {
    classes = m_delete(classes,var);
    return 1;
}
 
varargs mixed query_class(string var) {
    mixed ret;
    if(!var) return copy_mapping(classes);
    ret = classes[var];
    return ret;
}




