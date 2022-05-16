
#include <ansi.h>
#define NAME "@@query_name:$this_player()$@@"

int query_no_save(){ return 1; }

status explorer(string spell_type, mixed alt_type) {
    int damage;

    damage = (int)this_player()->query_stat("focus")/2;

    this_player()->load_spell(({
	"target",           environment(this_player()),
	"name",             "explorer",
	"stat",             "intellect",
        "type",             "focus",
	"damage",           damage,
	"cost",             50,
	"level",            50,
	"cast time",         4,
	"spell object",     "inherit/objs/explorer_eye",
	"casting msg", ({
	  MAGENTA_F+"You feel magical energies flowing around you."+NORM+"\n",
	  B_MAGENTA_F+"You concentrate the energies around you."+NORM+"\n",
	  B_BLUE_F+"You weave the energies into a large floating eye."+NORM+"\n",
	}),
	"passive",
      }));
    return 1;
}

