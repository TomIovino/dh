// ash 2001

object target(string what, mapping flags)
       {
       object find;
       if(!mappingp(flags)) flags = ([]);
       if(!what) find = environment(this_player());
       if(!find && flags["inv"])
        find=present(what,this_player());
       if(!find && flags["env"])
        find=present(what,environment(this_player()));
       if(flags["debug"])
       tell_object(this_player(), "find = "+(string)find+"\n");
     return find;
}
