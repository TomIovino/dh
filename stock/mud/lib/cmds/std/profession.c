#define TP this_player()
#define VAL "secure/valid_d"

int main(string what){

     string now;
     now = TP->query_class();
     if(!what){ what = "unsure"; }

  //   if(now != "" &&  now != TP->query_profession())
     { 
        write("Attempting to set profession!\n");
        if(VAL->is_class(what) == what)
        {
          write("Your profession is now a "+what+"!\n");
          return 1;
        }

     }
     TP->set_class(TP->query_profession()); 
  return 1;
}

