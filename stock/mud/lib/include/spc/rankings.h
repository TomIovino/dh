//////////
// rankings.h -- Interface to rank_d for player object Header
//////////                                                 //////////
// Copyright (c) 1997,1998 Andrew T. Graham -- All Rights Reserved //
//////////                                                 //////////
// $ Id: $
//////////
// ChangeLog:
//   $Log: rankings.h,v $
//   Revision 1.1  1998/03/03 23:58:32  atgraham
//   Initial revision
//
//////////

#ifndef __RANKINGS_H__
#define __RANKINGS_H__

#include <rank_d.h>

#define RANKINGS "/obj/rankings"
#define PLAYERD "/secure/player_d"

varargs void update_player_ranking(int del);
int player_main_ranking();
int player_alignment_ranking();
int player_race_ranking();
int spouse_main_ranking();
int spouse_alignment_ranking();
int spouse_race_ranking();
void clear_rank_cache();

#endif

