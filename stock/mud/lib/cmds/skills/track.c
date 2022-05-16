
status main(){
       remove_call_out("track");
       call_out("track",3);
       return 1;
}
 
status track(string str, mixed alt_type) {
    object ob;
    int    track_time;
    string track_type;
    string track_dir;
    ob = this_player();
    if(ob->query_mp() < 25){
            write("You are too tired to look for tracks.\n");
            return 1;
    }
    ob->adj_mp(-25);
    ob->checkskill("track","primal",5);

    if(ob->query_skill("track") < random(100)){
            write("You are unable to make out any tracks here.\n");
            return 1;
    }
    if(environment(ob)->query_no_track()) {
            write("You are unable to make out any tracks here.\n");
	    return 1;
    }
    if(!environment(ob)->query_track_type()){
            write("You are unable to make out any tracks here.\n");
            return 1;
    }
    if(environment(ob)->query_track_type()){
       track_type = environment(ob)->query_track_type();
       track_time = environment(ob)->query_track_time();
       track_dir  = environment(ob)->query_track_dir();
      if(track_time+1800 < time()){
       write("You detect tracks but they are too faded for specifics.\n");
        return 1;
      }
      if(track_time+300 < time()){
       write("You detect some tracks heading "+track_dir+".\n");
        return 1;
      }
       write("You detect "+track_type+" tracks heading "+track_dir+".\n");
       return 1;
    }
 write("Your tracking skill seems to be broken. Contact a GOD.\n");
}


