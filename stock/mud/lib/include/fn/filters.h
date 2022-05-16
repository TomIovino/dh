 /**************************************************************************/
 /* filter_users - removes invis, and players logging in */


static status filter_users(object ob) {
  int sec, lvl;

  if(!environment(ob)) return 0;
  sec = (int)ob->query_security_level();
  lvl = (int)ob->query_level();
  if(sec) {
    if((ob->query_invis() && 
       sec <= this_player()->security_level()) || !ob->query_invis()) {
      return 1;
    }
  }
  else {
    if((ob->query_invis() && (lvl <= (int)this_player()->query_level() ||
      (int)this_player()->query_security_level()))
    || !ob->query_invis()) {
      return 1;
    }
  if(ob == this_player()) return 1;
  }
  return 0;
}

status filter_wizards(object ob) {
  return (ob->query_security_level()) ? 1 : 0;
}

     
 /***************************************************************************/
 /* sort by security level */

static status by_sec_level(object a, object b) { 
  if((int)a->query_security_level() == (int)b->query_security_level()
    && (int)a->query_level() < (int)b->query_level()) return 1;
  return
  ((int)a->query_security_level() < (int)b->query_security_level()) ? 1 : 0;
}
