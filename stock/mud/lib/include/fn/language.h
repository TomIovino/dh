// Dragonheart - Ash - Jan 1998

static string *languages_known;
static string speak_language;


string query_speak_language()    {
    return (speak_language) ? speak_language : "common";
}
status query_language(string str){
    if(this_player() && this_object()->query_security_level()) return 1;
    return (member_array(str, languages_known) == -1) ? 0 : 1;
}
void add_language(string str){
    if(str) languages_known += ({ str, });
}
void remove_language(string str){
    int i;
    if((i = member_array(str, languages_known)) != -1 && str != "common")
{
        languages_known = languages_known[0..i-1]
        + languages_known[i+1..sizeof(languages_known)-1];
    }
}


string *query_languages_known() {
          languages_known = ({ "common", });
     if(TP->query_race()) languages_known += ({ TP->query_race(),});
     if(TP->query_al_title()) languages_known += ({TP->query_al_title(),});
              return languages_known;
  }

string *set_languages_known(string *arr) {
    return languages_known = (arr) ? arr : ({});
}

status change_language(string str) {
    if(str == "common" || query_language(str)) {
        write("You begin to speak in "+ str +".\n");
        speak_language = str;
        return 1;
    }
    write("You don't know the language "+ str +".\n");
    return 1;
}

status set_speak_language(string str) {
    speak_language = (query_language(str)) ?  speak_language : "common";
    return (str == speak_language) ? 1 : 0;
}

