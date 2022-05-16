// this function handled timed liv obj varibles


#include <ansi.h>
 
varargs mixed   query_var(string var);
        mapping varibles      = ([ ]);
        status  set_var(string var, mixed setting);
        status  unset_var(string var);

status set_var(string var, mixed setting) {
       varibles[var] = setting;
       return 1;
}
status adj_var(string var, mixed setting) {
       varibles[var] += setting;
       return 1;
}


int unset_all_vars(){ varibles = ([]); }
int query_total_vars(){ return sizeof(m_indices(varibles)); }
status unset_var(string var) {
    varibles = m_delete(varibles,var);
    return 1;
}
 
varargs mixed query_var(string var) {
    mixed ret;
    if(!var) return copy_mapping(varibles);
    ret = varibles[var];
   return ret;
}


static mapping time_update_map =
    ([
        "dragonkin":
            "dragonkin_time";
            "You become normal.",
        "regenerate":
            "regenerate_time";
            "Your magical regeneration fades.",
        "sight":
            "sight_time";
            "Your sight becomes less focused.",
        "awareness":
            "awareness_time";
            "Your heightened awareness fades away.",
        "scales":
            "scales_time";
            "You lose your scales.",
        "breath":
            "breath_time";
            "Your breathing becomes less controlled.",
        "fangs":
            "fangs_time";
            "Your fangs retract into your gums.",
        "claws":
            "claws_time";
            "Your claws retract into your skin.",
        "wings":
            "wings_time";
            "Your wings fade out of existance.",
        "invisibility":
            "invisibility_time";
            "You fade into existance.",
        "awareness":
            "awareness_time";
            "Your awareness fades.",
        "bless":
            "bless_time";
            "Your blessing expires.",
        "heroism":
            "heroism_time";     
            "You feel less heroic.", 
        "manafuse":
            "manafuse_time";     
            "Your manafuse disipates.", 
        "fortitude":
            "fortitude_time";     
            "You feel less fortified.", 
        "frenzy":
            "frenzy_time";     
            "You feel less frenzied.", 
        "feedback":
            "feedback_time";     
            "You lose your feedback.", 
        "absorb":
            "absorb_time";
            "You feel less durable.",
        "avenger":
             "avenger_time";
             "Your avenger fades away.",           
        "defender":
             "defender_time";
             "Your defender fades away.",
        "barrier":
             "barrier_time";
             "Your barrier fades away.",
        "reflect":
             "reflect_time";
             "Your reflective magic fades away.",
        "deflect":
             "deflect_time";
             "Your deflective magic fades away.",
        "curse":
             "curse_time";
             "You are no longer under a curse.",
        "weaken":
             "weaken_time";
             "You are no longer weak.",
        "stun":
             "stun_time";
             "You can move again.",
        "poison":
             "poison_time";
             "Your blood has purged itself of the poison.",
        "disease":
             "disease_time";
             "Your body has purged itself of disease.",
        "blind":
             "blind_time";
             "You can see again.",
        "strength":
             "strength_time";
             "You lose some strength.",
        "intellect":
             "intellect_time";
             "You lose some intellect.",
        "wisdom":
             "wisdom_time";
             "You lose some wisdom.",
        "stamina":
             "stamina_time";
             "You lose some stamina.",
        "agility":
             "agility_time";
             "You lose some agility.",
        "dexterity":
             "dexterity_time";
             "You lose some dexterity.",
        "focus":
             "focus_time";
             "You lose some focus.",
        "primal":
             "primal_time";
             "You lose some primal.",
        "slow":
             "slow_time";
             "You dont feel so slow anymore.",
        "fast":
             "fast_time";
             "You dont feel so fast anymore.",
    ]);
            
private static 
void update_one_variable(string spell, string counter, string fade_msg) {
    if(query_var(spell)) {
        adj_var(counter,-1);
        if(query_var(counter) <= 0) {
            unset_var(spell);
            unset_var(counter);
            if(fade_msg)
                write(GREY_F + fade_msg + NORM + "\n");
        }
    } else if(query_var(counter))  // If xxx_time is set without xxx being set
        unset_var(counter);        // unset xxx_time
}
        
int update_timed() {
    // The following will make calls calls like:
    // update_one_variable("bless","bless_time","Your blessing fades.")
    // for every entry in the time_update_map above.
    walk_mapping(time_update_map,#'update_one_variable);
}


