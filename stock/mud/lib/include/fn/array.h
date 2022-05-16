// this isnt really used but still needed, all npcs and players
// belong to class 'all' (Ash)


static string *classes;            /* list of classes */
string *set_classes(string *arr) { return classes = (arr) ? arr : ({}); }
string *query_classes()          { return classes;  }


status query_class(string str) {
    if(!classes) classes = ({});
    return (member_array(str, classes) == -1) ? 0 : 1;
}

void add_class(string str){
    if(!query_class(str) && str) classes += ({ str, });
}

void remove_class(string str){
    int i;
    if((i = member_array(str, classes)) != -1) {
        classes = classes[0..i-1]+classes[i+1..sizeof(classes)-1];
    }
}


