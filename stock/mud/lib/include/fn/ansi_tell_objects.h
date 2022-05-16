// certified april 1 99

#ifndef TELL_H
#define TELL_H

#include <ansi_tell_object.h>

/***************************************************************************/
/* tell_objects */

varargs void tell_objects(object *who, string str, object *reject) { 
  int i; 

  if(!reject) reject = ({});
  who -= reject;
  for (i = 0; i < sizeof(who); i++) { 
    if(who[i] != this_player()) { 
       tell_object(who[i], str); 
    }
  } 
} 
   
#endif /* end TELL_H */
