

/*  Mudlib Configuration File */

#ifndef MUDLIB_H
#define MUDLIB_H

#define PRINTD(X) printf("X = %-=*O\n",75 - strlen("X"),X)

#define BETA 3
#define MUD_VERSION "v.67" /* DragonHeart Mudlib */

/********************************************************************/
/* Define One of the following Lpmud Driver Types                   */

/* Notes:  Although the mudlib will run in native mode, its natural */
/*         configuration is COMPAT.  So security is not base upon   */
/*         euid, and uid.                                           */
/*                                                                  */
/*         Mudlib will autoconfig for MUDOS, AMYLAAR and            */
/*         NATIVE_MODE. Define OK312 for the old msdos driver       */
/*                                                                  */

/* #define OK312       /* ok312 msdos driver           */


/***********************************************************************/

#undef MUD_NAME
#define MUD_NAME        "DragonHeart"
#define INTERMUD_NAME   "DragonHeart"

/***********************************************************************/
/* Auto-config some stuff */

#ifndef COMPAT_FLAG
#define NATIVE_MODE
#define UID_ROOT     "Root"
#define UID_BACKBONE "Backbone"
#define UID_TYPE string
#else
#define UID_ROOT     1
#define UID_TYPE int
#endif /* COMPAT_FLAG */


#ifdef MUDOS       

#define MUDOS_DR
#define HAVE_WIZLIST 0

#else /* !MUDOS */

#ifdef __VERSION__

#if __VERSION__ > "03.02.1@0" && __VERSION__ < "03.02.1@999"
#define AMYLAAR321
#endif

#if __VERSION__ > "03.02@0" && __VERSION__ < "03.02@999"
#define AMYLAAR
#endif

#ifdef AMYLAAR321
#define AMYLAAR
#endif

#else /* !__VERSION__ */

#if !defined(OK312)
#define 312DR
#endif /* !OK312 */

#define HAVE_WIZLIST 0

#endif /* !__VERSION__ */

#endif /* !MUDOS */


/*******some overly used defines****************************************/
#define LL 50
#define PL 10
/***********************************************************************/
/* Driver Configuration */

#undef VERSION
#define DEBUG_LOG   "/"__HOST_NAME__".debug.log"



	/*************************************/
	/* configure for MSDOS 3.1.2 version */
	/*************************************/

#if defined(OK312)          

#define 312MASTER   
#define MSDOS
#define OLD_EXPLODE

#ifdef NATIVE_MODE
#define VERSION "3.1.2 MS-DOS Native"
#else
#define VERSION "3.1.2 MS-DOS Compat"
#endif
#undef DEBUG_LOG
#define DEBUG_LOG              "/debug.log"

#endif /* OK312 */


	/***********************/
	/* configure for 3.1.2 */
	/***********************/

#if defined(312DR)            

#define 312MASTER
#define OLD_EXPLODE

#ifdef NATIVE_MODE
#define VERSION "3.1.2 Native"
#else
#define VERSION "3.1.2 Compat"
#endif

#endif /* 3.1.2 */


	/********************************/
	/* configure for 0.9.1.93 MUDOS */
	/********************************/
	
#if defined(MUDOS_DR)         

#ifdef MSDOS
#define VERSION "OK09193c MS-DOS mudOS"
#undef DEBUG_LOG
#define DEBUG_LOG              "/log/debug.log"
#else
#define VERSION "mudOS"
#endif

#endif /* ok09193 msdos mudos */


	/********************************/
	/* configure for Amylaar Driver */
	/********************************/
	
#if defined(AMYLAAR)

#if defined(AMYLAAR321) || __VERSION__ > "03.02@156"
#define HAVE_WIZLIST 0
#else
#define HAVE_WIZLIST 1
#endif 

#ifdef NATIVE_MODE
#define VERSION "Amylaar "+ __VERSION__ +" Native"
#else
#define VERSION "Amylaar "+ __VERSION__ +" Compat"
#endif
#ifdef MSDOS
#undef DEBUG_LOG
#define DEBUG_LOG "/debug.log"
#endif /* MSDOS */
#endif /* AMYLAAR */

/*****************************************************************/

#ifdef MSDOS
#define DEAD_ED      "dead_ed"
#define ED_SAVE      "_edrc"
#endif

#ifndef DEAD_ED
#define DEAD_ED      ".dead_ed_files"
#endif

#ifndef ED_SAVE
#define ED_SAVE      ".edrc"
#endif


/**********************************************************************/
/* Main Mudlib Files  */

#define SIMUL_EFUN             "secure/sim_ef_1"
#define SPARE_SIMUL_EFUN_FILE  "secure/sim_ef_2"  /* Not used in MUDos */
#define BASE                   "obj/base_obj"
#define STATS                  "obj/stats"
#define SKILLS                 "obj/skills"
#define ACTIONS                "obj/actions"
#define PLAYER                 "obj/player"
#define WIZARD                 "obj/wizard"
#define MASTER                 __MASTER_OBJECT__
#define DOMAIN_DIR             "/d/"
#define WIZARD_DIR             "/w/"


/****************************************************************/
/* Intermud */

#if defined(AMYLAAR) && !defined(MSDOS)
#define INTERMUD       1
#define INETD          "/secure/inet/inetd"
#define UDP_CMD_DIR    "/secure/inet/udp/"
#endif

/***********************************************************************/
/* General Mudlib  */

#define ARMOUR            "inherit/armour"
#define MAGIC_ARMOUR      "inherit/armour"
#define LIVING            "obj/living"
#define BARKEEP           "inherit/barkeep"
#define SHOPKEEP          "inherit/shopkeep"
#define MONSTER           "inherit/monster"
#define TEACHER           "inherit/teacher"
#define ROOM              "inherit/room"
#define TIMER   "inherit/timer"
#define TREASURE          "inherit/treasure"
#define MONEY             "obj/money"
#define WEAPON            "inherit/weapon"
#define MAGIC_WEAPON      "inherit/weapon"
#define CONTAINER         "inherit/contain"
#define BANKKEEP              "inherit/bankkeep"
#define WAREHOUSE         "inherit/warehouse"
#define VOID              "secure/local/void"
#define TOWNHALL          "inherit/townhall"
#define CHURCH            "inherit/church"
#define INNER             "d/wizard/inner"
#define PASTE_DIR         "open/paste"
#define MORE_OB           "obj/more"
#define MAGIC_SCROLL      "inherit/scroll"
#define MAGIC_POTION      "inherit/potion"
#define UNDEAD            "inherit/undead"
#define BOARD             "inherit/board"
#define SIGN              "inherit/sign"
#define WAND              "inherit/wand"
#define SHUTD             "cmds/wiz/_sd"
#define CONTAIN           "inherit/contain"
#define FOOD              "inherit/food"
#define DRINK             "inherit/drink"
#define BINS             ({ "std", "skills", "spells" })
#define WIZ_BINS         ({ "wiz", "tools", "objects" })

/* EXP LEVEL COST */
#define EXP ({\
          0,        500,       1000,       2000,       4000, \
       8000,      12000,      15000,      18000,      21000, \
      24000,      28000,      32000,      34000,      36000, \
      40000,      50000,      60000,      70000,      80000, \
      90000,     100000,     110000,     120000,     130000, \
     150000,     180000,     210000,     240000,     270000, \
     300000,     350000,     400000,     450000,     500000, \
     550000,     650000,     750000,     850000,    1000000, \
    1200000,    1400000,    1600000,    1800000,    2000000, \
    2300000,    2600000,    2900000,    3100000,    3400000, \
    3800000,    4200000,    4600000,    4800000,    5000000, \
    5500000,    6000000,    6500000,    7000000,    7500000, \
    8000000,    8500000,    9000000,    9500000,   10000000, \
   11000000,   12000000,   14000000,   14000000,   15000000, \
   16000000,   17000000,   18000000,   19000000,   20000000, \
   21000000,   22000000,   23000000,   24000000,   25000000, \
   26000000,   27000000,   28000000,   29000000,   30000000, \
   32000000,   34000000,   36000000,   38000000,   40000000, \
   42000000,   44000000,   46000000,   48000000,   49000000, \
   50000000,   60000000,   70000000,   80000000,   90000000, \
   99000000,  })



/**********************************************************/
/* template files used by create_wizard() ('promote' cmd) */

#define INIT_ACCESS            "doc/template/access.c"
#define INIT_WORKROOM          "doc/template/workroom.c"
#define INIT_FILE              "/secure/init_file"

/*****************************************************************/
/* If the following are not defined, the relavent object will be */
/* removed from usuage in player objects.                        */   

#define PARTY_DAEMON      "obj/party/party_d"
#define MAIL_FIELD_LOADER "obj/mail_ldr"
#define MAILD   "secure/bim/bimd"
#define EDITOR            "obj/editor"
#define PAGER             "obj/pager"
#undef DEBUG
#define DEBUG             "obj/debug"
#define SOUL              "obj/soul"
#define SOUL_FUNC_FILE    "/obj/soul_fcn.c"
#define QUEST_ROOM       "d/quest/quest_room"
/*
#define USE_DEBUG /* force use of obj/debug as interactive object */


/******************************************************************/
/* a shorter time string */

#define DATE(T) ctime(T)[4..10]+((ctime(T)[20..23] == ctime(time())[20..23])\
 ? ctime(T)[11..15] : ctime(T)[20..23])


#define NOTIFYD "/secure/notify_d"
#endif /* MUDLIB_H */
