////////////////////////////////////////////////////////////
// SHOPKEEP.C         // DRAGONHEART // ASH.5.17.97       //
////////////////////////////////////////////////////////////

#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;

string STOREROOM, SIGN_TEXT;
string storeroom_ok, not_ok_mess1, not_ok_mess2;
string storeroom_exit, owner, owner_objective;
string *always_in_stock;
int max_value, total_stock, updated, owner_cha;
int max_list_length, bargain, max_stock;
object store_keep;
string type;  // weapon armour magic clothing general

#include <ansi_filter.h>

string calc_value(int i);


void reset(status arg) {
     ::reset(arg);
     load_spells(10, ({ "heal", }));
}

string format(){
    if(!STOREROOM) STOREROOM = ENV;
   if(!SIGN_TEXT) SIGN_TEXT = "\
The commands are:\n\n\list\n\browse\n\buy\n\value\n\sell\n";
    owner_objective = "he";
    max_value = 10000;
    total_stock = 60;
    owner_cha = 50;
    storeroom_exit = "storage";
    max_list_length = 100;
    max_stock = 1;
    not_ok_mess1 = "\
You attempt to enter the storeroom, but are instantly blocked by one of \n\
clerks.\n";
    not_ok_mess2 = "\
%s attempts to enter the storeroom but is instantly blocked \n\
by a large clerk.\n",
    storeroom_ok = "into the storeroom.";

return "";
}

string set_type(string t){ type = t; return t;}
string calc_value(int i);
set_in_stock(string *arr) {
    int i;
    object ob;
    always_in_stock = arr;
    if(type == "armour"){
    for(i=0;i<sizeof(arr);i++){
        ob = clone_object(arr[i]);
         if(ob->query_ac() > 15) destruct(ob);
        else move_object(ob,STOREROOM);
    }

    }
    else if(type == "weapon"){
    for(i=0;i<sizeof(arr);i++){
        ob = clone_object(arr[i]);
        if(ob->query_wc() > 20 )destruct(ob);
        else move_object(ob,STOREROOM);
    }

    }
    else
    for(i=0;i<sizeof(arr);i++)
        move_object(clone_object(arr[i]),STOREROOM);
}

init() {
    NPC::init();
    format();
     if(this_player()->is_npc()){return 0;}
    add_action("do_sell", "sell");
    add_action("do_list", "list");
    add_action("do_buy", "browse");
    add_action("do_buy", "buy");
    add_action("do_read", "read");
    add_action("goto_storeroom", "storage");
    STOREROOM->reset();
    }

get_clerk() {
}

do_read(str) {
    if(str == "sign") {
    printf(SIGN_TEXT);
    return 1;
    }
}

do_list(str) {
    int i, sz, v, g, c, s;
    string tmp, list, listing;
    int amount, max_v;
    string *item_number, *item_check, *item_list;
    string tmp1;
    object *to_list, *ret_value, ob;

    g = c = s = amount = 0;
    owner = this_object()->query_name();
    if(!str) str = "all";
    if(objectp(STOREROOM)) to_list = all_inventory(STOREROOM);
    else to_list = all_inventory(find_object(STOREROOM));
    ret_value = 
    "/inherit/shopstuff"->get_relevant_stuff(to_list,sizeof(to_list),str);
    to_list = ret_value[1];
    if(!(sz = ret_value[0]))
    if(str == "all")
        printf("\
%s tells you that there is nothing for sale at the moment.\n", owner);
    else
        printf("\
%s tells you that there is nothing like that for sale.\n", owner);
    else {
    str = (( str == "all") ? "for sale" : "matching that description");
    tmp = (( sz == 1) ? "only object" : "objects");
    if(sz > 50) {
        printf("\
%s tells you there are many items %s\nand lists those %s remembers.\n",
          owner, str, owner_objective);
        sz = 50;
    }
    else
        printf("%s lists the %s %s.\n", owner, tmp, str);
    item_check = ({ });
    item_number = ({ });
    item_list = ({ });
    for(i = 0; i < sz; i++) {
        int q;
        g = c = s = amount = v = 0;
        ob = to_list[i];
        v = ob->query_value() * 2;
        max_v = ob->query_max_value() *2;
        if(!max_v)  max_v = ob->query_value()*2;
        tmp1 = filter_ansi(ob->short()) +"#"+ max_v;
        if((q = member_array(tmp1, item_check)) != -1) {
        if(item_number[q] >= max_stock)
            destruct(ob);
        else
            item_number[q] += 1;
        continue;
        }
        amount = v;
        if(amount >= 100) g = (amount/100);
        while(amount >= 100) {
        amount -= 100;
        }
        if(amount >= 10) s = (amount/10);
        while(amount >= 10) {
        amount -= 10;
        }
        if(amount) c = amount;
        tmp = "  "+ob->short();
        list = sprintf("[%s%s %s %s%s]  %s",
          NORM,
          g ? sprintf("%s%2dg",YELLOW_F,g) :    "   ",
          s ? sprintf("%s%2ds",B_WHITE_F,s) :   "   ",
          c ? sprintf("%s%2dc",NORM+BROWN_F,c) :"   ",
          NORM,tmp);
        item_check += ({ tmp1, });
        item_list += ({ list, });
        item_number += ({ 1, });
    }
    listing = "";
    for( i = 0; i < sizeof(item_list); i++) {
        listing += /*  item_number[i] + */ "  "+item_list[i] + "\n";
    }
    printf(listing);
    }
    return 1;
}


goto_storeroom() {
    object tp;

    if((tp = this_player())->query_security_level())
    tp->move_player(storeroom_ok, STOREROOM);
    else {
    printf(not_ok_mess1);
    tell_room(this_object(), sprintf(not_ok_mess2,
        tp->query_name()), ({ tp }));
    }
    return 1;
}

do_sell(str) {
    int i, j, sz, szi, scnt, dcnt;
    int not_single, valu, bargain;
    object *to_sell, *sold, *ret_value, ob, container, tp, sr;
    status flag;
    string tmp;
    int sum;
    owner = this_object()->query_name();
    tp = this_player();
    if(!str) return 0;
    if(str == "all") str = "extra";
    bargain = 0; 
    if(sscanf(str, "%s from %s", str, tmp) != 2) {
    container = tp;
    tmp = "";
    } else {
    if(!container = present(tmp, tp)) {
        printf("You are not carrying a %s.\n", tmp);
        return 1;
    }
    if(1 != i = container->accessable()) {
        if(!i)
        printf("The %s is not a container.\n", tmp);
        return 1;
    }
    tmp = sprintf(" from %s", container->short());
    }
    ret_value = "/inherit/shopstuff"
    ->get_relevant_stuff(to_sell = all_inventory(container),
      sizeof(to_sell), str);
    to_sell = ret_value[1];
    if(!(sz = ret_value[0])) {
    if(str == "all") printf("You do not see anything to sell.\n");
    else printf("You do not see anything like this to sell.\n");
    return 1;
    }
    ret_value = "/inherit/shopstuff"->get_big_array(2);
    sold = ret_value[0];
    not_single = (sz != 1);
    for( i = sum = scnt = dcnt = 0; i < sz; i++) {
    ob = to_sell[i];
        if(ob->query_worn() || ob->query_wielded()) {
        printf("\
%s says: To avoid any confusion I recomend you remove your \n\
\t%s before selling it.\n", owner, ob->query_name());
        return 1;
        }
    if(ob->query_owner() && ob->query_owner() != ""){
        write("That would be a mistake.\n");
        return 1;
   }
    if(ob->query_food() || ob->query_drink()){
  write("The shopkeeper says, 'I don't want a stinkin "+ob->query_name()+"!\n");
        return 1;
    }
    if(ob->short() && ob->query_value() && !ob->drop()) {
        if(type == "weapon" && !ob->query_wc()){
        write("The shopkeeper says 'I only buy weapons!\n");
        return 1;
        } else if(type == "armour" && !ob->query_ac()){
        write("The shopkeeper says 'I only buy armour!'\n");
        return 1;
        } else if(type == "magic" && !ob->query_bonus()){
        write("The shopkeeper says 'I only buy magic!\n");
        return 1;
        } else if(type == "clothing" && !ob->query_ac()){
        write("The shopkeeper says 'I only buy clothing!\n");
        return 1;
        } else if(type == "clothing" && ob->query_ac() > 2){
        write("The shopkeeper says 'I only buy clothing!\n");
        return 1;
        }

            if(!ob->query_short()){
                write("The shopkeeper says, I dont SEE that.\n");
                return 1;
            }
            value = (int)ob->query_value();
            if(value < 1) value = 1;
            if(value > max_value) value = max_value;
            value = this_player()->sell(value);
        printf("\
    %s haggles with you over the price of %s.\n",
          owner, ob->short());
  printf("%s gives you %s for %s\n",
       owner, calc_value(value), ob->short());
        tp->add_money(value);
        container->add_weight(ob->query_weight() * (-1));
        if(ob->query_sell_destruct()) {
        printf("\
    %s gives the item to a clerk in the storeroom.\n", owner);
        destruct(ob);
        }
	else if(ob->query_ego()) {
        printf("\
    %s gives the item to a clerk in the storeroom.\n", owner);
        destruct(ob);
        }
        else {
            if(catch(move_object(ob, STOREROOM))) {
                catch(STOREROOM->Load_object());
                sr = find_object(STOREROOM);
                if(sr && catch(move_object(ob, sr))) {
                    log_file("SERR",sprintf("%s %O %O %O\n",timestamp(),this_object(),ob,STOREROOM));
                    destruct(ob);
                }
            }
        }
        flag = 1;
    }
    }
    if(!flag) printf("%s isn't interested.\n", owner);
    this_player() && this_player()->recalc_carry();
    return 1;
}

do_buy(str) {
    object ob, tp, *inv;
    int value, number, i, j, abort;
    string type, verb, *item_check;
    string ob_path, nothing;
     owner = this_object()->query_name();
    verb = query_verb();
    tp = this_player();
    if(!str) {
    printf("\
%s says: What do you want to %s?\n", owner, verb);
    return 0;
    }
    if(!present(lower_case(owner))) {
    printf("\
The shopkeeper doesn't seem to be around right now to help you.\n");
    return 1;
    }
    ob = find_object(STOREROOM);
    if(!ob) {
    printf("\
%s tells you that his warehouse was burned down in a fire and that \n\
you should contact a creator to rebuild it.\n", owner);
    return 1;
    }
    ob = present(str, find_object(STOREROOM));
    if(!ob) {
    printf("\
%s says: Sorry, I don't have any of those in stock.\n", owner);
    return 1;
    }
    if(ob->query_value() == 0){ write("You can't buy that\n"); return 1; }
    if(verb == "browse") {
    printf("\
%s shows you the %s.\n", owner, ob->query_name());
    ob->long();
    return 1;
    }
    value = ob->query_value();
    value = this_player()->buy(value);
    printf("\
%s haggles you over the price of %s.\n", owner, ob->short());
    if(tp->query_money() < value) {
    printf("\
%s says: You seem to be a little short on funds!\n", owner);
    return 1;
    }
    if(!call_other(tp, "add_weight", ob->query_weight())) {
    printf("\
%s says: You should gain a little more muscle before buying the \n\
%s.\n", owner, ob->query_name());
    return 1;
    }
    tell_room(environment(tp), sprintf("\
%s buys a %s.\n", tp->query_name(), ob->query_name()), ({ tp }));
    printf("\
%s takes %s for %s.\n", owner, calc_value(value), ob->short());
    move_object(ob, tp);
    tp->add_money(-value);
    ob_path = file_name(ob);
    sscanf(ob_path, "%s#%s", ob_path, nothing);
    if(always_in_stock && !present(str, find_object(STOREROOM)))
    for(i = 0; i < sizeof(always_in_stock); i++) {
        if(ob_path == always_in_stock[i]) {
        printf("\
%s says: There is quite a demand for those, you know? I am \n\
really glad that I have a number of them in stock.\n", owner);
        move_object(clone_object(always_in_stock[i]), STOREROOM);
        }
    }
    return 1;
}

void set_owner(string owner_name) {
}

#define GOLD    100
#define SILVER  10
#define COPPER  1
 
string calc_value(int i) {
  string tmp;
  tmp = "";
  if(i >= GOLD) {
      tmp += (YELLOW_F + i/GOLD + " gold"+NORM);
      i %= GOLD;
  }
  if(i >= SILVER) {
      tmp += (tmp == "" ? "" : " ")
          + (B_WHITE_F + i/SILVER +" silver"+NORM);
      i %= SILVER;
  }
  if(i) {
      tmp += (tmp == "" ? "" : " ")
           + (NORM+BROWN_F+ i +" copper"+NORM);
  }
  return tmp;
}

