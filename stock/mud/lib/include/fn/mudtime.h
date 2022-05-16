#ifndef _MUDTIME_H_
#define _MUDTIME_H_ 

#define QUARTER_CHANGE  1
#define DAYNIGHT_CHANGE 2
#define HOUR_CHANGE     3

#ifdef MUDTIMED 
#undef MUDTIMED
#endif

#define MUDTIMED "/obj/mudtime"

#define receive_time_notices recieve_time_notices

#endif

