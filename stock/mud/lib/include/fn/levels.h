#ifndef LEVELS_H
#define LEVELS_H

/*** security levels ***/ 
 
#define SEC0 0        /*  player                                      */
#define SEC1 10  
#define SEC2 20       /*  Apprentice - 'wiz powers, no privleges */
#define SEC3 30       /*  Immortal   - 'full wiz',  open area         */
#define SEC4 40       
#define SEC5 50
#define SEC6 60
#define SEC7 70
#define SEC8 80  
#define SEC9 90  
#define SEC10 100  
#define SEC11 111     /* Ash */

/****************************************************************/
/* optional QCs full read access, except private, write access  */
/* according to security level                                  */

#define QC  ({ "casti", })  /* array of wizard names */
#undef QC

/****************************************************************/
/* optional SCs have write access to the soul function object   */

#define SC  ({ "mumbles", }) /* array of wizard names */
#undef SC

#endif /* LEVELS_H */
