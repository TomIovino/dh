  /* Check to see if player exists. Made special for Bubbs. :) */
  // Dax of Dragon's Heart 8-6-95

#include <player.cfg>

status user_exists(string who) {
  who = lower_case(who);
  return restore_object(SAVE_NO_BANISH+who)
      || restore_object(SAVE_WIZARD+who)
      || restore_object(SAVE_PLAYER+who)
      || restore_object(SAVE_FIRST +who);
}
