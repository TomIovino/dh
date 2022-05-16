#include <mudlib.h>
inherit ROOM;

reset (arg) {
  if (arg) return;

  set_short( "Standard Workroom");
  set_long ("\
This is the standard workroom. Work on it.\n\
A small sign hangs on the wall, maybe you should look at it.\n\
  ");
  set_light(2);
  set_exits (({
             }));
set_items(({
"sign",
"Welcome to the world of immortals!\n"+
"\n"+
"Please read the files in /doc/mudlib/\n"+
"These can also be accessed by 'help <file name>\n"+
"\nEnjoy your time here!\n",
}));
}
