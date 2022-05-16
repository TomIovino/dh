////////////////////////////////////////////////////////////
// BANKEEP.C          // DRAGONHEART // ASH.5.17.97       //
////////////////////////////////////////////////////////////

#include <ansi.h>
#define OWNER this_object()->query_name()
string owner, owner_name;
object owner_ob;

#include <mudlib.h>
#include <money.cfg>
inherit MONSTER;

mixed calc_value(int i);
void  set_owner(string str);

void reset(status arg) {
   ::reset(arg);
  if(arg) return;
  set_name("fred");
  set_short("human banker");
  set_alias("banker");
  set_long("A friendly money keeper");
  set_no_exp(1);
  set_level(50);
  set_race("human");



}

void init() {
  ::init();
  add_action("read", "read");
  add_action("withdraw", "withdraw");
  add_action("deposit",  "deposit");
  add_action("balance",  "balance");
}

status read(string str) {
  if(str == "plaque") {
    say(this_player()->query_name()+" reads the plaque on the wall.\n");
    printf("\
%sThe plaque reads:%s\n\
\t\t%sWelcome to %s!%s\n\
%sThe bank is a secure place to store away your money when you are out \n\
adventuring. The bank charges five percent on all transactions, which \n\
can be carried out with:%s\n\
%swithdraw <amount>    -    deposit <amount>    -    balance%s\n\
%sThe default is copper but you can specify gold or silver coins.\n\
1 gold = 10 silver, 1 silver = 100 copper coins.%s\n",
    B_MAGENTA_F, NORM, MAGENTA_F, short(0), NORM,
    GREY_F, NORM, MAGENTA_F, NORM, GREY_F, NORM);
    return 1;
  }
  return 0;
}

status withdraw(string str) {
  string tmp1, tmp2;
  int amount;

  if(!str) {
    write(OWNER+" asks: What did you say?\n");
    return 1;
  }

  if(sscanf(str, "%d %s", amount, tmp1)) {
    if(tmp1 == "gold")        amount *= GOLD;
    else if(tmp1 == "silver") amount *= SILVER;
  }
  if(amount > (int)this_player()->query_savings()) {
    write(OWNER+" says: You don't have "+
    "that much in the vault!\n");
    return 1;
  }
  if(amount < 1) { 
   write(OWNER+ " says: nice try!\n");
   return 1;
   }
  write(OWNER+" takes "+calc_value(amount)+" out of the vault.\n");
   write(OWNER+" takes "+
          calc_value((amount/20)+1)+" as his fee.\n");
  this_player()->add_savings(-amount);
  amount -= (amount/20)+1;
  this_player()->add_money(amount);
  write(OWNER+" gives you "+calc_value(amount)+" coins.\n");
  say((string)this_player()->query_name()+" withdraws some money.\n");
  return 1;
}

status deposit(string str) {
  string tmp1, tmp2;
  int amount;

  if(!str) {
    write(OWNER+" asks: What was that? What did you say?\n");
    return 1;
  }

  if(sscanf(str, "%d %s", amount, tmp1)) {
    if(tmp1 == "gold")    amount *= GOLD;
    if(tmp1 == "silver")  amount *= SILVER;
  }

  if(amount > this_player()->query_money()) {
   write(OWNER+" exclaims: You don't have that much money to deposit!\n");
   return 1;
  }
  if(amount < 1){
   write(OWNER+" says: nice try!\n");
   return 1;
   }

  this_player()->add_money(-amount);
/**
  if(!amount/20) {
    write(OWNER+" takes "+calc_value(1)+" as his fee.\n");
  }
  write(OWNER+" takes "+calc_value(amount/20)+" as his fee.\n");
  if(!amount/20) amount -= 1;
  else
    amount -= amount/20;
**/
  this_player()->add_savings(amount);
  write(OWNER+" places "+calc_value(amount)+" in the vault.\n");
  return 1;
}


status balance() {

  write(OWNER+" says: "+ 
   this_player()->query_money_string()+"\n");
  return 1;
}


/* exchange rate for coins - do not alter 
 * 1 gold = 10 silver, 1 silver = 100 copper
 */
mixed calc_value(int amount) {
  int amt, g, s, c;
  string tmp;

  g = s = c = 0;
  amt = amount;
  tmp = "";
  if(amt >= GOLD) g = amt/GOLD;
  while(amt >= GOLD) {
    amt -= GOLD;
  }
  if(amt >= SILVER) s = amt/SILVER;
  while(amt >= SILVER) {
    amt -= SILVER;
  }
  if(amt) c = amt;
  tmp = "/inherit/shopstuff"->money_string(g,s,c);
  if(!tmp) tmp = "nothing";
  return tmp;
}

