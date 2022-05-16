//////////
// eqstate.h -- Handles Eq state save/restore
//////////                                                 //////////
// Copyright (c) 1997,1998 Andrew T. Graham -- All Rights Reserved //
//////////                                                 //////////
// $ Id: $
//////////
// ChangeLog:
//   $Log: eqstate.h,v $
//   Revision 1.1  1998/03/03 23:58:31  atgraham
//   Initial revision
//
//////////

#ifndef _EQSTATE_H_
#define _EQSTATE_H_

#define EQSTATE "/obj/eqstate"

void save_eq();
void restore_eq();
void force_save_me();
void restore_one_armor(string what);
void restore_eq_armor(mapping EQ_save);
void restore_eq_weapon(mapping EQ_save);
void restore_eq_containers(mapping EQ_save);

#endif
