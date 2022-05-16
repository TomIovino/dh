// Convert data from one format in player.c to another.
// note this program needs to changed for each conversion run.

//#define TEST "secure/daemons/test"
#include <races.h>

// Bubbs. 29/Aug/93.

#if MUD_NAME == "Timewarp"
#define TIME_BETWEEN_DIRS    10
#else
#define TIME_BETWEEN_DIRS     5
#endif

// Here, we need a list of ALL variables in the player object.
// Including old ones, and new ones.
// Since they change quite often, probably need to re-collate this also.

mapping  periodic_effects,
         skills,
         money,
         attribs;
string   name,
         class,
         race;
int      level,
         age,
         experience,
         time_to_heal,
         gender,
         lawfullness,
         hit_point,
         spell_points,
         action_points,
         max_hp,
         max_sp,
         max_ap,
         PS,
         CO,
         QU,
         AG,
         MA,
         RE,
         PR,
         ME;
status   ghost;
mapping  aliases;
string  *cmd_paths;
mapping  env_vars,
         channels,
         account,
         finger_info;
mixed   *auto_load;
string  *quests,
         password,
         title,
         law_title;
int      scar,
         wiz_level,
         lives,
         total_value,
         stuffed,
         soaked,
         intoxicated,
         headache,
         max_headache,
         exp_modifier,
         magic_block,
         quest_points;
status   is_invis;	
int      player_killer;


convert() {
  call_out("convert_directory", TIME_BETWEEN_DIRS, 'a');
  }

end_convert() {
  if (this_player())
   write("Convert finished.\n");
  return;
  }

convert_directory(char) {
  string *files;
  int index, size;
  files = get_dir(sprintf("/data/users/%c/*.o", char));
  size = sizeof(files);
  for(index = 0; index < size; index++)
   convert_character(sprintf("data/users/%c/%s", char, files[index][0..<3]));
  if (char == 'z') end_convert();
  else call_out("convert_directory", TIME_BETWEEN_DIRS, char+1);
  return;
  }

convert_character(file) {
  restore_object(file);
  convert_data();
#if defined(TEST)
  save_object(TEST);
#else
  save_object(file);
#endif
  return;
  }

// This is the function that needs to be changed.
convert_data()
{
    if (member(quests, "Orc Slayer") != -1) quest_points-=10;
    if (member(quests, "Elven Banner") != -1) quest_points-=40;
    if (member(quests, "Fountain of Justice") != -1) quest_points-=50;
    quests-=({ "Orc Slayer", "Elven Banner", "Fountain of Justice" });
}
