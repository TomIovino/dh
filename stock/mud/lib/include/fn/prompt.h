
#include <ansi.h>
#ifndef PROMPT_H
#define PROMPT_H

#define TP this_object()
#define PROMPT(X)  (prompt==X)
#define HPMETER    1

int prompt;
string prompt_str;

string conv_p_str(string x);
string print_prompt() {
    string ret, mesg;
    int hp, max;
    switch(prompt) {
        case 1:
            hp = (int)this_object()->query_hp();
            max = (int)this_object()->query_max_hp();
 
      switch(100*hp/max) {
   
                case 0..10:
                    ret = BLINK+BOLD+RED_F;
                    break;
                case 11..24:
                    ret = RED_F;
                    break;
                case 25..49:
                    ret = YELLOW_F;
                    break;
                 case 50..75:
                    ret = BOLD+GREEN_F;
                     break;
                default:
                    ret = BOLD+BLUE_F;
                    break;
            }
            if(!prompt_str) {
              mesg = sprintf(
                NORM GREEN_F"\n>"NORM);
              return (mesg);
            }
            else {
              mesg = sprintf(
   NORM GREEN_F"%s %s" NORM "%s\n>" NORM,ret,conv_p_str(prompt_str),NORM);
              return (mesg);
            }
            break;
        case 0:
        default:
            return "\n>";
            break;
    }
    return "\n>";    
}
 

string conv_p_str(string msg) {
  string rest;
  object enemy;
  while(sscanf(msg,"%s$s%s", msg, rest)) msg +=  TP->query_sp() + rest;
  while(sscanf(msg,"%s$S%s", msg, rest)) msg +=  TP->query_max_sp()+rest;
  while(sscanf(msg,"%s$h%s", msg, rest)) msg +=  TP->query_hp() + rest;
  while(sscanf(msg,"%s$H%s", msg, rest)) msg +=  TP->query_max_hp() + rest;
  while(sscanf(msg,"%s$m%s", msg, rest)) msg +=  TP->query_mp() + rest;
  while(sscanf(msg,"%s$M%s", msg, rest)) msg +=  TP->query_max_mp() + rest;
  while(sscanf(msg,"%s$c%s", msg, rest)) msg +=  TP->query_cp() + rest;
  while(sscanf(msg,"%s$C%s", msg, rest)) msg +=  TP->query_max_cp()+rest;
  while(sscanf(msg,"%s$x%s", msg, rest)) msg +=  
                                          TP->query_earned_percent() + rest;
  while(sscanf(msg, "%s$e%s", msg, rest))
  if(enemy = TP->query_attack()) msg += " EC:"
                             +(string)enemy->query_short_condition() + rest;
  return msg;
}

status set_prompt(string arg) {
  string rest, remove;
  if(!arg) {
    if(!prompt_str) {
      printf("You have no recorded prompt string. Redo your prompt to get one.\n");
      return 1;
    }
    printf("You prompt string is: %s.\n", prompt_str);
    return 1;
  }
  if(arg == "off") {
    prompt = 0;
    return printf("Prompt reset to '> '.\n"),1;
  }
  if(arg != "hp") {
   prompt = 1;
  if(sscanf(arg,"-a %s", rest)) {
    prompt_str += rest;
    return 1;
  }
  if(sscanf(arg,"-r %s", remove)) {
    if(sscanf(prompt_str, "%s"+remove+"%s",prompt_str,rest)) {
      prompt_str += rest;
      return 1;
    }
    return 1;
  }
  prompt_str = arg;
  }
  else {
    prompt = 1;  // hit point display   
    prompt_str = 0;
  }
  return 1;
}

void init_prompt() {
   efun::set_prompt(#'print_prompt,this_object());
}
 
#endif
