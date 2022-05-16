 
int main(string arg) {
    string name;
  if(!arg){ 
   write("clear <aliases/description/plan>\n");
     return 1;
}
  if(arg == "plan"){
    this_player()->set_plan("");
    write("plan cleared\n");
    return 1;
}
  if(arg == "aliases"){
   this_player()->clear_aliases();
   write("cleared aliases\n");
   return 1;
}
  if(arg == "description"){
    this_player()->set_long("");
   write("cleared description\n");
   return 1;
}
   else{
   write("clear [aliases/description]\n");
   return 1;
    }
    return 1;
}
 
