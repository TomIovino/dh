/** Invisibilty **/
 

string *query_cmd_aliases() { return
({"invisible", }); }
 
 
status invisibility(mixed targ, mixed alt_type) {
  int time, i;
  string file;
  string x,y;
 
  if(sscanf(this_player()->query_current_room(), "%s/arena/%s", x,y)== 2) {
      return 1;
      }

  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
 
  this_player()->load_spell(({
  "target",            this_player(),
  "name",              "invisibility",
  "type",              "focus",
  "stat",              "intellect",
  "level",             50,
  "cost",              50,
  "damage",            time,
  "cast time",         1,
  "spell object",      file,
  "passive",
 
/* other spell options... 
  "msg target",       0,
  "msg room",         0,
  "msg caster",       0,
 */
  }));
  return 1;
}
 
 
/* call when this object is cloned to target */
 
status cast_spell(object caster, object target, object previous, int time) {
  object ob;
  int failed;

    if(caster->query_invis()) {
        write("You extend the time of your invisibility spell.\n");
        destruct(this_object());
        return 1;
   }
  caster->toggle_invis();
  return 1;
}
 
 
 
 
 
void dispel_magic(){ destruct(this_object()); }
void time_out(){ if(this_player()->query_invis()){
                    this_player()->toggle_invis() ; }
                  destruct(this_object());
    }
 
 
status id(string str) { 
  return str == "spell"
      || str == "invisibility";
}
 
 
int query_no_save(){ return 1; }
