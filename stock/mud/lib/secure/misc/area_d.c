#define TP this_player()


static  mapping exp = ([ ]);
static  mapping time = ([ ]);
static  mapping money = ([ ]);


varargs up_exp(mixed area, mixed setting){
       if(setting > 1) 
       exp[area] += setting;
       return setting;
}
varargs up_money(mixed area, mixed setting){
       if(setting > 1) 
       money[area] += setting;
       return setting;
}
varargs up_time(mixed area, mixed setting){
       if(setting > 1) 
       time[area] += setting;
       return setting;
}

varargs mixed query_exp(mixed what) {
    if(what) return exp[what];
    return copy_mapping(exp);
}
varargs mixed query_money(mixed what) {
    if(what) return money[what];
    return copy_mapping(money);
}
varargs mixed query_time(mixed what) {
    if(what) return time[what];
    return copy_mapping(time);
}


update(){
    string past,now,room;
    string x,y;
    int adj_exp,adj_time,adj_money;
    past = TP->query_data("past_env");
    room = TP->query_current_room();
    if(sscanf(room, "d/%s/%s", x,y) == 2)
    now = x;
    if(!past){
       past = "void";
     TP->set_temp_data("past_env",now);
     TP->set_temp_data("past_exp",TP->query_level());
     TP->set_temp_data("past_time",time());
     TP->set_temp_data("past_money",TP->query_money()+TP->query_savings());
    }
    if(!now){
    if(sscanf(room, "w/%s/%s", x,y) == 2)
    now = x;
    }
    if(now == past) return; // SAME DOMAIN SO END FUNC
//  MACRO IT!
    adj_exp   = TP->query_level() - TP->query_data("past_exp");
    adj_time  = time() - TP->query_data("past_time");
    adj_money = TP->query_money()+TP->query_savings() - 
                          TP->query_data("past_money");
//  ADJ AREA DATA
    up_exp(past,adj_exp);
    up_time(past,adj_time);
    up_money(past,adj_money);
//  RESET DATA in player object
    TP->set_temp_data("past_env",now);
    TP->set_temp_data("past_exp",TP->query_level());
    TP->set_temp_data("past_time",time());
    TP->set_temp_data("past_money",TP->query_money()+TP->query_savings());

}
