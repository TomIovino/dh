/* /secure/loadlogger
**
** Just makes every five minutes a one line log into a file.
** The line contains:
**   ctime() #total_users #players #wizards cmds/s lines/s #objects
** The numbers may be evaluated later with GNUplot or similar.
**
** Idea by Freaky (Unitopia).
**
**   11-Jan-94 [Mateese]
**   01-Aug-96 [Mateese] Added #objects count.
**   02-Aug-97 [Deepthought] Made number of objects calculation much faster
**   17-Dec-97 [Geewhiz]  Brought to Dragonheart.
*/

#define LOGFILE ("LOADLOG")
#define DELAY 600

#define PO previous_object()
#define TP this_player()
#define TI this_interactive()
#define TO this_object()

/*------------------------------------------------------------------------*/

reset() {

  if (-1 != member_array ('#', file_name(TO))) {
    log_file("/log/LOADLOG.bad", ctime(time())+": Illegal attempt!\n"
               +"  this:        "+file_name(TO)+" ["+getuid(TO)+"]\n"
      +"  previous:    "+(PO ? file_name(PO)+" ["+getuid(PO)+"]" : "<none>")+"\n"
      +"  player:      "+(TP ? file_name(TP)+" ["+getuid(TP)+"]" : "<none>")+"\n"
      +"  interactive: "+(TI ? file_name(TI)+" ["+getuid(TI)+"]" : "<none>")+"\n\n"
         );
    destruct (TO);
    return;
  }
  do_log();
}

/*------------------------------------------------------------------------*/

static do_log() {
  string *s, rc;
  object *pl, o;
  mixed *wiz;
  int nru, nro, nrp, nrw;
  while (remove_call_out("do_log") != -1)
    /* do nothing */;
  pl = users();
  nru = sizeof(pl);
  nrw = sizeof(map_objects(pl, "query_security_level") - ({ 0 }));
  nrp = nru - nrw;
  s = explode(query_load_average(), " ");
  nro = numobjects();
  rc = ctime()+"\t"
       +nru +"\t"
       +nrp +"\t"
       +nrw +"\t"
       +s[0]+"\t"
       +s[2]+"\t"
       + nro+"\n";
  log_file(LOGFILE, rc);
  call_out("do_log", DELAY);
}

/*------------------------------------------------------------------------*/
static numobjects()
{
  int num;
  object obj;
  int lower;
  int upper;

  upper = 1 << 24;
  lower = 0;

  while (lower != upper) {
    num = (lower+upper)/2+1;
    obj = debug_info(2, num);
    if (obj)
      lower = num;
    else
      upper = num-1;
  }
  return num;
}
/*========================================================================*/

