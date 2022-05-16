#ifndef COMPARE_H
#define COMPARE_H

/* compare two stats between multible objects */

object *compare(string my_stat, string their_stat, object *compare) { 
   if(!my_stat || !their_stat) return ({}); 
   if(!compare) compare = all_inventory(environment());
   return filter_array(compare,"comp",this_object(),
   ({my_stat,their_stat,}));
}

static status comp(object ob, string *stat) {
  int my_stat, their_stat;
  if(!ob || ob == this_player()) return 0;
  my_stat = (int)call_other(this_player(), "query_"+stat[0]) + 1; 
  their_stat = (int)call_other(ob, "query_"+stat[1]) + 1;
  if(my_stat < their_stat) return 1;
  return 0;
} 

#endif /* end COMPARE_H */
