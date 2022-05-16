#define TP this_player()
#define VALID "secure/valid_d"

status main(string skill,string spell){
  int cost,sk,msk;
//  if(spell) skill += spell;
  write("Result:"+skill+"\n");
  if(skill && (VALID->is_valid_skill(skill) ||
               VALID->is_valid_spell(skill)))
  {
    sk = TP->query_skill(skill);
    cost = (sk*sk)*3;
    if(sk > 89)
    { 
      write("Your skill is maxed in that.\n");
      return 1;
    }
    if(sk+(sk/10) >= TP->query_level())
    { 
      write("Your skill is max trained for your level.\n"); 
      return 1;
    }
    if(cost > TP->query_money())
    {
     write("You dont have "+cost+" coins.\n");
     return 1;
    }
     TP->adj_money(-cost);
   write("You train "+skill+" from "+sk+" to "+(sk+1)+" for "+cost+" coins!\n");
     TP->adj_skill(skill,1);
     return 1;
   }
   write("Invalid Skill\n");
   return 1; 
}
