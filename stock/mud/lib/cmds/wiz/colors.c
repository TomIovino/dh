#include <ansi.h>
 
string get_col(int i) {
    string ret;
    switch(i) {
        case 30: ret = "Black Foreground  "; break;
        case 31: ret = "Red Foreground    "; break;
        case 32: ret = "Green Foreground  "; break;
        case 33: ret = "Yellow Foreground "; break;
        case 34: ret = "Blue Foreground   "; break;
        case 35: ret = "Magenta Foreground"; break;
        case 36: ret = "Cyan Foreground   "; break;
        case 37: ret = "White Foreground  "; break;
        case 40: ret = "Black Background  "; break;
        case 41: ret = "Red Background    "; break;
        case 42: ret = "Green Background  "; break;
        case 43: ret = "Yellow Background "; break;
        case 44: ret = "Blue Background   "; break;
        case 45: ret = "Magenta Background"; break;
        case 46: ret = "Cyan Background   "; break;
        case 47: ret = "White Background  "; break;
    }
    return ret;
}
 
void page(string data);
 
string get_attrib(int a) {
    string ret;
    switch(a) {
       case 0: ret = "All Attributes Off"; break;
       case 1: ret = "High Intensity"; break;
       case 2: ret = "Normal"; break;
       case 4: ret = "Underline"; break;
       case 5: ret = "Blinking"; break;
       case 7: ret = "Reverse Video"; break;
       default:ret = "Unknown Attribute"; break;
   }
   return ret;
}
 
 
string make_swatch(int f, int b) {
    int a, *attribs, i;
    string ret;
    attribs = ({ 0, 1, 2, 4, 5, 7 });
    ret = "";
    for(i=0;i < 6; i++) {
      ret += sprintf("%c[0m%c[%d;%d;%dmThis is %s %s on a %s\n",
                    27,27,attribs[i],f,b,get_attrib(attribs[i]),get_col(f),get_col(b));
    }
   return ret;
}
 
status main() {
   int i,j;
   string ret; 
   ret = "\n\n\tColor Swatch Generator\n";
   ret += " Color   Ansi Code\tAnsi Code       Color\n";
   for(i=40; i < 48; i++) {
       for(j=30; j < 38; j++) {
             ret += make_swatch(i,j);
       }
   }
   ret += NORM;
   page(ret+"\n\n");
   return 1;
}
 
// Some defines for the BIM pager.
#define PG_BUFF           "BFF"         // The edit buffer.
#define PG_Q_FUNC         "QTF"         // The quit-function.
#define PG_LENG           "LNG"         // Page length.
 
void page(string data) {
    int lines;
    string *d;
    d = explode(data,"\n");
    lines = 20;
    return (void) "/secure/bim/pager"->start_pager(
                                   ([
                                      PG_BUFF  : d,
                                      PG_Q_FUNC: "quit_pager",
                                      PG_LENG : lines,
                                   ]));
}
 
void quit_pager(mapping note) {
    return;
}
