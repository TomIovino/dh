//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Soul -- Actually and Interface to the Emote_D                ////
//// By Geewhiz                                                   ////
//// This file is part of the Emote_D package and falls under the ////
////     same restrictions found in emote_d.c                     ////
////                                                              ////
//// Copyright (c) 1996 Andrew T. Graham -- All Rights Reserved   ////
//// Licensed for Use on Muds Licensed to Geewhiz's Emote_D       ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// note this file is used by NPC's to emote socials. -Ash.
 
// This file is intended to be both an inherit for the living inherit
// (or player and/or monster inherits)  and also as an object to be cloned.
//
// If you are using this as an inherit file:
//        inherit "/path/soul";
//    In the section that adds the standard commands via add_actions()'s:
//        soul::add_feelings();
// If you are using this as a standalone object in compat:
//        move_object(clone_object("/path/soul"),the_living_object);
//    Or in native mode:
//        clone_object("/path/soul")->move(the_living_object);
 
 
//
// The EMOTE_D #define must point to the correct file
//
 
#define EMOTE_D "/secure/soul/emote_d"
 

#if 0

status id(string s) { return s == "soul"; }
status get() { return 1; }
status drop() {
    destruct(this_object());
    return 1;
}
 
void time_out() { call_out("DestSoul",4); }
void DestSoul() { destruct(this_object()); }

 
void init() {
    printf("A soul enters your body.\n");
    remove_call_out("DestSoul");
    add_action("Emote_Hook","",1);
}

#endif /* !SOUL_INHERIT */
 
void add_feelings() {
    add_action("Emote_Hook","",1);
}
 
status Emote_Hook(string arg) {
    status ret;
    catch(ret = (status) EMOTE_D->emote_hook(arg));
    return ret;
}

