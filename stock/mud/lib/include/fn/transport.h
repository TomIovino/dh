#define VALID "/secure/valid_d"

/**********************************************************************/
/* transport macros */
string enter_room;

status goto_enter(){
   if(enter_room) move_player("X#"+enter_room);
   if(!enter_room) this_player()->church();
   if(!this_player()->query_current_room())
   move_player("X#"+VOID);
   return 1;
}


status church() {
  string dest;
  if(query_alignment()){
     move_player("X#/d/town/church/church");
  return 1;
  }
   move_player("X#"+VOID);
  return 1; 
}

status arena() {
  string dest;
     move_player("X#","/d/arena/main");
     this_object()->adj_hp(1000);
     this_object()->adj_sp(1000);
     this_object()->adj_mp(1000);
     this_object()->unset_env_var("ARENA");
  return 1; 
}



/***************************************************************************/

