inherit "env/env";

void reset(status arg) 
{
  if(arg) return;
  set_data("short","grand marble tower");
  set_data("long",
"A large circular chamber made of smooth and solid marble blocks.\n"
"Windows covered with colored glass mark each of the primary and\n"
"secondary compass directions. The chamber itself is divided into\n"
"four sections. The western section is made up a long wooden desk\n"
"around which are a few padded leather chairs. The southern section\n"
"has a couch and three large comfortable chairs are positioned in\n"
"front of the window that appears to overlook the city of Twilight\n"
"in the distance. The eastern chamber is made up of a table with\n"
"four chairs around it and cabinets along the wall. The northern\n"
"chamber contains a large wooden framed bed with large headboards\n"
"on both ends and cabinets on each side. Central to the chamber\n"
"is a large roaring fireplace.\n");

  set_data("exits", ([ "down":"/env/island" ]) );

}     
