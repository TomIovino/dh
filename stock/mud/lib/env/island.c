inherit "env/env";

void reset(status arg) 
{
  if(arg) return;
  set_data("short","floating island");
  set_data("long",
   "A small floating island in the middle of time and space. A stunning\n"
   "waterfall flows off a rocky cliff that creates a pool in the center\n" 
   "of the island, which is surrounded by a lush field of grass. In the\n"
   "northern section is a grove of trees which contain fruits and nuts\n"
   "of every variety. In the south is a large berry patch and to the\n"
   "west is a garden of fruits and vegetables. The sunlight seems to\n"
   "come from everywhere although no sun can be seen. A huge marble\n"
   "tower stands in the middle of the this floating island\n");
  set_data("exits", ([ "up":"/env/tower" ]) );
}     
