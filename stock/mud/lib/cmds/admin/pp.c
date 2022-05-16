 
status main(string arg) {
  object *i; 
  string s;
  int j,k,inv;
  string t;
  s="\n";
  i= users();
  i= sort_array(i, lambda( ({'x, 'y}), ({#'<,
             ({#'call_other, 'x, "query_security"}),
             ({#'call_other, 'y, "query_security_level"}) }) ) );
  for(j=0,k=sizeof(i);j<k;j++) {
    inv=0;
    if(i[j]->query_invis()){
      if(this_player()->query_security_level() < i[j]->query_security_level())
        continue;  
      else inv=1;
  }
   s+=sprintf("%s\n",
   sprintf("%11s  %3d  %2d %18s %-30s",
                          (i[j]->query_real_name()),
                          (i[j]->query_security_level()),
                          (i[j]->query_level()),
                          query_ip_name(i[j]),
                          (i[j]->query_current_room())));
  }
  write(s);
  return 1;
}
 
 

