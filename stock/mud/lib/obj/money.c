  ////////////////////////////////////////////////////////////
  // MONEY.C         //  CRIMSONESTI @  // ASH.5.17.97      //
  ////////////////////////////////////////////////////////////

int money;

#include <money.cfg>
#include <ansi.h>

reset(arg) {
    if (arg) return;
    money = 1;
}

query_weight() { return 0; }

short() {
    if (money <= 0)
        return 0;
    return convert(money) +" coins";
}

query_name() { return to_string(short()); }

init()
{
  if (environment(this_object())==this_player()) {
    call_other(this_player(), "add_money", money);
    if(money > 10000) {
        log_file("MONEYTRACK",sprintf("%s\n%s\n  %O recieved %s (%O)\n\n",
        timestamp(),caller_stack(),this_player(),convert(money),money));
    }
    money = 0;
    set_heart_beat(1);
  }
}

get()
{
  return money>0;
}

set_money(m) {
    if(m <= 0)
        write_file("/log/MONEYBUGS",sprintf("%s\n%s\n  money::set_money(%O)\n\n",timestamp(),caller_stack(),m));
    money = m;
}

id(str) {
    if (str == "coins")
    return 1;
    if (str == "money")
    return 1;
}

heart_beat() {
    if(money <= 0)
    destruct(this_object());
}

/* Add money added by Kingbilly, to allow for incremental */
/* change of amounts */

add_money(m)
{
    int old_money;
    old_money = money;
    money += m;
    if(money <= 0) {
        write_file("/log/MONEYBUGS",sprintf("%s\n%s\n  money::add_money(%O)\n  money was O%\n  now is %O\n\n",timestamp(),caller_stack(),m,old_money,money));
    }
    
}
/* Money Converter.
   THIS CODE IS USED IN PLAYER, SHOPSTUFF, INV..
 */
 
 
string convert(int i) {
  string tmp;
  if(money <= 0)
   write_file("/log/MONEYBUGS",
   sprintf("%s\n%s\n  money::convert()\n  money = %O\n",
   ctime(time()),caller_stack(),money));
  tmp = "";
  if(i >= GOLD)      {
      tmp += (YELLOW_F + i/GOLD + " gold"+NORM);
      i %= GOLD;     }
  if(i >= SILVER)    {
      tmp += (tmp == "" ? "" : " ") 
          + (B_WHITE_F + i/SILVER +" silver"+NORM);
      i %= SILVER;   }
  if(i)              { tmp += (tmp == "" ? "" : " ") 
          + (NORM+BROWN_F+ i +" copper"+NORM);  }
  return tmp;
}
string miniconvert(int i) {
  string tmp;
  if(money <= 0)
   write_file("/log/MONEYBUGS",
   sprintf("%s\n%s\n  money::convert()\n  money = %O\n",
   ctime(time()),caller_stack(),money));
  tmp = "";
  if(i >= GOLD)      {
      tmp += (i/GOLD + "G");
      i %= GOLD;     }
  if(i >= SILVER)    {
      tmp += (tmp == "" ? "" : ", ") 
          +(i/SILVER +"S");
      i %= SILVER;   }
  if(i) { 
      tmp += (tmp == "" ? "" : ", ") 
          +(i +"C"); }
  return tmp;
}

