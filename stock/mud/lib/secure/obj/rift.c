
init(){
    write("Something breaks and turns to ashes.\n");
    call_out("dest",1);
  }
dest(){
    destruct(this_object());
  }

