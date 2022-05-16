
status main(string str) {
    object *play;
    int i,x;
    if(!str) {
	write("slam what?\n");
	return 1;
    }
    play=users();
    for(i=0;i<sizeof(play);i++) {
          for(x=0;x<5;x++)
	    tell_object(play[i],str+"\n");
    }
    return 1;
}

void help() {
    write("useage: slam <whatever>\n");
    return;
}
