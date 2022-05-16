#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
#define TP this_player()

string answer(int x){ 
  if(x) return  (string)B_GREEN_F+"on "+YELLOW_F;
  if(!x) return (string)B_RED_F+"off"+YELLOW_F;
}

string *query_cmd_aliases() { return ({ "toggle", "configure" });}


status main(string blah) {
 int x;
printf("\n\%s\nCurrent Configuration :\n%s",PRINTBAR, YELLOW_F); 

if(blah == "ansi"){
      if(TP->query_env_var("ANSI")){
        write(NORM); TP->unset_env_var("ANSI");
      }
      else TP->set_env_var("ANSI",1);
}
if(blah == "tellblock"){
      if(TP->query_env_var("tell_block")) TP->unset_env_var("tell_block");
      else TP->set_env_var("tell_block","players");
}
if(blah == "autogold"){
      if(TP->query_env_var("AUTOGOLD")) TP->unset_env_var("AUTOGOLD");
      else TP->set_env_var("AUTOGOLD",1);
}
if(blah == "autobury"){
      if(TP->query_env_var("AUTOBURY")) TP->unset_env_var("AUTOBURY");
      else TP->set_env_var("AUTOBURY",1);
}
if(blah == "autoloot"){
      if(TP->query_env_var("AUTOLOOT")) TP->unset_env_var("AUTOLOOT");
      else TP->set_env_var("AUTOLOOT",1);
}
if(blah == "visit"){
      if(TP->query_env_var("VISIT")) TP->unset_env_var("VISIT");
      else TP->set_env_var("VISIT",1);
}
if(blah == "summon"){
      if(TP->query_env_var("SUMMON")) TP->unset_env_var("SUMMON");
      else TP->set_env_var("SUMMON",1);
}
if(blah == "brief"){
      if(TP->query_env_var("BRIEF")) TP->unset_env_var("BRIEF");
      else TP->set_env_var("BRIEF",1);
}
if(blah == "nogive"){
      if(TP->query_env_var("NO_GIVE")) TP->unset_env_var("NO_GIVE");
      else TP->set_env_var("NO_GIVE",1);
}
if(blah == "arena"){
      if(TP->query_env_var("ARENA")) TP->unset_env_var("ARENA");
      else TP->set_env_var("ARENA",1);
}
if(blah == "accept"){
      if(TP->query_env_var("ACCEPT")) TP->unset_env_var("ACCEPT");
      else TP->set_env_var("ACCEPT",1);
}
if(blah == "roleplay"){
      if(TP->query_env_var("ROLEPLAY")) TP->unset_env_var("ROLEPLAY");
      else TP->set_env_var("ROLEPLAY",1);
}


printf("ansi             : %s\n",answer(TP->query_env_var("ANSI")));
printf("autoloot         : %s\n",answer(TP->query_env_var("AUTOLOOT")));
printf("autogold         : %s\n",answer(TP->query_env_var("AUTOGOLD")));
printf("autobury         : %s\n",answer(TP->query_env_var("AUTOBURY")));
printf("visit            : %s\n",answer(TP->query_env_var("VISIT")));
printf("summon           : %s\n",answer(TP->query_env_var("SUMMON")));
printf("arena            : %s\n",answer(TP->query_env_var("ARENA")));
printf("accept           : %s\n",answer(TP->query_env_var("ACCEPT")));
printf("brief            : %s\n",answer(TP->query_env_var("BRIEF")));
printf("tellblock        : %s\n",answer(TP->query_env_var("tell_block")));
printf("nogive           : %s\n",answer(TP->query_env_var("NO_GIVE")));
printf("roleplay         : %s\n",answer(TP->query_env_var("ROLEPLAY")));
	
printf("\
%s\n\
%s\n",
PRINTBAR, NORM);
  return 1;
}


status help(){ 
       main("");
       write("config <what>\n");
         return 1;
     }

