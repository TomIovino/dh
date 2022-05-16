
// A certain type of magic is required to alter this code.
//////////
// rankings.c -- Interface to rank_d for player object
//////////                                                 //////////
// Copyright (c) 1997,1998 Andrew T. Graham -- All Rights Reserved //
//////////                                                 //////////
// $ Id: $
//////////
// ChangeLog:
//   $Log: rankings.c,v $
//   Revision 1.3  1998/10/02 04:55:59  atgraham
//   o DragonHeart Current Files
//
//   Revision 1.2  1998/03/12 23:12:05  atgraham
//   o Fixed bug where rankings were not updating
//   o Adding some debug logging that can be removed sometime in the future.
//
//   Revision 1.1  1998/03/03 23:58:32  atgraham
//   Initial revision
//
//////////

#include <rankings.h>

string query_real_name();
int query_master_exp();
int query_total_exp();
int query_level();
string query_race();
string query_alignment();
string query_house();
string query_spouse();
string query_profession();
int query_security_level();

varargs void update_player_ranking(int del) {
    string err;
    err = catch(RANK_D->update_ranking(!del ? R_PLAYER : -R_PLAYER,
                 query_real_name(),
                 query_master_exp(),
                 query_level(),
                 query_race(),
                 query_alignment(),
                 query_house(),
                 query_security_level(),
                 query_total_exp(),
                 query_profession()));
   log_file("/MAIN/RANKINGS",
   sprintf("%s update_ranking(%O,%O,%O,%O,%O,%O,%O,%O,%O,%O)\n",timestamp(),
                 !del ? R_PLAYER : -R_PLAYER,
                 query_real_name(),
                 query_master_exp(),
                 query_level(),
                 query_race(),
                 query_alignment(),
                 query_house(),
                 query_security_level(),
                 query_total_exp(),
                 query_profession()));
   log_file("/MAIN/RANKINGS",
   sprintf("%s update_ranking(%O,%O,%O,%O,%O,%O,%O,%O,%O,%O)\n",timestamp(),
                 !del ? R_PLAYER : -R_PLAYER,
                 query_real_name(),
                 query_master_exp(),
                 query_level(),
                 query_race(),
                 query_alignment(),
                 query_house(),
                 query_security_level(),
                 query_total_exp(),
                 query_profession()));
     if(err && stringp(err))
     log_file("/MAIN/RANKINGS",sprintf("%s u_p_r()\n\terr = %s\n",timestamp(),err));
}


static int m_rank = 0;

int player_main_ranking() {
    int ret;
    if(m_rank) return m_rank;
    catch(ret = (int) RANK_D->player_ranking(query_real_name(),P_MAIN));
    return m_rank = ret;
}
static int p_rank = 0;

int player_profession_ranking() {
    int ret;
    if(p_rank) return p_rank;
    catch(ret = (int)
RANK_D->player_ranking(query_real_name(),P_PROFESSION,query_profession()));
    return p_rank = ret;
}

static int a_rank = 0;

int player_alignment_ranking() {
    int ret;
    if(a_rank) return a_rank;
    catch(ret = (int) RANK_D->player_ranking(query_real_name(),P_ALIGNMENT,query_alignment()));
    return a_rank = ret;
}

static int r_rank = 0;

int player_race_ranking() {
    int ret;
    if(r_rank) return r_rank;
    catch(ret = (int) RANK_D->player_ranking(query_real_name(),P_RACE,query_race()));
    return r_rank = ret;
}    

static int s_m_rank = 0;

int spouse_main_ranking() {
    int ret;
    if(s_m_rank) return s_m_rank;
    catch(ret = (int) RANK_D->player_ranking(query_spouse(),P_MAIN));
    return s_m_rank = ret;
}

static int s_a_rank = 0;

int spouse_alignment_ranking() {
    int ret;
    string s_altitle;
    if(s_a_rank) return s_a_rank;
    catch(s_altitle = (string) PLAYERD->query_alignment(query_spouse()));
    if(!s_altitle) s_altitle = query_alignment();
    catch(ret = (int) RANK_D->player_ranking(query_spouse(),P_ALIGNMENT,s_altitle));
    return s_a_rank = ret;
}
static int s_p_rank = 0;

int spouse_profession_ranking() {
    int ret;
    string s_prof;
    if(s_p_rank) return s_p_rank;
    catch(s_prof = (string) PLAYERD->query_profession(query_spouse()));
    if(!s_prof) s_prof = query_profession();
    catch(ret = (int) 
      RANK_D->player_ranking(query_spouse(),P_PROFESSION,s_prof));
    return s_p_rank = ret;
}    

static int s_r_rank = 0;

int spouse_race_ranking() {
    int ret;
    string s_race;
    if(s_r_rank) return s_r_rank;
    catch(s_race = (string) PLAYERD->query_race(query_spouse()));
    if(!s_race) s_race = query_race();
    catch(ret = (int) RANK_D->player_ranking(query_spouse(),P_RACE,s_race));
    return s_r_rank = ret;
}    

void clear_rank_cache() {
    s_p_rank = s_r_rank = s_a_rank = a_rank = r_rank = p_rank = 0;
}

