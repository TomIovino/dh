/* Ships, Nof August 2012 
 *
 * support for 2 ports only */
 
/* todo - 
   load/reload cmd for boats in case they screw up 
   */
   
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;

#define debug 0
#define DEBUG_WIZ "nof"

/**************************************************************************/
/* function prototypes
 */
void ship_leaves(string str);
void ship_arrives();
void ships_start();
void check_exits();
varargs status tell_area(string start_room, string message, string exclusion);

/**************************************************************************/
/* global variables 
 */
mapping beaches = ([]); 

string *ports;         

string port, port_file, ship_name, ship_desc, ship_captain, notify;          

int trip_length, timer, pre_leave, waiting, counter, counter2, wait_time, under_way;

/**************************************************************************/
/* sets/queries 
 */
mapping query_beaches() { return copy_mapping(beaches); }          /* islands in between ports, syntax "room file_name":"printed name";({start#,finish#}) */
mapping set_beaches(mapping map){ return beaches = map; }          /* start and finish #'s is the range corresponding to trip length the player */   
                                                                    /* automatically reverses on return trip */ 

int set_trip_length(int i) { return trip_length = i; }            /* length of trip, *2 gives time in seconds */
int query_trip_length() { return trip_length; }

int set_wait_time(int i) { return wait_time = i; }                /* length of time ship waits in ports, *2 gives time in seconds */
int query_wait_time() { return wait_time; }

string *set_ports(string *arr) { return ports = arr; }             /* set_ports({"port1","d/port/1","port2","d/port/2",}) , boat starts at first instance */
string *query_ports() { return ports; }                            /* note - no '/' before the room file_name or scope won't work */

string set_ship_name(string str) { return ship_name = str; }       /* name of ship printed in room */
string query_ship_name() { return ship_name; }

string set_ship_desc(string str) { return ship_desc = str; }       /* what it says when you look ship at port */
string query_ship_desc() { return ship_desc; }

string set_ship_captain(string str) { return ship_captain = str; } /* name only of captain of ship */
string query_ship_captain() { return ship_captain; }

/**************************************************************************/

void reset(status arg) {
  if(!wait_time) wait_time = 15;
  if(!trip_length) trip_length = 0;
  if(!ship_name) ship_name = "Backachakala Express";
  if(!ship_desc) ship_desc = "An odd looking ship";
  if(!ship_captain) ship_captain = "Nof";
  
  if(arg) return;
  ports = ({});
  set_no_clean(1);
  call_out("ships_start",0);
}

void ships_start(){
  if(sizeof(ports)){
    port = ports[0];
    notify = ports[2];
    port_file = ports[1];
    
    port_file->set_ship_info(query_ship_name()+" has dropped anchor here in the harbour.");
    port_file->add_exit(file_name(this_object()),"ship");
    port_file->add_item("ship#boat",query_ship_desc());
    
    this_object()->add_exit(port_file,"gangplank");
    this_object()->add_item("gangplank#plank","It leads off the boat towards the port of "+port+".");
  }
  
  waiting = 1;
  counter = 0;
  counter2 = 2;
  set_heart_beat(1);
  set_no_summon(1);
}

void heart_beat() {
  timer++;
  
  if(waiting) {
    if(timer > wait_time) {
      tell_area(file_name(this_object()),"Captain "+query_ship_captain()+" exclaims: All aboard!!!!!!!!!! Ship is leaving soon!!\n","gangplank");
      tell_area(port_file,"The "+query_ship_name()+" is preparing to set sail!.\n","ship");
        
      call_out("ship_leaves",10,notify);      
      timer = 0;
      waiting = 0;
      return;
    }
    return;
  }
  
  if(pre_leave) {
    if(timer > trip_length) { 
      ship_arrives();
      timer = 0;
      return;
    }
  }
  
  if(under_way)
    check_exits();
}

void check_exits(){
/* check to see if we are in range of an island
 * used with beaches mapping
 */
  string *keys;
  string exit;
  int i,low,high;

  keys = m_indices(beaches);
  exit = 0;

  for(i=0;i<sizeof(keys);i++){
    low  = beaches[keys[i],1][0];
    high = beaches[keys[i],1][1];
    
    if(notify == ports[0]){
      if(trip_length-timer > low  
      && trip_length-timer < high){
        exit = keys[i];
        break;
      }
    }
    
    if(notify == ports[2]){
      if(timer > low
      && timer < high){
        exit = keys[i];
        break;
      }
    }
  }
  
  if(exit){
    this_object()->add_exit(exit,"dive");
    
    if(random(2))
      tell_area(file_name(this_object()),"The boat is passing by "+beaches[exit]+". It looks as if you can 'dive' off\n"+
                "The starboard side of the main deck.\n"NORM,"dive");
    
    tell_room(exit,query_ship_name()+" is passing by.\n");
  }
  else
    remove_exit("dive");
}

string scope_ship(string room){
/* telescope function called with init() in a room.
 * will automatically display a range of strings depending how
 * close or far the ship is from port.
 */
  string str,str2,str3,exit;
  string *keys;
  int i;
  
  
  str  = "You peer thru the telescope lens and see...\n";
  str += GREY_F+"---------------------------------------------------------\n"+NORM;
  
  if(under_way){
    if(room == ports[1]){
      if(port_file == room){
        str2 = "The "+query_ship_info()+" is about to sail into the harbour.";
      
        if(trip_length-timer < trip_length*95/100)
          str2 = "You see the "+query_ship_info()+" close by."; 
        if(trip_length-timer < trip_length*90/100)
          str2 = "In the distance you see the "+query_ship_info();
        if(trip_length-timer < trip_length*80/100)
          str2 = "You see something far away resembling a ship.";
        if(trip_length-timer < trip_length*75/100)
          str2 = "You don't see any ships.";
      }
      else{
        if(trip_length-timer < trip_length*20/100)
          str2 = "You don't see any ships.";  
        if(trip_length-timer < trip_length*15/100)
          str2 = "You see something far away resembling a ship.";  
        if(trip_length-timer < trip_length*10/100)
          str2 = "In the distance you see the "+query_ship_info();
        if(trip_length-timer < trip_length*5/100)
          str2 = "You see the "+query_ship_info()+" close by.";       
      }
    }
    
    if(room == ports[3]){ 
      if(port_file == room){
        str2 = "The "+query_ship_info()+" is about to sail into the harbour.";
      
        if(trip_length-timer < trip_length*95/100)
          str2 = "You see the "+query_ship_info()+" close by."; 
        if(trip_length-timer < trip_length*90/100)
          str2 = "In the distance you see the "+query_ship_info();
        if(trip_length-timer < trip_length*80/100)
          str2 = "You see something far away resembling a ship.";
        if(trip_length-timer < trip_length*75/100)
          str2 = "You don't see any ships.";    
      }
      else{
        if(trip_length-timer < trip_length*20/100)
          str2 = "You don't see any ships.";  
        if(trip_length-timer < trip_length*15/100)
          str2 = "You see something far away resembling a ship.";  
        if(trip_length-timer < trip_length*10/100)
          str2 = "In the distance you see the "+query_ship_info();
        if(trip_length-timer < trip_length*5/100)
          str2 = "You see the "+query_ship_info()+" close by."; 
      }
    }
  }
  else{
    keys = room->query_exits();
    
    for(i=0;i<sizeof(keys);i++){
      if(keys[i] == "ship"){
        str2 = room->query_ship_info();
        break;
      }
    }
  }
  
  if(!str2) 
    str2 = "What ship.";
  
  str3 = GREY_F+"\n---------------------------------------------------------\n"+NORM;
  
  return str+str2+str3;
}

status swim_back(string room){
/* function to check if a player can swim back to the ship from an island
 * code is used with init() in the room you are calling from
 */
  string *exits;
  int i;
  
  exits = this_object()->query_exits();
  
  for(i=0;i<sizeof(exits);i++){
    if(exits[i] == room){
      return 1;
      break;
    }
  }
      
  return 0; 
}

varargs status tell_area(string start_room, string message, string exclusion){ /* pain in my ass */
/* tell an area of rooms a message branching out 3 deep in every direction
 *
 * start_room - is the room you are calling this from
 * message    - the message you want printed
 * exclusion  - an 'exit' name you want excluded from the branch i.e. excluding
 *              'ship' will stop any message from being sent to ship rooms
 *              support for 1 exclusion only at the moment but can add support
 *              for more if necessary
 */
  mixed *exits, *open, *open2, *rooms, *done;
  int i,roomchange,range;
  object room;
  string winner;
      
  range = 3;
  roomchange = 0;
  rooms = ({});
  open = ({});
  open2 = ({});
  done = ({});

  if(!start_room) 
    return 0;
        
  if(!exits = start_room->query_all_exits()) 
    return 0;
        
  for(i=0;i<sizeof(exits);i++){
    if(exits[i] == exclusion)  
      winner = exits[i-1];
      
    if(i%2==0)
      open += ({ exits[i], });
  }
  
  open -= ({ winner, });

  while(++roomchange < range){  
    while(sizeof(open)){
      room = open[0];
      
      if(member(rooms,room)==-1)
        rooms += ({ room, });
      
      open -= ({ room, }); 
      exits = room->query_all_exits();
      
      if(exits)
      for(i=0;i<sizeof(exits);i++){
        if(i%2==0)
          open2 += ({ exits[i], });
      }
    }
    open = open2;
    open2=({});
  }
  
  for(i=0;i<sizeof(rooms);i++){
    if(rooms[i][0..0] == "/")
      rooms[i] = rooms[i][1..];
    
    if(member(done,rooms[i])!=-1)
      continue;
      
    tell_room(rooms[i],message); /* tell rooms */
    done += ({ rooms[i], });
#if debug
  tell_object(find_player(DEBUG_WIZ),"Room - "+rooms[i]+"\n"+"Msg  - "+message+"\n");
#endif
  }  
  return 1;
}

void ship_leaves(string str) {
/* ship leaves a port
 */
  timer = 0;
  pre_leave = 1;
  
  tell_area(file_name(this_object()),capitalize(ship_captain)+" exclaims: Right then maties! Set sail for "+
      capitalize(str)+" harbour!\n","gangplank");
  
  if(port_file){
    port_file->remove_exit("ship");
    port_file->remove_item("ship#boat");
    port_file->unset_ship_info();
    tell_area(port_file,"The "+query_ship_name()+" sails out of the harbour...\n","ship");
  }
  
  this_object()->remove_exit("gangplank");
  this_object()->remove_item("gangplank#plank");
  
  tell_area(file_name(this_object()),"The anchor is pulled up onto the deck.\n","gangplank");
  tell_area(file_name(this_object()),"The gang plank is pulled in, and the ship sets sail.\n","gangplank");
  
  set_heart_beat(1);
  under_way = 1;
  timer   = 0;
  waiting = 0;
  return;
}

void ship_arrives() {
/* ship arrives at a port
 */
  timer = 0;
  counter += 2;
  counter2 += 2;
  
  if(counter > (sizeof(ports)-1)) 
    counter = 0;

  if(counter2 > (sizeof(ports)-1)) 
    counter2 = 0;
      
  port_file = ports[counter+1];
  
  tell_area(file_name(this_object()),"The ship finally reaches "+capitalize(notify)+".\n"+
      capitalize(ship_captain)+" says: Right then, we're here mates!\n"+
      "The gang plank is pulled out toward the dock.\n","gangplank");
  
  this_object()->add_exit(port_file,"gangplank");
  this_object()->add_item("gangplank#plank","It leads off the boat towards the port of "+port+".");
  
  port_file->set_ship_info(query_ship_name()+" has dropped anchor here in the harbour.");
  port_file->add_exit(file_name(this_object()),"ship");
  port_file->add_item("ship#boat",query_ship_desc());
  
  tell_area(port_file,"The "+query_ship_name()+" sails into the harbour...\n","ship");
  
  pre_leave = 0;
  under_way = 0;
  waiting = 1;
  notify = ports[counter2];
}

/**************************************************************************/