void write_define(string file, string what, string value) {
    write_file(file,sprintf("#define %s %s\n",what,value));
}
 
string esc(string s) { return sprintf("ESC(\"%s\")",s); }
 
status main() {
   int i;
   string f; 
   printf("Generating /open/ansi_.h\n");
   f = "/open/ansi_.h";
   write_define(f,"ESC(X)",sprintf("(\"%c[\"X\"m\")",27));
   write_define(f,"NORM",esc("0"));
   write_define(f,"GREY_F", esc("1;30"));
   write_define(f,"B_BLACK_F", esc("1;30"));
   write_define(f,"B_RED_F",esc("1;31"));
   write_define(f,"B_GREEN_F",esc("1;32"));
   write_define(f,"B_BLUE_F",esc("1;34"));
   write_define(f,"B_MAGENTA_F",esc("1;35"));
   write_define(f,"B_CYAN_F",esc("1;36"));
   write_define(f,"B_BROWN_F",esc("1;33"));
   write_define(f,"B_WHITE_F",esc("1;37"));
   write_define(f,"BLACK_F",esc("30"));
   write_define(f,"RED_F",esc("31"));
   write_define(f,"GREEN_F",esc("32"));
   write_define(f,"YELLOW_F",esc("1;33"));
   write_define(f,"BLUE_F",esc("34"));
   write_define(f,"MAGENTA_F",esc("35"));
   write_define(f,"CYAN_F",esc("36"));
   write_define(f,"BROWN_F",esc("33"));
   write_define(f,"NORMAL", "write("+esc("0;40;37")+");");
   write_define(f,"NORM2",esc("0;40;37"));
   write_define(f,"BOLD",esc("1"));
   write_define(f,"FAINT",esc("2"));
   write_define(f,"BLINK",esc("5"));
   write_define(f,"REVERSE",esc("7"));
   write_define(f,"OFF",esc("0"));
   return 1;
}
