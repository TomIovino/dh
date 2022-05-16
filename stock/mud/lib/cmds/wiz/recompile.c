// Recompile Command v1.1
// Geewhiz@Dreamtime, Geewhiz@DragonHeart, Geewhiz@DoD
// Copyright 1995, 1996 Andrew T. Graham ("Geewhiz")
 
#define FAIL(X) return notify_fail(X),0
 
string *query_cmd_aliases() { return ({ "rc", "compile" }); }
 
varargs object compile_object(string s, status t);
int recompile_(string file);
 
status main(string file) {
   object what;
   string *il, c;
   int i, err, d;
 
  
   if(file && sscanf(file,"-d %s",file)==1) d++;
   if(!file) {
       file = "/obj/wizard.c";
      printf("%s:  Syntax: %s [-d] <filename>\n",query_verb(),
                 query_verb());
       printf("Filename defaulting to '/obj/wizard.c'\n");
    }
   sscanf(file,"%s.c",file);
   file = (string)this_player()->make_path(file);
   if(this_player()->bad_file(file))
       FAIL(sprintf("%s: Bad Filename '%s'.\n",query_verb(),file));
   what = compile_object(file);
   if(!what) FAIL("recompile: Failed:  Object could not be loaded\n");
   err = 0;
   if(!d) {
      err = recompile_(file);
   } else {
      i = sizeof(il = inherit_list(what));
      while(i--) {   
        err += recompile_(il[i]);
      }
   }
   printf("There were %s errors. (%s)\n",(err ? to_string(err) : "no"),
              explode(query_load_average(),", ")[1]);
   return 1;
}
 
int recompile_(string file) {
   object what;
   string *il, c;
   int i, err;
 
   err = 0;
   printf("Recompiling %s...\n",file);
   what = compile_object(file);
   if(!what) {
      printf("\t%-32s: Could not reload!\n", file);
      return 1;
   }
   i = sizeof(il = inherit_list(what));
   while(i--) {
      printf("\t%-32s: %s\n",il[i],((what = compile_object(il[i],1)) ? 
                                 "Compiled Successfully." :
                                 "Could not reload!"));
      if(!what) err++;
   }     
   return err;
}
 
varargs 
object compile_object(string file,status force_reload) {
   object ob;
   mixed c;
   if(!(ob = find_object(file))) {
      c = catch(call_other(file,"???"));
      ob = find_object(file);
   } else {
      if(force_reload) destruct(ob);
      c = catch(call_other(file,"???"));
      ob = find_object(file);
   }
   return ( ob ? ob : 0 );
}
