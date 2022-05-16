#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;
 
int query_wc(){ return 1; }
reset (arg){
    if(arg) return;
    set_type("amulet");
    set_ac(5);
    set_weight(5);
    set_value(888);
    set_name("pendant");
    set_alias("compass");
    set_short(B_RED_F+"eight gem compass pendant"+NORM);
    set_long(RED_F+"An eight gem compass pendant.\n"+NORM);

}

query_value(){ return 888; }

long(string wiz){
  string res;
  string we;
  string ns;
  string cr;
  string env;

     write("This compass has eight gems, one for each element, and\n");
     write("a center gem which stays in the middle, when you hold\n");
     write("it out in a level fashion the gems roll around.\n\n");     

     cr = this_player()->query_current_room();

      if(sscanf(cr, "d/world/%s/%s", we,ns) != 2){
         write("You cannot seem to get a good reading\n");
         return 1;
      }

     res = NORM;

   switch(we){
     case "a": case "b": case "c": case "d": case "e":   
       res += "far west";
       break;
     case "f": case "g": case "h": case "i": case "j":   
       res += "a bit west";
       break;
     case "k": case "l":
      res += "just west";
       break;
     case "m": default :
      res += "neither west nor east";
       break;
     case "n": case "o":   
      res += "just east";
       break;
     case "p": case "q": case "r": case "s": case "t":   
       res += "a bit east";
       break;
     case "u": case "v": case "w": case "x": case "y":   
      res += "far to the east";
    }
                
      res += " and ";

   switch(ns){
     case "1": case "2": case "3": case "4": case "5":   
       res += "far north";
       break;
     case "6": case "7": case "8": case "9": case "10":   
       res += "a bit north";
       break;
     case "11": case "12":
       res += "just north";
        break;
    case "13":  default :
      res += "neither north nor south";
       break;
     case "14": case "15":   
      res += "just south";
       break;
     case "16": case "17": case "18": case "19": case "20":   
       res += "a bit south";
       break;
     case "21": case "22": case "23": case "24": case "25":   
      res += "far to the south";
       break;
    }
   write(GREEN_F+"You hold your compass level to get a good reading.\n");
   write(RED_F+"The gems flicker with color.\n");
//   write(YELLOW_F+"You are the realm of "+capitalize(env)+".\n");
   write(B_WHITE_F+"Somewhere "+res+" of its center\n"+NORM);
   return 1;
}

