
#include <mudlib.h>

int main(){
  mapping one,two;
  string file,name;
  int respawn;
  
  file = "/d/world/x/4";
  name = "harry";
  respawn = 900;
  
  two = ([file:respawn]);
  one = ([name:two]);
  
  write(""+one[name][file]+"\n");
  return 1;
}

900

#include <mudlib.h>

int main(){
  mapping map;
  string file,name;
  string *keys;
  int respawn,i;
  
  map = ([]);
  
  file = "/w/nof/bill";
  name = "bill";
  respawn = 900;
  
  map += ([ name : file; ({ respawn,time() }) ]);
  keys = m_indices(map);
  
  write(keys[0]+"\n");        //name
  write(map[name]+"\n");      //file
  write(map[name,1][0]+"\n"); //respawn
  write(map[name,1][1]+"\n"); //time()
  write(map[keys[0],1][1]+"\n");
  
  if(!member(map,"bob")) write("1\n");
  if(member(map,"bill")) write("2\n");
  return 1;
}


bill
/w/nof/bill
900
1419535366
1419535366
1
2