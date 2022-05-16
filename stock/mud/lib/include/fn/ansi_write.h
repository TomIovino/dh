#ifndef WRITE_H
#define WRITE_H
 
#include <ansi_filter.h>
 
void write(mixed msg) {
    object tp;
    if(!msg) msg = "";
    if(pointerp(msg)) msg = "<ARRAY>";
    if(mappingp(msg)) msg = "<UNKNOWN>";
    msg = to_string(msg);
    if((tp=this_player()) && interactive(tp) && !(tp->ansi_on())) {
       efun::write(filter_ansi(msg));
    } else {
       efun::write(msg);
    }
}
 
#endif
