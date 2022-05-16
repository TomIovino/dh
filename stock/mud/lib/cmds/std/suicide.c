

int main(){
   if((status)environment(this_player())->query_no_fight() == 1){
     if(this_player()->query_level() > 1){
     write("iiiiiiiaaaaaaaaaaaaaaeeeeeeeeeeooooooooo\n");
     this_player()->death();
     return 1;
     }
     write("Your as good as dead now.\n");
     return 1;
   }
     write("goto church to suicide.\n");
     return 1;
}

