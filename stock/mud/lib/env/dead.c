inherit "new/obj";

void reset(status arg) 
{
  if(arg) return;
  set_data("name","void");
  set_data("short","The Plane of Non Existance");
  set_data("long",
    "Aparently You nolonger exist therefore you must make a choice:\n"+
    "[A]> Rebirth into your old form with a reduction of power.\n"+
    "[B]> Saccerfice your former powers to be reborn in a new form.\n"+
    "[C]> Fade away from the world.\n"+
    "\n"); 
}     
