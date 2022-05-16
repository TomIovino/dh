 // Ansi.c
 // 21/Dec/93. Bubbs.
// used in mail and pager

object find_player(string str);
mapping codes;

static private void check_codes() {
  if (codes) return;
  codes = ([ ]);
/*
     "home"     : sprintf("%c[H",       27); 0,
     "save"     : sprintf("%c7",        27); 0,
     "restore"  : sprintf("%c8",        27); 0,
     "standard" : sprintf("%c[2;37;0m", 27); 1,
     "normal"   : sprintf("%c[0m",      27); 1,
     "bold"     : sprintf("%c[1m",      27); 1,
     "highlight": sprintf("%c[1m",      27); 1,
     "underline": sprintf("%c[4m",      27); 1,
     "flash"    : sprintf("%c[5m",      27); 1,
     "inverse"  : sprintf("%c[7m",      27); 1,
     "black"    : sprintf("%c[30m",     27); 1,
     "red"      : sprintf("%c[31m",     27); 1,
     "green"    : sprintf("%c[32m",     27); 1,
     "yellow"   : sprintf("%c[1;33m",   27); 1,
     "brown"    : sprintf("%c[33m",     27); 1,
     "blue"     : sprintf("%c[34m",     27); 1,
     "magenta"  : sprintf("%c[35m",     27); 1,
     "cyan"     : sprintf("%c[36m",     27); 1,
     "white"    : sprintf("%c[37m",     27); 1,
   ]);

*/
  }

varargs string cursor_up(int number, object target) {
  check_codes();
  if (!target) target = this_player();
  return ((status)target->ansi_on() ?
   sprintf("%c[%dA", 27, number) : "");
  }

varargs string cursor_down(int number, object target) {
  check_codes();
  if (!target) target = this_player();
  return ((status)target->ansi_on() ?
   sprintf("%c[%dB", 27, number) : "");
  }

varargs string cursor_right(int number, object target) {
  check_codes();
  if (!target) target = this_player();
  return ((status)target->ansi_on() ?
   sprintf("%c[%dC", 27, number) : "");
  }

varargs string cursor_left(int number, object target) {
  check_codes();
  if (!target) target = this_player();
  return ((status)target->ansi_on() ?
   sprintf("%c[%dD", 27, number) : "");
  }

varargs string cursor_position(int l, int c, object target) {
  check_codes();
  if (!target) target = this_player();
  return ((status)target->ansi_on() ?
   sprintf("%c[%d;%dH", 27, l, c) : "");
  }

varargs string erase_line(int mode, object target) {
  check_codes();
  if (!target) target = this_player();
  return ((status)target->ansi_on() ?
   sprintf("%c[%dK", 27, mode) : "");
  }

varargs string erase_screen(int mode, object target) {
  check_codes();
  if (!target) target = this_player();
  return ((status)target->ansi_on() ?
   sprintf("%c[%dJ", 27, mode) : "");
  }

varargs string ansi_colour(string code, object target) {
  string colour, *cols;
  int index, size;
  if (!target) target = this_player();
  if ((status)target->ansi_on() == 0) return "";
  size = sizeof(cols = explode(code, " "));
  check_codes();
  for(index = 0, colour = ""; index < size; index++)
   if (member(codes, cols[index]))
    colour = sprintf("%s%s", colour, codes[cols[index]]);
  return colour;
  }

varargs string std_colour(mixed arg) {
  object who;
  string colour, *cols;
  int index, size;
  if (!arg)
   who = this_player();
  else if (stringp(arg))
   who = find_player(arg);
  else who = arg;
  if (!who)
   raise_error("std_colour() : Invalid argument.\n");
  if (!colour) return "";
  size = sizeof(cols = explode(colour, " "));
  check_codes();
  for(index = 0, colour = ""; index < size; index++)
   colour = sprintf("%s%s", colour,
    codes[cols[index], 1] ? codes[cols[index]] : "");
  return colour;
  }
