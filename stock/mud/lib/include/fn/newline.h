#ifndef NEWLINE_H
#define NEWLINE_H

#include <ansi.h>
string add_newline(string str) {
  string lchar;
  int normed;

  if(!str) return "\n";

  if(strlen(str) > 2 && str[<strlen(NORM)..] == NORM) {
      str = str[0..<3];
      normed = 1;
  }  
  lchar = extract(str,strlen(str)-1);
  str = (lchar == "!" || lchar == "." || lchar == "?")
     ? str + "\n"
     : (lchar == "\n")
     ? str
     : str + ".\n";
  return normed ? str + NORM : str;
}


#endif
