#include <mudlib.h>
#include <ansi.h>

#define HOUSED "/cmds/std/house"
#define TO this_object()

int rank;
string spouse, house, title, pretitle;
static string staticpretitle;

string who_string, color, race,alignment,
       class, avatar,profession,magic,faith;

static mapping classes;

string set_cc(string cc);
string query_who(string x);

void clear_who_string() { who_string = 0; this_object()->query_who(); }
void init_static_link(){ staticpretitle = 0;
                          this_object()->query_pretitle();
                          this_object()->clear_who_string();
                          }


string set_class(string x)     { class = x;      return x; }
string set_race(string x)      { race = x;       return x; }
string set_magic(string x)     { magic = x;      return x; }
string set_avatar(string x)    { avatar = x;     return x; }
string set_alignment(string x) { alignment = x;  return x; }
string set_faith(string x)     { faith = x;      return x; }
string set_title(string x)     { title = x;      return x; }
string set_pretitle(string x)  { pretitle = x;   return x; }
string set_profession(string x){ profession = x; return x; }

string query_alignment()   {
       if(!alignment) return "neutral";
        return alignment;  }
string query_race()        {
        return race;       }
string query_avatar()        {
        return avatar;       }
string query_profession()  {
        if(!profession || profession == "") profession = "adventurer";
        return profession; }
string query_class() {
        if(!class) return query_profession();
        return class; 
}
string query_title() {
        if(!title) title = "";
        return title; 
}
string query_pretitle() {
        if(!pretitle)  pretitle = "";
        return pretitle; 
}
string query_magic() {
        if(!magic) magic = "magic";
        return magic; 
}
string query_faith() {
        if(!faith) faith = "faith";
        return faith; 
}
          


string query_house() {
        if(!house || house == "") return "commoner";
  if(!HOUSED->query_member(house,this_object()->query_real_name())) 
        house = "commoner";
  if(this_object()->query_level() < 1 && house != "commoner"){
      HOUSED->remove_member(house,this_object()->query_real_name());
        house = "commoner";
        }
        return house; 
}

string query_color() {
       if(color == "w") return NORM+WHITE_F;
       if(color == "bw") return B_WHITE_F;
       if(color == "g") return NORM+GREEN_F;
       if(color == "bg") return B_GREEN_F;
       if(color == "r") return NORM+RED_F;
       if(color == "br") return B_RED_F;
       if(color == "c") return NORM+CYAN_F;
       if(color == "bc") return B_CYAN_F;
       if(color == "m") return MAGENTA_F;
       if(color == "bm") return B_MAGENTA_F;
       if(color == "b") return NORM+BLUE_F;
       if(color == "bb") return B_BLUE_F;
       if(color == "y") return BROWN_F;
       if(color == "by") return YELLOW_F;
       if(color == "bl") return NORM+GREY_F;
       return NORM; 
}

string init_link(string str){
  int r;
  if(!HOUSED->query_member(house,TO->query_real_name())) house = "commoner";
  if(house != "commoner") TO->set_channel(house,1);
/*
  if(TO->query_level() > 10)  TO->set_channel(alignment,1);
*/
                                           r = 1; // starting
  if(TO->query_level() > 19)               r = 2; 
  if(TO->query_level() > 39)               r = 3; 
  if(house != "commoner")                  r = 4;
  if(house != "commoner" && TO->query_wp())r = 5;
  if(r == 5 && TO->query_wp() > 1)         r = 6;
  if(r == 6 && TO->query_wp() > 3)         r = 7;
  if(r == 7 && TO->query_wp() > 5)         r = 8;
  if(r == 8 && TO->query_wp() > 7)         r = 9;
  if(TO->query_security_level())           r = 9;
  if(TO->query_security_level() >  101)    r = 10;
  if(TO->query_gender() == 2){
     if(r == 1) pretitle = "Lass";
     if(r == 2) pretitle = "Maiden";
     if(r == 3) pretitle = "Amazon";
     if(r == 4) pretitle = "Noblewoman";
     if(r == 5) pretitle = "Lady"; 
     if(r == 6) pretitle = "Baroness"; 
     if(r == 7) pretitle = "Duchess"; 
     if(r == 8) pretitle = "Countess"; 
     if(r == 9) pretitle = "Princess"; 
     if(r ==10) pretitle = "Queen"; 
  }
  if(TO->query_gender() == 1){
     if(r == 1) pretitle = "Lad";
     if(r == 2) pretitle = "Serf";
     if(r == 3) pretitle = "Barbarian";
     if(r == 4) pretitle = "Nobleman";
     if(r == 5) pretitle = "Lord"; 
     if(r == 6) pretitle = "Baron"; 
     if(r == 7) pretitle = "Duke"; 
     if(r == 8) pretitle = "Count"; 
     if(r == 9) pretitle = "Prince"; 
     if(r ==10) pretitle = "King"; 
  }

  this_object()->update_player_ranking();
  this_object()->update_player_ranking(1);
  this_object()->message("online");
  this_object()->update_player_ranking();
  this_object()->init_class();
  this_object()->init_stats(query_race(),query_profession());

  

  this_object()->init_static_link();
  return pretitle;
}


int message(string nom) {
    string msg,chan;
    msg = sprintf("%s from %s [%d]",this_object()->query_real_name(),
            this_object()->query_ip(), this_object()->query_level());
        if(msg && nom) {
            chan = "monitor";
            catch("/secure/channeld/channeld"->
            tell_channel(chan,nom,msg));
       }
   return 1;
}



string query_player_info(){
 string str,pre,c,n;
 c = B_CYAN_F;
 n = NORM+CYAN_F;
 str = "";
 pre = "";
 pre += n+"You are "+c+this_object()->short()+".\n";
 if(spouse && spouse != "")
            str += n+"You are married to "+c+capitalize(spouse)+n+".\n";
 if(house) str += n+"You belong to "+c+"House "+capitalize(house)+n+".\n";
 if(spouse && spouse != "" && house)
     str = n+"You are married to "+c+capitalize(spouse)+n+
           n+" and belong to "+c+"House "+capitalize(house)+n+".\n";

 return pre+str;  
 }

string query_link(){ 
  if(this_object()->query_attrib("link"))
     return (string)this_object()->query_attrib("link");
}
string set_link(string str){
   if(!str || str == "remove") {  
     this_object()->set_attrib("link",0);
     return "removed"; }
     str = lower_case(str);
     this_object()->set_attrib("link",str);
     return (string)this_object()->query_attrib("link");
   }

string query_spouse(){ 
  if(spouse) return spouse;
}

string set_spouse(string str){
   if(!str || str == "remove"){ spouse = ""; return "removed"; }
     str = lower_case(str);
     spouse = str;
     return spouse;
}


string set_house(string str) {
   string remove;
   if(!str || str == "remove") {
       house = "commoner";
   } else {
       house = str;
   }
   this_object()->set_channel(house,1);
   this_object()->clear_static_link();
   return (string) house;
}


string query_who(string brief){
   string wi,x;
   if(who_string == "Logon") who_string = 0;
   if(who_string) return who_string;
   wi = "";
   wi += this_object()->do_cc();
   if(pretitle && pretitle != "") wi += capitalize(query_pretitle())+" ";
   wi += this_object()->query_name();
   if(title && title != "") wi += " " + capitalize(query_title());
   wi += NORM;
   who_string = wi;
   return wi;
}


