// ok'ed april 1 

#ifndef TELL_OBJECT_H
#define TELL_OBJECT_H

void tell_object(object o, string s) {
    if(o && s && objectp(o) && stringp(s)) {
        if(o->is_player())
            return (void) o->catch_tell(s);
        if(o->is_npc()) return (void)"";
        
    }

    return (void) efun::tell_object(o,s);

}

#endif

