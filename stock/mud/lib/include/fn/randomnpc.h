

void randomnpc(){
 int r;
  r = random(500);
  if(!random(5)){
   if(r > 498)
     this_object()->add_object("dragon","g/npc/dragon/"+THIS_ENV);
     else if(r > 498)
     this_object()->add_object("hero","g/npc/hero/"+THIS_ENV);
     else if(r > 496)
     this_object()->add_object("fanatic","g/npc/"+THIS_ENV+"/fanatic");
     else if(r > 446)
     this_object()->add_object("scout","g/npc/common/scout");
     else if(r > 396)
     this_object()->add_object("peon","g/npc/common/peon");
     else if(r > 346)
     this_object()->add_object("warrior","g/npc/"+THIS_ENV+"/warrior");
     else if(r > 296)
     this_object()->add_object("rogue","g/npc/"+THIS_ENV+"/rogue");
     else if(r > 244)
     this_object()->add_object("magician","g/npc/"+THIS_ENV+"/magician");
     else if(r > 194)
     this_object()->add_object("cleric","g/npc/"+THIS_ENV+"/cleric");
     else if(r > 144)
     this_object()->add_object("scout","g/npc/"+THIS_ENV+"/scout");
     else if(r > 96)
     this_object()->add_object("peon","g/npc/common/peon");
     else
     this_object()->add_object("scout","g/npc/common/scout");
  }
}
