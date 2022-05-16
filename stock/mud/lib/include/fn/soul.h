/*
// Defines for SOUL Bin commands
// Everyone should use these!
*/
 
#include <ansi.h>
 
#define TP         this_player()
#define TPNAME     TP->query_name()
#define GHOST(XXX) XXX->query_ghost()
#define IS_GHOST   GHOST(TP)
 
/* Returns gender ('male', 'female', or 'neuter') */
#define GEND TP->query_gender_string()
 
/* Returns pronoun ('he', 'she', or 'it') */
#define PRO TP->query_pronoun()
 
/* Returns possessive ('his', 'her', 'its') */
#define POSS TP->query_possessive()
 
/* Returns objective ('him', 'her', 'it') */
#define OBJ TP->query_objective()
 
/* Returns objective + "self" ('himself', 'herself', 'itself') */
#define OBJSELF TP->query_objective()+"self"
 
void write(string s) { return efun::printf("%s%s%s",CYAN_F,s,NORM); }
varargs void say(string s, object x) { if(!x) x = this_player(); return efun::say(CYAN_F+s+NORM,x); }
void tell_object(object o, string s) { return efun::tell_object(o,B_CYAN_F+s+NORM); }
