/*
 Dragonheart Mud is a creation of Daniel H. Huber (ash@rrnet.com)
 [1121 3rd St S. Moorhead Mn, 56560] All code not specifically documented
 otherwise is a property of Daniel H. Huber who retains all legal and
 intellectual rights under the laws of the United States and the treaties
 of the United Nations.
*/

/*
written by ash to make bin commands uniform and neat =)
feel free to add neat stuff.. =)
a note though, using different colors in cool, but my main concern 
was that things line up properly.
*/


#include <mudlib.h>
#include <ansi.h>
#define BAR "=============================="
#define EMPTY "                            "
#define END "--== End of List ==--"
#define FEND "--== End of File ==--"



#define ENDBAR NORM+RED_F+EMPTY+END+EMPTY+NORM
#define PRINTBAR NORM+RED_F+BAR+MAGENTA_F+MUD_NAME+BLUE_F+BAR+NORM
#define ENDFILE NORM+BLUE_F+EMPTY+END+EMPTY+NORM
