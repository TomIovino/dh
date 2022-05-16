
#include <player.cfg>

status user_exists(string who) {
  who = lower_case(who);
  who = subst(who," ",""); 
  who = subst(who,".","");
  who = subst(who,"/",""); 
  who = subst(who,"*","");
  who = subst(who,"?","");
  return restore_object(SAVE_NO_BANISH+who)
      || restore_object(SAVE_WIZARD+who)
      || restore_object(GET_SAVE_PLAYER(who))
      || restore_object(SAVE_FIRST +who);
}
