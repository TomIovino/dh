////////////////////////////////////////////////////////////
// SHOPSTUFF.C        // DRAGONHEART // ASH.5.17.97       //
////////////////////////////////////////////////////////////

#include <ansi.h>
#include <money.cfg>

string plural(string str);
string strip_article(string str);
object *obj_array, *two_ret_values, *three_ret_values;
object *big_array1, *big_array2, big_array3, *big_arrays;

reset(arg){
  if(arg) return;
  big_arrays = allocate(3);
  big_arrays[0] = (big_array1 = allocate(1024));
  big_arrays[1] = (big_array2 = allocate(1024));
  big_arrays[2] = (big_array3 = allocate(1024));
  two_ret_values = allocate(2);
  three_ret_values = allocate(3);
  obj_array = allocate(1024);
  return;
}

object *get_big_array(int how_many) {
  switch(how_many) {
    case 0:
    case 1:
      return big_array1;
    case 2:
    case 3:
      return big_arrays;
  }
}
string *numbers = ({"zero", "one", "two", "three", "four", "five",
                   "six", "seven", "eight", "nine", "ten",
                   "eleven", "twelve", "thirteen", "fourteen", "fifteen",
                   "sixteen", "seventeen", "eighteen", "nineteen", "twenty",
                   "twenty one", "twenty two", "twenty three",
                   "twenty four", "twenty five", "twenty six",
                   "twenty seven", "twenty eight", "twenty nine", "thirty",
                   "thirty one", "thirty two", "thirty three",
                   "thirty four", "thirty five", "thirty six",
                   "thirty seven", "thirty eight", "thirty nine", "forty",
                   "forty one", "forty two", "forty three",
                   "forty four", "forty five", "forty six",
                   "forty seven", "forty eight", "forty nine", "fifty",
                   "fifty one", "fifty two", "fifty three",
                   "fifty four", "fifty five", "fifty six",
                   "fifty seven", "fifty eight", "fifty nine", "sixty",
                   "sixty one", "sixty two", "sixty three",
                   "sixty four", "sixty five", "sixty six",
                   "sixty seven", "sixty eight", "sixty nine", "seventy",
                   "seventy one", "seventy two", "seventy three",
                   "seventy four", "seventy five", "seventy six",
                   "seventy seven","seventy eight", "seventy nine", "eighty",
                   "eighty one", "eighty two", "eighty three",
                   "eighty four", "eighty five", "eighty six",
                   "eighty seven", "eighty eight", "eighty nine", "ninety",
                   "ninety one", "ninety two", "ninety three",
                   "ninety four", "ninety five", "ninety six",
                   "ninety seven", "ninety eight", "ninety nine"});


query_number(num) {
  if(num < 0) return 0;
  if(num < sizeof(numbers))
    return(numbers[num]);
  return("a lot of");
}

get_relevant_stuff(array, size, str){
  int i, cnt, num;
  string name;
  object ob, *str_array;

  obj_array = allocate(2048);


  if(str == "all"){                    
    if(size > 1024) size = 1024;
    for(i = cnt = 0; i < size; i ++)
      if((ob = array[i])->short()) {
//        printf("i = %O | cnt = %O | ob = %O | array[%O] = %O\n",i,cnt,ob,i,array[i]);
        obj_array[cnt++] = ob;
      }
    two_ret_values[0] = cnt;
    two_ret_values[1] = obj_array;
    return two_ret_values;
  }

if(str == "all gear" || str == "gear" || str == "extra" || str == "extra gear"){
    for(i = cnt = 0; i < size; i ++)
      if((ob = array[i])->short() &&
        (ob->armour_class() || ob->weapon_class()) &&
        (!ob->query_worn() && !ob->query_wielded()))
        obj_array[cnt++] = ob;
    two_ret_values[0] = cnt;
    two_ret_values[1] = obj_array;
    return two_ret_values;
  }
  if(str == "all equip" || str == "equip" || str == "all equipment" ||
     str == "equipment") {
    for( i = cnt = 0; i < size; i ++)
      if((ob = array[i])->short() &&
         (ob->query_worn() || ob->query_wielded()))
        obj_array[cnt++] = ob;
    two_ret_values[0] = cnt;
    two_ret_values[1] = obj_array;
    return two_ret_values;
  }

  if(str == "all loot" || str == "loot"){
    for(i = cnt = 0; i < size; i ++)
      if((ob = array[i])->short() &&
         !ob->weapon_class() &&
         !ob->armour_class() &&
         !ob->query_wielded() &&
         !ob->query_worn())
        obj_array[cnt++] = ob;
    two_ret_values[0] = cnt;
    two_ret_values[1] = obj_array;
    return two_ret_values;
  }


  str_array = explode(str, " ");
  cnt = sizeof(str_array) - 1;
  if(sscanf(str_array[cnt],"%d", num) != 1){
    name = str;
    num = 1;
  } else {
    for(i = 1, name = str_array[0]; i < cnt; i++)
      name = sprintf("%s %s", name, str_array[i]);
  }
  
  for(i = 0; i < size; i ++)
    if((ob = array[i])->id(name))
      if(!(--num)){
        two_ret_values[0] = 1;
        obj_array[0] = ob;
        two_ret_values[1] = obj_array;
        return two_ret_values;
      }


  if(sscanf(str,"all %s", name) != 1)
    name = str;
  for(i = cnt = 0; i < size; i ++)
    if((ob = array[i])->short() &&
        (ob->id(name) || ob->id(name) || ob->query_type() == name))
      obj_array[cnt++] = ob;
  two_ret_values[0] = cnt;
  two_ret_values[1] = obj_array;
  if(cnt) return two_ret_values;
  
  if(str == "all armour" || str == "armour"){
    for(i = cnt = 0; i < size; i ++)
      if((ob = array[i])->short() &&
         ob->armour_class())
        obj_array[cnt++] = ob;
    two_ret_values[0] = cnt;
    two_ret_values[1] = obj_array;
    return two_ret_values;
  }
  
  if(str == "all weapons" || str == "weapons"){
    for(i = cnt = 0; i < size; i ++)
      if((ob = array[i])->short() &&
         ob->weapon_class())
        obj_array[cnt++] = ob;
    two_ret_values[0] = cnt;
    two_ret_values[1] = obj_array;
    return two_ret_values;
  }
  
  return two_ret_values;
}

string object_list_to_string(object *array, int array_size) {
  int i, j, n;
  mixed *finite_array, *nice_array;
  string accum, tmp;

  accum = "";
  finite_array = allocate(array_size);
  for( i = 0; i < array_size; i++) {
    finite_array[i] = array[i];
  nice_array = unique_array(finite_array, "short");
  n = sizeof(nice_array);
  for(i = 0; i < n; i++ )
    if(i != 0)
      if(i == n - 1)
        accum += " and ";
      else accum += ", ";
    j = sizeof(nice_array[i]);
    if(j == 1)
      accum += nice_array[i][0]->short();
    else {
      accum += numbers[j] + " ";
      if(tmp = nice_array[i][0]->query_plural())
        accum += tmp;
      else
        accum += plural(nice_array[i][0]->short());
    }
  }
  return accum;
}

string plural(string str) {
  string tmp;
  mixed s;
  int ss;

  s = (('s' == s = str[strlen(str) - 1]) || s == 'z' || s == 'h') ?
    "es" : "s";
  return strip_article(str) + s;
}

string strip_article(string str) {
  string tmp;
  if(sscanf(str, "a %s", tmp))
    return tmp;
  if(sscanf(str, "an %s", tmp))
    return tmp;
  if(sscanf(str, "the %s", tmp))
    return tmp;
  return str;
}

#define MONEY "/obj/money"

money_string(g,s,c){
   string cash;
   int tmp;
   tmp =  g*GOLD;
   tmp += s*SILVER;
   tmp += c*COPPER;
   if(tmp)
   cash = (string)call_other(MONEY, "convert", tmp);
   if(!cash) return "";
   return cash;

}

