#ifndef COLOR_CONV_H
#define COLOR_CONV_H

#include <ansi.h>
 
static string *colors;
//string process_msg(msg);

string r_c() { return "["+(31 + random(8))+"m"; }
 
string colorize(string arg) {
   int i, j;
   string res;
   string *args;

  args = explode(arg, "");
  j = sizeof(args);
  res = "";
  for(i = 0;i < j; i++) {
    res += (string)r_c() + args[i];
  }
  return process_msg(res);
}
 
string color_conv(string color) {
  if(color == "green")     return GREEN_F;
  if(color == "red")       return RED_F;
  if(color == "blue")      return BLUE_F;
  if(color == "magenta")   return MAGENTA_F;
  if(color == "yellow")    return YELLOW_F;
  if(color == "grey")      return GREY_F;
  if(color == "cyan")      return CYAN_F;
  if(color == "brown")     return BROWN_F;
  else return WHITE_F;
}

string conv_color(string msg) {
  string rest, ansi_str; 

  while(sscanf(msg,"%s^[[%sm%s", msg, ansi_str,rest))  msg += rest;  
  return process_msg(msg);
}
#endif

