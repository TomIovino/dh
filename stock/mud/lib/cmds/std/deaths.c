#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
#define TP this_player()
status main(string str) {

printf("\n\
%s\n",
PRINTBAR,
NORM);
write(BOLD+RED_F);
tail("/log/DEATHS.dat");
write(NORM);

printf("\
%s\n\
%s\n",
PRINTBAR,
NORM);
  return 1;
}
