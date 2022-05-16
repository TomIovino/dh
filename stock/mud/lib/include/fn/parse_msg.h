#ifndef PARSE_MESSAGE_H
#define PARSE_MESSAGE_H

#include <color_conv.h>

string parse_msg(string msg, object ob) {
  string rest, sh, color, color_msg; 

  sh = (ob->query_name())
     ? (string)ob->query_name()
     : (ob->short())
     ? (string)ob->short()
     : "an invisible object";  
  while(sscanf(msg,"%s$mixed(%s)%s", msg, color_msg, rest))
    msg += colorize(color_msg)+ NORM + rest;  
  while(sscanf(msg,"%s$obj%s", msg, rest))  msg += sh + rest;  
  while(sscanf(msg,"%s$color(%s,%s)%s", msg, color, color_msg, rest))
    msg += color_conv(color)+ color_msg + NORM + rest;  
  while(sscanf(msg,"%s$name%s", msg, rest)) msg += query_name() + rest;  
  return process_msg(msg);  
}

string parse_color(string msg) {
  string rest, color, color_msg; 

  while(sscanf(msg,"%s$mixed(%s)%s", msg, color_msg, rest))
    msg += colorize(color_msg)+ NORM + rest;  
  while(sscanf(msg,"%s$color(%s,%s)%s", msg, color, color_msg, rest))
     msg + color_conv(color) + color_msg + NORM + rest;
  while(sscanf(msg,"%s$name%s", msg, rest)) msg += query_name() + rest;  
  return process_msg(msg);  
}

#endif

