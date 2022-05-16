#define catch(X) _catch(X)
mixed _catch(mixed x) { return 0; }
//////////////////////////////////////////////////////////////////////////////
// Banish Daemon Interface (BDI) (v0.01)
// Geewhiz@DoD, Geewhiz@DragonHeart
// Copyright (c) 1996,1998 Andrew T. Graham  All Rights Reserved
//////////
// $Id: bdi.c,v 1.1 1998/03/03 23:50:29 atgraham Exp atgraham $
//////////
// ChangeLog:
//   $Log: bdi.c,v $
//   Revision 1.1  1998/03/03 23:50:29  atgraham
//   Initial revision
//
//////////

 
#include <banish.h>
 
mapping RegistrationInfo = ([ ]);
 
 
// The callback, cb, is used to pass the next function in the logon chain
// We take care to make sure the callback is called if the banish_d can't
// be loaded or if it returns 0.
void accept_connection(closure cb) {
    mixed err;
    int ret;
 
    if(err = catch(ret = BANISH_D->accept_connection(this_object(),
                                                     RegistrationInfo,cb))
       || !ret) {
        printf("banish_d::accept_connection() = %s\treturned %d",err,ret);
        funcall(cb);
    }
}
