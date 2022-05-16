 
inherit "inherit/treasure";
 
reset (arg){
    if(arg) return;
 
    set_weight(1);
    set_value(79);
    set_name("mirror");
    set_short("small mirror");
    set_long("\
This small mirror fragment is large enough that you can see your \n\
reflection in it.\n");
}

