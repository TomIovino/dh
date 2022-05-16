#ifndef _PAGE_H_
#define _PAGE_H_
 
#include <mudlib.h>

#undef PAGER
#define PAGER "obj/pager2"
 
varargs
void page(string data, string header) {
    object pager;
    return move_object(
                       (pager=clone_object(PAGER)),
                       this_player()
                      ),
            pager->more_text(explode(data,"\n"),header ? header : "");
}
 
#endif
