#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
#define TP this_player()

status main(string str) {
string ansi;
ansi = "";

if(str == "help" || !str){
   write("colorme <w/bw/g/bg/r/br/c/bc/m/bm/b/bb/y/by/bl>\n");
   return 1;
   }
  
if(!str) ansi =        "w";
if(str == "w") ansi =  "w";
if(str == "bw") ansi = "bw";
if(str == "b") ansi =  "b";
if(str == "bb") ansi = "bb";
if(str == "r") ansi =  "r";
if(str == "br") ansi = "br";
if(str == "g") ansi =  "g";
if(str == "bg") ansi = "bg";
if(str == "y") ansi =  "y";
if(str == "by") ansi = "by";
if(str == "m") ansi =  "m";
if(str == "bm") ansi = "bm";
if(str == "c") ansi =  "c";
if(str == "bc") ansi = "bc";
if(str == "bl") ansi = "bl";
  write(NORM+ansi+"You have set your player color successfully.\n");
  TP->set_cc(ansi);
  TP->clear_who_string();
  TP->query_static_link();
  return 1;

}
