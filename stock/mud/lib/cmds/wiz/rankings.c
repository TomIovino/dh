/////////////////
// rankings
/////////////////

#include <rank_d.h>
#include <ansi.h>
#include <bar.h>
#include <valid.cfg>

private nomask mixed *info;

#define DEBUG_WIZARD_NAME "test"
#define NO_PRINTD
#define NO_ASSERT
#include <debug_utils.h>


varargs string cap_words(string str, int blah) {
    return implode(map_array(explode(str," "),#'capitalize)," ");
}    


int min(int a, int b) { 
    return (a < b) ? a : b;
}



string format_syntax(mapping m) {
    return implode(sort_array(m_indices(m),#'>), " | ");
}    


#define T_MAIN        1
#define T_RACE        2
#define T_ALIGNMENT   3
#define T_LEVEL       4
#define T_PARTYMAIN   5
#define T_PARTYKILLS  6
#define T_HOUSE       7
#define T_PROFESSION  8

int adminp(object who) {
    return (int) who->query_security_level() >= 90;
}


int playerp(object who) {
    return (int) who->query_security_level() == 0;
//    return 1;
}

int valid_house(string house) {
    return "/secure/house_d"->restore_house(house)
        || house == "commoner"
        || house == "noble";
}

int valid_level(string lev) {
    int ret;
    ret = to_int(lev);
    return ret > 0 && ret < 101;
}

int main(string arg) {
    int j, i, top, diff, rating, exp, kill, top_e, top_k, num_to_do;
    float prcnt;
    mixed *data;
    string *parts;
    int type;
    string header;
    string format;
    string file;
    int *data_idx;
    int tmp;
//    int is_player;

    info = data_idx = file = header = format = 0;

    if(arg && stringp(arg) && arg != "") {
        if(arg == "main") {
            type = T_MAIN;
                header = "    Name         House          Profession Race      Align. Rating     Level\n"
                         "    ~~~~~~~~~~~~ ~~~~~~~~~~~~~~ ~~~~~~~~~~ ~~~~~~~~~ ~~~~~~ ~~~~~~~~~~ ~~~~~\n";
                format = "%2d] %-12s %-14s %-10s %-9s %7s %10d %-5d\n";
                data_idx = ({ I_NAME, I_HOUSE, I_PROF, I_RACE, I_ALIGN, I_RATING, I_LEVEL });
            file = "/ob_saves/rankd/player_main";
        } else if(arg == "party main") {
            type = T_PARTYMAIN;
            header = "     Party Name                    Rating       Kills   Creator\n"
                     "     ~~~~~~~~~~                    ~~~~~~       ~~~~~   ~~~~~~~\n";
            format = "%3d] %-=21s   %12d   %9d   %-=13s\n";
            data_idx = ({ I_NAME, I_RATING, I_DATA1, I_CREATORNAME, 0, 0, 0 });
            file = "/ob_saves/rankd/party_main";
        } else if(arg == "party kills") {
            type = T_PARTYKILLS;
            header = "     Party Name                    Rating       Kills   Creator\n"
                     "     ~~~~~~~~~~                    ~~~~~~       ~~~~~   ~~~~~~~\n";
            format = "%3d] %-=21s   %12d   %9d   %-=13s\n";
            data_idx = ({ I_NAME, I_DATA1, I_RATING, I_CREATORNAME, 0, 0, 0 });
            file = "/ob_saves/rankd/party_kills";
        } else if(arg[0..4] == "race ") {
            if(valid_race[arg[5..]]) {
                type = T_RACE;
                header = "    Name         House          Profession Race      Align. Rating     Level\n"
                         "    ~~~~~~~~~~~~ ~~~~~~~~~~~~~~ ~~~~~~~~~~ ~~~~~~~~~ ~~~~~~ ~~~~~~~~~~ ~~~~~\n";
                format = "%2d] %-12s %-14s %-10s %-9s %7s %10d %-5d\n";
                data_idx = ({ I_NAME, I_HOUSE, I_PROF, I_RACE, I_ALIGN, I_RATING, I_LEVEL });
                file = "/ob_saves/rankd/player_race_"+arg[5..];
            } else {
                arg = 0;
            }
        } else if(arg[0..9] == "alignment ") {
            if(valid_alignment[arg[10..]]) {
                type = T_ALIGNMENT;
                header = "    Name         House          Profession Race      Align. Rating     Level\n"
                         "    ~~~~~~~~~~~~ ~~~~~~~~~~~~~~ ~~~~~~~~~~ ~~~~~~~~~ ~~~~~~ ~~~~~~~~~~ ~~~~~\n";
                format = "%2d] %-12s %-14s %-10s %-9s %7s %10d %-5d\n";
                data_idx = ({ I_NAME, I_HOUSE, I_PROF, I_RACE, I_ALIGN, I_RATING, I_LEVEL });
                file = "/ob_saves/rankd/player_alignment_"+arg[10..];
            } else {
                arg = 0;
            }
        } else if(arg[0..5] == "house ") {
            if(valid_house(arg[6..])) {
                type = T_HOUSE;
                header = "    Name         House          Profession Race      Align. Rating     Level\n"
                         "    ~~~~~~~~~~~~ ~~~~~~~~~~~~~~ ~~~~~~~~~~ ~~~~~~~~~ ~~~~~~ ~~~~~~~~~~ ~~~~~\n";
                format = "%2d] %-12s %-14s %-10s %-9s %7s %10d %-5d\n";
                data_idx = ({ I_NAME, I_HOUSE, I_PROF, I_RACE, I_ALIGN, I_RATING, I_LEVEL });
                file = "/ob_saves/rankd/player_house_"+arg[6..];
            } else {
                arg = 0;
            }
        } else if(arg[0..5] == "level ") {
            if(valid_level(arg[6..])) {
                type = T_LEVEL;
                header = "    Name         House          Profession Race      Align. Rating     Level\n"
                         "    ~~~~~~~~~~~~ ~~~~~~~~~~~~~~ ~~~~~~~~~~ ~~~~~~~~~ ~~~~~~ ~~~~~~~~~~ ~~~~~\n";
                format = "%2d] %-12s %-14s %-10s %-9s %7s %10d %-5d\n";
                data_idx = ({ I_NAME, I_HOUSE, I_PROF, I_RACE, I_ALIGN, I_RATING, I_LEVEL });
                file = "/ob_saves/rankd/player_level_"+arg[6..];
            } else {
                arg = 0;
            }
        } else if(arg[0..10] == "profession ") {
            if(valid_profession[arg[11..]]) {
                type = T_LEVEL;
                header = "    Name         House          Profession Race      Align. Rating     Level\n"
                         "    ~~~~~~~~~~~~ ~~~~~~~~~~~~~~ ~~~~~~~~~~ ~~~~~~~~~ ~~~~~~ ~~~~~~~~~~ ~~~~~\n";
                format = "%2d] %-12s %-14s %-10s %-9s %7s %10d %-5d\n";
                data_idx = ({ I_NAME, I_HOUSE, I_PROF, I_RACE, I_ALIGN, I_RATING, I_LEVEL });
                file = "/ob_saves/rankd/player_profession_"+arg[11..];
            } else {
                arg = 0;
            }
        } else if(arg[0..3] == "run " && adminp(this_player())) {
            if(arg[4..] == "players") {
                printf("Running rankings on players...\n");
                RANK_UPDATE_D->continue_rank(R_PLAYER,'a');
                return 1;
            } else if(arg[4..] == "parties") {
                printf("Running rankings on parties...\n");
                RANK_UPDATE_D->continue_rank(R_PARTY,'a');
                return 1;
            } else {
                arg = 0;
            }
        } else {
            if(valid_race[arg]) return main("race " + arg);
            if(valid_alignment[arg]) return main("alignment " + arg);
            if(valid_profession[arg]) return main("profession " + arg);
            if(valid_level(arg)) return main("level " + arg);
            if(valid_house(arg)) return main("house " + arg);
            if(arg == "kills") return main("party kills");
            if(arg == "party") return main("party main");
            arg = 0;
        }
    }
    if(!arg || !stringp(arg) || arg == "") {
        notify_fail(sprintf("Syntax:\n"
                    "    rankings main\n"
                    "    rankings race %-=50s\n"
                    "    rankings alignment %-=50s\n"
                    "    rankings profession %-=50s\n"
                    "    rankings house <housename>\n"
                    "    rankings level [ 1 - 100 ]\n"
                    "    rankings party [ main | kills ]\n"
                    "    rankings %-=60s\n"
                    "%s",
                        sprintf("[ %s ]",format_syntax(valid_race)),
                        sprintf("[ %s ]",format_syntax(valid_alignment)),
                        sprintf("[ %s ]",format_syntax(valid_profession)),
                        sprintf("[ %s ]",format_syntax(valid_race+valid_alignment+valid_profession+([ "1 - 100":1, "<housename>":1,"party":1,"kills":1,"main":1]))),
                        adminp(this_player()) ? "    ----\n    rankings run [ players | parties ]\n" : "" ));
        return 0;
    }
    
//     is_player = !(this_player()->query_security());

    if(playerp(this_player())) {
        parts = explode(header,"\n");
        if(parts[0][<5..<1] == "Level") {
            parts[0] = parts[0][0..<6];
            parts[1] = parts[1][0..<6];
            header = implode(parts,"\n");
            if(format[<5..<1] == "%-5d\n")
                format = format[0..<6] + "\n";
        }
    }

    printf(NORM PRINTBAR "\n");
    printf(B_RED_F "%s" B_CYAN_F, header);

    restore_object(file);

    if(!info) {
        printf(YELLOW_F "             <no data available>\n" NORM);
        return 1;
    }

    j = sizeof(info[A_KEYS]);

    if((num_to_do = (int) this_player()->query_env_var("NUMRANKING"))
    && intp(num_to_do)
    && (num_to_do > 0)) 
        num_to_do = min(j,num_to_do);
    else
        num_to_do = j;

    while(j-- && i < num_to_do) {
          data = info[A_DATA][j];
        printf(format,
            ++i,
            cap_words(data[data_idx[0]]),
            data[data_idx[1]],
            data[data_idx[2]],
            data[data_idx[3]],
            data[data_idx[4]],
            data[data_idx[5]],
            data[data_idx[6]]);
    }
    printf(NORM PRINTBAR "\n");
    return 1;
}
    
