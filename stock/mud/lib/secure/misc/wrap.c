////////////////////////////////////////////////////////////
// WRAP.C             // DRAGONHEART // GEEWHIZ 6.06.95   //
////////////////////////////////////////////////////////////

varargs string format(string str, int indent, int shift, int width) {
    int extra,pre;
    if(!str || !stringp(str))
        raise_error("format(): Bad arg 1 (need string)\n");
    if(str == "")
        return str;
    while(str[<1] == '\n')
        str = str[0..<2];
    if(!indent || indent < 0)
        indent = 0;
    if(!width || width < 0)
        width = 75;
    if(shift < 0) {
        pre = -1 * shift;
        extra = 0;
    } else if(shift == 0) {
        pre = 0;
        extra = 0;
    } else { // shift > 0
        pre = 0;
        extra = shift;
    }
    if(extra) str = sprintf("%*s",extra,"") + str;
    return sprintf("%*s%-*=s\n",indent,str[0..pre-1],width-indent,str[pre..]);
}
 
varargs string wrap(string str, int width, int indent) {
    if(!width) width = 78;
    if(!indent) indent = 0;
    return format(str,indent,(-1*indent),width);
}
