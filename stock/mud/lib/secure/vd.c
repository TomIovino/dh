// valid_d by Ash@dragonheart used to keep track of what is and what isnt.

#include <valid.cfg> // mappings of valid groups
#include <ansi.h>



string query_string_rating(int me, int max){
    int percent;
    string doh;
    percent = 100 -( 100 * me / (max));
    switch(percent){
        case      0:  doh =  NORM       "(nonexistant)  ";       break;
        case   1..4:  doh =  WHITE_F    "(bad)          ";       break;
        case   5..9:  doh =  YELLOW_F   "(poor)         ";       break;
        case 10..19:  doh =  BROWN_F    "(decent)       ";       break;
        case 20..29:  doh =  RED_F      "(average)      ";       break;
        case 30..39:  doh =  CYAN_F     "(good)         ";       break;
        case 40..49:  doh =  B_BLUE_F   "(great)        ";       break;
        case 50..59:  doh =  MAGENTA_F  "(excellent)    ";       break;
        case 60..69:  doh =  B_MAGENTA_F"(incredible)   ";       break;
        case 70..79: doh =   GREEN_F    "(amazing)      ";       break;
        case 80..89: doh =   GREEN_F    "(superb)       ";       break;
        case 90..99: doh =   B_GREEN_F  "(*DIVINE*)     ";       break;
        case    100: doh =   B_GREEN_F  "(*GODLY*)      ";       break;
        default:     doh =   NORM       "(improbable)   ";       break;
     }

     return doh;
}




varargs mixed is_valid_kingdom(string var) {
    mixed ret;
    if(!var) return copy_mapping(valid_kingdom);
    ret = valid_kingdom[var];
    return ret;
}
varargs mixed is_valid_race(string var) {
    mixed ret;
    if(!var) return copy_mapping(valid_race);
    ret = valid_race[var];
    return ret;
}
varargs mixed is_valid_alignment(string var) {
    mixed ret;
    if(!var) return copy_mapping(valid_alignment);
    ret = valid_alignment[var];
    return ret;
}
varargs mixed is_valid_profession(string var) {
    mixed ret;
    if(!var) return copy_mapping(valid_profession);
    ret = valid_profession[var];
    return ret;
}
varargs mixed is_valid_skill(string var) {
    mixed ret;
    if(!var) return copy_mapping(valid_skill);
    ret = valid_skill[var];
    return ret;
}
varargs mixed is_valid_spell(string var) {
    mixed ret;
    if(!var) return copy_mapping(valid_spell);
    ret = valid_spell[var];
    return ret;
}
varargs mixed test_site_wiz(string var) {
    mixed ret;
    if(!var) return copy_mapping(test_site_wiz);
    ret = test_site_wiz[var];
    return ret;
}
varargs mixed live_site_wiz(string var) {
    mixed ret;
    if(!var) return copy_mapping(live_site_wiz);
    ret = live_site_wiz[var];
    return ret;
}

