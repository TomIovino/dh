#ifndef ANSI_FILTER_H
#define ANSI_FILTER_H
 
#if 1
status __not_ansi_code(string s) { return s[0] != 27; }
string filter_ansi(string s) {
    string *c;
    c = regexplode(s,sprintf("%c\[[0-9;]*m",27));
    c -= ({ "" });
    c = filter_array(c,#'__not_ansi_code);
    c -= ({ "" });
    return implode(c,"");
}
#else
string filter_ansi(string s) {
    string t, e;
    string FILTER;
    if(!s || !stringp(s)) s = "";
    FILTER = sprintf("%%s%c[%%sm%%s",27);
    while(sscanf(s, FILTER, s, t, e)) s += e;
    s = implode(explode(s,sprintf("%c",27)),"ESC");
    return s;
}
#endif
 
#endif
