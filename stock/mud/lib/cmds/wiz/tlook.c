#include <ansi.h>
#include <ansi_filter.h>
 
string pluralize(string s);
 
closure cf;
string f_ansi(string s);
 
status main(string s) {
  
  string *sh, sht,header;
  object *inv, ob;
  int i, len, mon;
  status wiz,wizsight;
  mapping inventory;
  
  if(s && s[0] == '1') { cf = #'filter_ansi; }
  if(s && s[0] == '2') { cf = #'f_ansi; }
 
  inventory = ([ ]);
 
  ob = environment(this_player());
  wiz = (this_player()->query_security_level()) ? 1 : 0;
  wizsight = this_player()->query_wizsight();
  header = YELLOW_F+((string)ob->query_object_type() == "Room")
       ? ""
       : (!ob->query_name())
       ? "Inventory: "
       : (!living(ob))
       ? "The "+ (string)ob->query_name() +" contains: "
       : (ob == this_player()) 
      ? "You are carrying:\n\n"
       : capitalize((string)ob->query_name())+" is carrying:\n\n" + NORM;
 
  len = strlen(header);
  inv = all_inventory(ob);
  i = sizeof(inv);
  while(i--) {
     if(!inv[i]
     || inv[i] == this_player()
     || inv[i]->query_worn()
     || inv[i]->query_wielded()) continue;
     sht = (string)inv[i]->short(wiz);
     if(!inv[i]) continue;
     inv[i]->extra_long();
     if(sht=="" && wiz && wizsight) sht = "Nul Short->"+ file_name(inv[i]);
     else if(!sht && wiz && wizsight) sht = "Invisible->"+ file_name(inv[i]);
     if(!sht) continue;
     inventory[sht] += 1;
  }
  ob->exits();  /* between extra-long and inventory */
  i = sizeof(sh = m_indices(inventory));
  while(i--)
    header += sprintf("%s%-72s\n",inventory[sh[i]]>1?sprintf("%s%2d%s ",B_GREEN_F,inventory[sh[i]],NORM)
                                          : "",inventory[sh[i]]>1?pluralize(sh[i]):sh[i]);
  write(cf ? funcall(cf,header) : header);
  return 1;
}
 
#define DEBUG(St,Ob) printf("%s = %-=*O\n",St,70-strlen(St),Ob)
 
status not_ansi_code(string s) { return s[0] != 27; }
 
string f_ansi(string s) {
    string *c;
    c = regexplode(s,sprintf("%c\[[0-9;]*m",27));
    c -= ({ "" });
    c = filter_array(c,#'not_ansi_code);
    c -= ({ "" });
    return implode(c,"");
}
 
 
string _plural(string word) {
    int size;
    // *elf -> *elves
    if(word[<1] == 'f') { 
        word[<1] = 'v';
        return word + "es";
    }
    if(word[<1] == 'y') { 
        word[<1] = 'i';
        return word + "es";
    }
    if(word[<1] == 's') 
        return word + "es";
    return word + "s";    
}
 
string pluralize(string s) {
    string *parts,*t,a,b,*c;
    status stripped;
    int ptr,size;
    if(!s || s == "") return "";
    parts = explode(s," ");
//DEBUG("parts ",parts);
    c = regexplode(s,sprintf("%c\[[0-9;]*m",27));
    c -= ({ "" });
//DEBUG("colors",c);
    if(sizeof(t)) {
       if(sizeof(t) > 1) {
           parts[<1] = t[0];
           parts += ({ implode(t[1..],NORM) });
       } else {
           parts[<1] += t[0];
           parts += ({ NORM });
       }
    }
    size = strlen(parts[0]);
    if(parts[0][<1] == 'a') { stripped++; parts[0] = parts[0][0..<2]; }
    else if(size >= 2 
         && parts[0][<2] == 'a' 
         && parts[0][<1] == 'n') { stripped++; parts[0] = parts[0][0..<3]; }
    if(stripped && (sizeof(parts) > 1) ) {
       parts[1] = parts[0] + parts[1];
       parts = parts[1..];
    }
    ptr = member_array("of",parts);
    if(ptr != -1) {
        ptr--;
        parts[ptr] = _plural(parts[ptr]);
    } else {
        parts[<1] = _plural(parts[<1]);
    }
    return implode(parts," ");
}
 
