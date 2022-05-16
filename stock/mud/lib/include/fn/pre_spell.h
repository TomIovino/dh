// a little function to uniform spell damage.. . Ash - March 9, 1996

int pre_spell(string stat, int level, int cost){
  int damage;
  int sint;
  int failed;
  sint = this_player()->query_stat(stat);
  if(random(level) < random(sint)) failed = 1;
  if(level * 2 > sint) failed = 1;
  if(sint * 2 > level) failed = 0;
  if(failed){
     return 1; // total failure
            }
     damage = level + sint + cost; 
     if(damage > level*3) damage = level*3;
     if(damage > sint*3) damage = sint*3;
     if(damage > cost*3) damage = cost*3;
     damage = (damage/2) + random(damage/2);
return damage;
 }
