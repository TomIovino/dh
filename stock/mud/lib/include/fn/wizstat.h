#ifndef MUD_H #define MUD_H

/* status stuff */

string stat_player(object arg);
void stat_monster(object arg);
void stat_container(object arg);
void stat_object(object arg);
void stat_other(object arg);
void stat_weapon(object arg);
void stat_armour(object arg);
void stat_inventory(object ob);
void ans_stat_inventory(string ans);
static string is_valid_ac(object ob);
string is_valid_wc(object ob);
string is_valid_mon(object ob);
varargs status stat_ob_list(object *ob, int ob_index);
status next_com(string str);
status stat_info(string str);


static object *object_list;
static int command_counter;


/**************************************************************************/
/**************************************************************************/
/* status stuff */

/* general object stats */

void stat_object(object ob) {
  string stat;
  string tmp;

  stat = ""YELLOW_F;
  stat += (ob->query_name())  
       ? "Id Name: "+(string)ob->query_name()
       : "Id Name: Unknown";
  stat += (function_exists("id",ob))
       ? " "
       : "->No id() function ";
  stat += (ob->query_object_type())
       ? "Type : "+(string)ob->query_object_type()+"\n"
       : "Type : noninheritance\n"; 
  stat += (ob->short()) 
       ? "Short Name: "+(string)ob->short()+"\n"
       : "Short Name: Invisible Object\n";
  tmp = "";
  tmp  += (environment(ob))
       ? "Located: "+file_name(environment(ob))
       : "Located: None";
  tmp  += (environment(ob) && environment(ob)->short())
       ? "::"+environment(ob)->short()
       : ":: No desc.";
  stat += extract(tmp,0,79)+"\n";
  stat += (creator(ob))
       ? "Creator: "+creator(ob)
       : "";
  stat += " File: "+file_name(ob)+" ";
  stat += (ob->query_create_room())
       ? " Create Env: "+(string)ob->query_create_room()
       : " ";
  stat += (ob->query_modified()) 
       ? " Modified: "+(string)ob->query_modified()+""
       : " ";
  write(stat+"\n"NORM);
}


void stat_weapon(object ob) {
  string stat;
  stat = "";
  stat += is_valid_wc(ob)+"\n";
  stat += "Weapon Length: "+ob->query_length()+"\n";
  stat += (ob->query_wielded())
       ? "Weapon is wielded\n"
       : "";
  write(stat);
  ob->query_bonus(1);
}

void stat_armour(object ob) {
  string stat;

  stat = "";
  stat += is_valid_ac(ob)+"\n";
  stat += (ob->query_worn())
       ? "Armour is being worn\n"
       : "";
  write(stat);
  ob->query_bonus(1);
}


void stat_container(object ob) {
  string stat;

  stat = "";
  stat += "Object Type: Container\n";
  stat += "Carry Wt: "+(string)ob->query_weight()+"/"+
          (string)ob->query_max_weight()+"\n";
  stat += (ob->query_value())
       ? "Value: "+(string)ob->query_value()+" coins"
       : "Value: None!";
  stat += (ob->query_weight()) 
       ? ",  Weight: "+(string)ob->query_weight()+"\n"
       : ",  Weight: None!\n";
  write(stat);
}


void stat_food(object ob) {
  string stat;

  stat = "";
  stat += "Object Type: Food\n";
  stat += "Strength "+(string)ob->query_strength()+
            ",  Heal "+(string)ob->query_heal()+"\n";
  stat += (ob->query_value())
       ? "Value: "+(string)ob->query_value()+" coins"
       : "Value: None!";
  stat += (ob->query_weight()) 
       ? ",  Weight: "+(string)ob->query_weight()+"\n"
       : ",  Weight: None!\n";
  write(stat);
}

void stat_drink(object ob) {
  string stat;

  stat = "";
  stat += (ob->query_soft_drink())
       ? "Object Type: Soft Drink\n"
       : "Object Type: Alco Drink\n";
  stat += (ob->query_full())
       ? "Full  " 
       : "Empty  "; 
  stat += "Strength "+(string)ob->query_strength()+
            ",  Heal "+(string)ob->query_heal()+"\n";
  stat += (ob->query_value())
       ? "Value: "+(string)ob->query_value()+" coins"
       : "Value: None!";
  stat += (ob->query_weight()) 
       ? ",  Weight: "+(string)ob->query_weight()+"\n"
       : ",  Weight: None!\n";
  write(stat);
}

void stat_living(object ob) {
  string stat;
  stat = B_RED_F+
          "Lv: "+(string)ob->query_level()+
          ", Align: "+(string)ob->query_alignment()+
          ", Prof: "+(string)ob->query_profession()+
          ", Magic: "+(string)ob->query_magic()+
          ", Race: "+(string)ob->query_race();
  stat += "\n"+CYAN_F+
          "Hp:"+(string)ob->query_hp()+"/"+(string)ob->query_max_hp()+
          ", Sp:"+(string)ob->query_sp()+"/"+(string)ob->query_max_sp()+
          ", Mp:"+(string)ob->query_mp()+"/"+(string)ob->query_max_mp()+
          ", Cp:"+(string)ob->query_cp()+"/"+(string)ob->query_max_cp()+
          ", WcR:"+(string)ob->query_wc()+
          ", WcL:"+(string)ob->query_left_wc()+
          ", Ac:"+(string)ob->query_ac();
  stat += "\n"+GREEN_F+
          "STR:"+(string)ob->query_stat("strength")+
          ", DEX:"+(string)ob->query_stat("dexterity")+
          ", STA:"+(string)ob->query_stat("stamina")+
          ", AGI:"+(string)ob->query_stat("agility")+
          ", WIS:"+(string)ob->query_stat("wisdom")+
          ", INT:"+(string)ob->query_stat("intellect")+
          ", FOC:"+(string)ob->query_stat("focus")+
          ", PRI:"+(string)ob->query_stat("primal");
  stat += "\n"+YELLOW_F+
          "Position: "+(string)ob->query_position()+
          ", Tactic: "+(string)ob->query_tactic()+
          ", Visibility: "+(string)ob->query_visibility();
  write(stat+"\n"+NORM);
}


void stat_other(object ob) {
  string stat;
  stat = "";
  stat += (ob->query_object_type())
       ? "Type: "+ ((string)ob->query_object_type()) + "\n"
       : "Type: <non inheritance>\n";
  stat += (ob->query_value())
       ? "Value: "+(string)ob->query_value()+" coins"
       : "Value: None!";
  stat += (ob->query_weight())
       ? ",  Weight: "+(string)ob->query_weight()+"\n"
       : ",  Weight: None!\n";
  stat += (ob->query_data("donater"))
       ? ",  Donated: "+(string)ob->query_data("donater")+"\n"
       : "";
  stat += (ob->query_data("enchanter"))
       ? ",  Enchanter: "+(string)ob->query_data("enchanter")+"\n"
       : "";

  stat += (ob->query_data("sanctifier"))
       ? ",  Sanctifier: "+(string)ob->query_data("sanctifier")+"\n"
       : "";

  write(stat);
}


#define INV_NUM 4
static int stat_num;
static object stat_ob;



void ans_stat_inventory(string ans) {
  if(ans == "n") {
    stat_num = 0;
    stat_ob = 0;
  }
  else {
    stat_num += (stat_num) ? INV_NUM : 3;
    stat_inventory(stat_ob);
  }
}


void stat_inventory(object ob) {
  int i;
  string stat;
  object *inv;

  stat_ob = ob;
  inv = all_inventory(ob);
  stat = "";

  if(sizeof(inv)){
    int block_size;
    stat += WHITE_F;
    stat += "<Inventory of "+((ob->query_name()) ? (string)ob->query_name()
                                                 : file_name(ob))+">:\n";
    block_size = (stat_num) ? INV_NUM : 3;
    for(i = stat_num; i < sizeof(inv) && i < stat_num + block_size; i++){
      stat += (inv[i]->short()) 
           ? "    Short Name: "+(string)inv[i]->short()+"->\n"
           : "    Short Name: Invisible Object->\n";
      stat += (inv[i]->query_name())
           ? "    Id Name: "+(string)inv[i]->query_name()+"->"
           : "    Id Name: Unknown Id Name->";
      stat += "Id: "+file_name(inv[i])+"\n";
      if(!living(inv[i])) {
       if(inv[i]->armour_class())
          stat += "    Status: "+is_valid_ac(inv[i])+"\n";
       if(inv[i]->weapon_class())
          stat += "    Status: "+is_valid_wc(inv[i])+"\n";
       if(!inv[i]->armour_class() && !inv[i]->weapon_class()) {
           stat += (inv[i]->query_object_type())
                ? "    Status: "+ ((string)inv[i]->query_object_type())
                : "    Status: <non inheritance>";
           stat += " (W: " + inv[i]->query_weight() + ", V: " + inv[i]->query_value() + ")\n"; 
       }
      }
      else{
        if(!inv[i]->query_npc()){
          stat += "    Level: "+inv[i]->query_level()+
                  ", Hp: "+inv[i]->query_hp();
          stat += (inv[i]->query_security_level())
               ? "    Player Status: Wizard"
               : "    Player Status: Player";
          stat += ", Wc: "+(string)inv[i]->query_wc();
          stat += ", LWc: "+(string)inv[i]->query_left_wc();
          stat += ", Ac: "+(string)inv[i]->query_ac();
          stat += ", Carry Wt: "+(string)inv[i]->query_weight();
          stat += ", Money: "+(string)inv[i]->query_money()+" coins\n"; 
        }
        else{
          stat += "    Status: "+is_valid_mon(inv[i])+"\n";
        }
      }
      stat += "\n";
    }
    write(stat);
    if(i < sizeof(inv)) {
      write("Continue Inventory (y/n) [y]: ");
      input_to("ans_stat_inventory");
    }    

    else {
      stat_num = 0;
      stat_ob = 0;
    }
  }
  else{
    write("<Inventory>: None.\n");
  }
}



static string is_valid_ac(object ob){
  int val, ac, weight, bonus, ac_val, error;
  string *types, type;
  int *tmp, i, eflag;
  int aflag;
  string bad;
  mixed *ac_values;

  if(!(ob || ob->armour_class())) return 0;


    ac_values = ({ });
    ac = (int)ob->armour_class() ;
    weight = (int)ob->query_weight() ;
    val = (int)ob->query_value();
    bonus = (int)ob->query_bonus();
    bad = "Armour ("; 
    bad += "AC: "+(ac); 
    bad += ", B: "+(bonus);
    bad += ", W: "+(weight+0);
    bad += ", V: "+val;
    type = (string)ob->query_type();
    bad += ", type: "+type;
    bad += ")"; 
    return bad;
}



string is_valid_wc(object ob){
  string type;
  int val, wc, weight, wc_val,length, error;
  int *tmp, i, eflag, bonus;
  int aflag;
  string bad;
  mixed *wc_values;

  if(!(ob || ob->weapon_class())) return 0;


   wc_values = ({  });
   wc = (int)ob->weapon_class() ;
   bonus = (int)ob->query_bonus();
   length = (int)ob->query_length();
   weight = (int)ob->query_weight() ;
   val = (int)ob->query_value();
   type = (string)ob->query_type();
    bad = "Weapon (";
    bad += "WC: "+(wc);
    bad += ", B: "+(bonus);
    bad += ", W: "+(weight);
    bad += ", L: "+(length);
    bad += ", V: "+val;
    bad += ", type: "+type+")";
    return bad;
}


string is_valid_mon(object ob){
  return "";
}


/* stat list */

status next_com(string str) {
  write("\n");
  if(str == "n") {
    command_counter = 0;
    object_list = ({});
    return 1;
  }
  command_counter += 1;
  stat_ob_list(object_list, command_counter);
  return 1;
}
 


status stat_ob_list(object *ob, int ob_index) {

  command_counter = ob_index;

  if(!ob[ob_index]) return 1;
  stat_object(ob[ob_index]);
  if(living(ob[ob_index])) {
    stat_living(ob[ob_index]);
  }
  else if(ob[ob_index]->query_alco_drink() 
       || ob[ob_index]->query_soft_drink()) {
    stat_drink(ob[ob_index]);
  }
  else if(ob[ob_index]->weapon_class()) {
    stat_weapon(ob[ob_index]);
  }
  else if(ob[ob_index]->armour_class()) {
    stat_armour(ob[ob_index]);
  }
  else if(ob[ob_index]->can_put_and_get()) {
    stat_container(ob[ob_index]);
  }
  else {
    stat_other(ob[ob_index]);
  }
  stat_inventory(ob[ob_index]);

  if(sizeof(ob) > 1 && command_counter + 1 < sizeof(ob)) {
    write("Next Object (y/n) [y]: ");
    input_to("next_com");
  }
  else {
    command_counter = 0;
    object_list = ({});
  }
  return 1;
}




status stat_info(string str){
  object *ob;

  if(!str) {
    write("Usage: stat <object>\n\n"+
          "Notes: This shows important info contained by various objects\n"+
          "       It checks to see if the objects have valid values.\n");
    return 1;
  }
  if(!sizeof((ob = PROCESS_ARG(str)))) return 1;
  object_list = ob;
  stat_ob_list(ob,0);
  return 1;
}


/* end status */
#endif /* MUD_H */ 

