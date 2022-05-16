

// NOTE: set_light will control the light in the room.
//       0 - no light   1 - day/night   2 - light

#include <mudtime.h>

int daylight_adj;             /* current light adj, 1 == day, -1 == night */

void set_light(int i);
void fix_day();
status query_day();

void set_light(int i) {
  if(!i) this_object()->adj_light(i);
  if(i == 1) fix_day();
  else this_object()->adj_light(i);
}

void fix_day() { /* switches between night & day */
  int day;

  day = (int)MUDTIMED->query_light_adjustment();
  if((day < 0 && daylight_adj > 0) || (day > 0 && daylight_adj < 0))
    this_object()->adj_light(day);
  daylight_adj = day;
}


string query_time() {
  return((string)MUDTIMED->query_mud_time()+"");
}


status query_day() {
  if(!daylight_adj) fix_day();
  return (daylight_adj == 1);
}
