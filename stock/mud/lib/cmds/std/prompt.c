main(str) {
  this_player()->set_prompt(str);
  if(str == "0" || str == "off" || str == "none")
  this_player()->set_prompt("");
  if(str == "1" || str == "standard")
  this_player()->set_prompt("H:$h S:$s M:$m $e");
  if(str == "2" || str == "advanced")
  this_player()->set_prompt("H:$h/$H S:$s/$S M:$m/$M C:$c/$C X:$x% $e");
  return 1;
}

help() {
  printf("This command lets you customize your prompt. \n"+
         "Type: prompt <whatever you want here> to set it.\n"
         "\t$h    Current Hit Points.\n"+
         "\t$H    Max Hit Points.\n"+
         "\t$s    Current Spell Points.\n"+
         "\t$S    Max Spell Points.\n"+
         "\t$m    Current Movement Points.\n"+
         "\t$M    Max Movement Points.\n"+
         "\t$c    Current Carry Points.\n"+
         "\t$C    Max Carry Points.\n"+
         "\t$x    Current Experience to level percentage.\n"+
         "\t$e    Current Enemy Condition.\n"+
         "typing, 'prompt 0' will turn off your prompt.\n"+
         "typing, 'prompt 1' will give you the standard short prompt'\n"+
         "typing, 'prompt 2' will give you the standard long prompt'\n");
       return 1;
}
