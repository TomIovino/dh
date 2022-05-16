#include <ansi.h>
  

string cc;

string query_cc() { return cc; }

string set_cc(string str){
  string x;
 if(!str)        x = "w";
 if(str == "w")  x = "w";
 if(str == "bw") x = "bw";
 if(str == "b")  x = "b";
 if(str == "bb") x = "bb";
 if(str == "r")  x = "r";
 if(str == "br") x = "br";
 if(str == "g")  x =  "g";
 if(str == "bg") x = "bg";
 if(str == "y")  x =  "y";
 if(str == "by") x = "by";
 if(str == "m")  x =  "m";
 if(str == "bm") x = "bm";
 if(str == "c")  x =  "c";
 if(str == "bc") x = "bc";
 if(str == "bl") x = "bl";
   cc = x;
 return (string)cc;
}


string do_cc(string str){
  string x;
  if(!str) str = "";
  if(!cc)        x = WHITE_F;
  if(cc == "w")  x = WHITE_F;
  if(cc == "bw") x = B_WHITE_F;
  if(cc == "b")  x = BLUE_F;
  if(cc == "bb") x = B_BLUE_F;
  if(cc == "r")  x = RED_F;
  if(cc == "br") x = B_RED_F;
  if(cc == "g")  x = GREEN_F;
  if(cc == "bg") x = B_GREEN_F;
  if(cc == "y")  x = BROWN_F;
  if(cc == "by") x = YELLOW_F;
  if(cc == "m")  x = MAGENTA_F;
  if(cc == "bm") x = B_MAGENTA_F;
  if(cc == "c")  x = CYAN_F;
  if(cc == "bc") x = B_CYAN_F;
  if(cc == "bl") x = NORM+GREY_F;
  str = x+str;
  return str;
}

