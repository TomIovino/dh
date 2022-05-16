// cert'ed april 1

#include <ansi_filter.h> 

status ansi_on();
status defined_env_var(string var);

string color_string(string str) {
       return str;
}


void catch_tell(string msg) {
    object to;
 
    if(!msg || !stringp(msg)) return;
    if(!(to = this_object()) || !interactive(to)) return;

    if(defined_env_var("NO_BLACK")) {
        msg = subst(msg,B_BLACK_F,B_BLUE_F);
    }
    if(defined_env_var("NO_BLACK")) {
        msg = subst(msg,B_BLACK_F,B_BLUE_F);
    }
    if(!defined_env_var("ANSI")) {
        msg = filter_ansi(msg);
    }

    efun::tell_object(this_object(),msg);
    
}
 
void write(mixed msg) {
     if(!msg) msg = "";
     if(pointerp(msg)) msg = "<ARRAY>";
     if(mappingp(msg)) msg = "<MAPPING>";
     msg = to_string(msg);

     this_player() ? this_player()->catch_tell(msg) : catch_tell(msg);
}
 
varargs void printf(string s, mixed a, mixed b, mixed c, mixed d, mixed e,
                    mixed f, mixed g, mixed h, mixed i, mixed j,
                    mixed k, mixed l, mixed m, mixed n, mixed o) {
    
     string msg;
     msg = sprintf(s,a,b,c,d,e,f,g,h,i,j,k,l,m,o);

     this_player() ? this_player()->catch_tell(msg) : catch_tell(msg);
}

 

