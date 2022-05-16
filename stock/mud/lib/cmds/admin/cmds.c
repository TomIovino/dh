varargs
object get_object(string what, object where) {
    object ret;
    
    if(!what)
        return 0;
    if(what = "me")
        return this_player();

    if(what[0] == '='
    && ret = find_player(what[1..]))
        return ret;
    
    if(what[0] == '-'
    && ret = find_living(what[1..]))
        return ret;
    
    if(ret = find_player(what))
        return ret;

    if(ret = find_living(what))
        return ret;
    
    if(ret = present(what, where ? where : environment(this_player())))
        return ret;
    
    return 0;
}

varargs
int help(string arg) {
    printf("something\n");
    return 1;
}

int main(string arg) {
    object ob, obj;
    mixed *verbs;
    int i,m;
    mapping v_map, c_map;
    string cmd, fnc;
    
    
    object *o_a;
    string *c_a;
    
    int o_i, c_i;
    
    if(!arg)
        arg = "me";
        
    if(arg == "me")
        ob = this_player();
    else if(!ob = get_object(arg))
        return help();
    
    verbs = query_actions(ob, 1|8|16);
    
    m = sizeof(verbs);
    
    v_map = ([ ]);
    
    for(i = 0; i < m; i += 3) {
        cmd = verbs[i];
        obj = verbs[i+1];
        fnc = verbs[i+2];

        if(!v_map[obj])
            v_map[obj] = ([ ]);        
        v_map[obj] += ([ cmd : fnc ]);
    }
    
    o_i = sizeof(o_a = m_indices(v_map));
    while(o_i--) {
        printf("==%-'='75s==\n",to_string(o_a[o_i]));
        c_i = sizeof(c_a = m_indices(c_map = v_map[o_a[o_i]]));
        c_a = sort_array(c_a,#'>);
        while(c_i--) {
            printf("  %15s   %-15s\n",c_a[c_i] == "" ? "<*>" : c_a[c_i], c_map[c_a[c_i]]);
        }
    }
    
    return 1;
}

        
