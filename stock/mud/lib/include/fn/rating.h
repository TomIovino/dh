int query_defense(){
    int x;
    x = TP->query_ac();
    x += TP->query_class("defense")/10;
    if(TP->query_tactic() == "defensive")
    x += 20;
    return x;
}
string query_defensive_rating(){
    string str;
    int x;
    x = TP->query_defense();
    if(x > 230)
        str =(    "godly    ");
      else if(x > 220)
        str =(    "divine    ");
      else if(x > 200)
        str =(    "superb    ");
      else if(x > 180)
        str =(    "amazing   ");
      else if(x > 160)
        str =(    "incredible");
      else if(x > 140)
        str =(    "excellent ");
      else if(x > 110)
        str =(    "great     ");
      else if(x > 80)
        str =(    "good      ");
      else if(x > 60)
        str =(    "average   ");
      else if(x > 40)
        str =(   "decent    ");
      else if(x > 20)
        str =(   "poor      ");
      else
        str =(   "bad       "); 
        str = str;
       return str;
  }
int query_offensive(){
     string str;
     int x;
      x = TP->query_wc();
      x += TP->query_stat("fight")/20;
      x += TP->query_class("offense")/20;
    if(TP->query_tactic() == "offensive")
      x += 5;
     return x;
}

string query_offensive_rating(){
     string str;
     int x;
      x = TP->query_offensive();
      if(x > 45)
        str =(    "godly     ");
      else if(x > 43)
        str =(    "divine    ");
      else if(x > 40)
        str =(    "superb    ");
      else if(x > 38)
        str =(    "amazing   ");
      else if(x > 36)
        str =(    "incredible");
      else if(x > 33)
        str =(    "excellent ");
      else if(x > 30)
        str =(    "great     ");
      else if(x > 27)
        str =(    "good      ");
      else if(x > 25)
        str =(    "average   ");
      else if(x > 21)
        str =(    "decent    ");
      else if(x > 16)
        str =(    "poor      ");
      else
        str =(    "bad       "); 
        str = str;
   return str;

}
 
int query_stealth(){
     int x;
     x = TP->query_stat("dexterity");
     x += TP->query_class("stealth")/10;
     if(TP->query_tactic() == "stealth")
     x += 30;
     return x;
}

 string query_stealth_rating(){
     string str;
     int x;
     x = TP->query_stealth();
    if(x > 115)
        str =(      "godly     ");
      else if(x > 105)
        str =(      "divine    ");
      else if(x > 100)
        str =(      "superb    ");
      else if(x > 95)
        str =(      "amazing   ");
      else if(x > 90)
        str =(      "incredible");
      else if(x > 85)
        str =(      "excellent ");
      else if(x > 80)
        str =(      "great     ");
      else if(x > 75)
        str =(      "good      ");
      else if(x > 70)
        str =(      "average   ");
      else if(x > 60)
        str =(      "decent    ");
      else if(x > 50)
        str =(      "poor      ");
      else
        str =(      "bad       "); 
        str = str;
   return str;

}
int query_casting(){
     string str;
     int x;
       x = TP->query_stat("willpower");
       x += TP->query_stat("willpower");
       x += TP->query_stat("intelligence");
       x += TP->query_stat("wisdom");
       x += TP->query_class("cleric");
       x += TP->query_class("magician");
       x = x/10;
     if(TP->query_tactic() == "casting")
       x += 11;
    return x;
}

string query_casting_rating(){
     string str;
     int x;
      x = TP->query_casting();
      if(x > 65)
        str =(     "godly     ");
      else if(x > 60)
        str =(     "divine    ");
      else if(x > 55)
        str =(     "superb    ");
      else if(x > 50) 
        str =(     "amazing   ");
      else if(x > 45)
        str =(     "incredible");
      else if(x > 40)
        str =(     "excellent ");
      else if(x > 35)
        str =(     "great     ");
      else if(x > 30)
        str =(     "good      ");
      else if(x > 25)
        str =(     "average   ");
      else if(x > 20)
        str =(     "decent    ");
      else if(x > 15)
        str =(     "poor      ");
      else
        str =(     "bad       "); 
        str = str;
   return str;
}

