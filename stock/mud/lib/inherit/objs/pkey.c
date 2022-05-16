#include <ansi.h>
#include <mudlib.h>
inherit TREASURE;
reset(arg){
  ::reset(arg);
  call_out("dest",60);
  if(arg)return;
  set_name("pkey");
  set_short("");
  set_long("");
  }
dest(){
 destruct(this_object());
 }
drop(){ destruct(this_object()); }
