status id(string str) { return str=="TheGag"; }
status drop() { return 1; }
status get() { return 1; }
string short() {
    object e;
    if(e = environment())
        return sprintf("A gag designed especially for %s",
                       capitalize(e->query_real_name()));
    return "A gag without an owner";
}
 
void reset(int arg) {}
void init() {
    object env;
    if(this_player() != (env = environment())) return;
    if(env) 
        tell_object(env,"You have been gagged.\n");
    add_action("Verb_Gag","",1);
} 
 
status Verb_Gag(string arg) {
    string v;
    switch(v = query_verb()) {
        case "nogag":                /* So we can ungag ourselves =) */
            destruct(this_object());
            return 1;
 
        case "quit":
            printf("Good riddens!\n");
            /* Fall Through */
 
        case "look":      /* These are commands that gagged players keep */
        case "l":
        case "exa":
        case "who":
        case "say" :
            return 0;
 
        default:
           return printf("What do you mean %O ?\n",v),1;
    }
}
