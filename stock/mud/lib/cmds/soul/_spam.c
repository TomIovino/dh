#include <soul.h>
 
/*   Hmm. Did Geewhiz say something about not using long write() arguments? Nah...  -Sparrowhawk@Dreamtime   */
status emote(string arg) {
 
write("You sing: 'Lovely spam, Wonderful Spa-a-m,'\n"
      "          'Lovely spam, Wonderful S-Spam,'\n"
      "                'Spa-a-a-a-a-a-a-am,'\n"
      "                'Spa-a-a-a-a-a-a-am,'\n"
      "                'SPA-A-A-A-A-A-A-AM,'\n"
      "                'SPA-A-A-A-A-A-A-AM,'\n"
      "            'LOVELY SPAM,'\n"
      "                          'LOVELY SPAM,'\n"
      "            'LOVELY SPAM,'\n"
      "                          'LOVELY SPAM,'\n"
      "            'LOVELY SPA-A-A-A-AM . . .'\n"
      "                     'SPA-AM,'\n"
      "                     'SPA-AM,'\n"
      "                     'SPA-AM,'\n"
      "                   'SPA-A-A-AM!'\n");
say(TPNAME + " sings the Spam Song.\n");
return 1;
 
}
/* EOF */
 
