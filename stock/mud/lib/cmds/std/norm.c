#include <ansi.h>
#define CLRSCR sprintf("%c[2J",27)
#define HOMECSR sprintf("%c[1;1H",27)
status main(string arg) {
    string msg;
    msg = sprintf("%s%s%sTerminal Colors Reset.\n",
                      NORM,CLRSCR,HOMECSR);
    printf(msg);
    return 1;
}
