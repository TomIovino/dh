// ls.h
// Bubbs.  7/Aug/95.
// Symbols used to make using /cmds/wiz/_ls::do_ls() easier.

#if !defined(LS_H)

#define LS_H

#define LS_NO_SHOW        0x0001    // Used by -?
#define LS_SHOW_DIR       0x0002    // -d
#define LS_SHOW_HIDDEN    0x0004    // -a
#define LS_SHOW_LOADED    0x0008    // -l
#define LS_SHOW_FSIZE     0x0010    // -s
#define LS_SHOW_FTIME     0x0020    // -f
#define LS_SHOW_LTIME     0x0040    // -L
#define LS_SHOW_TOTALS    0x0080    // -t
#define LS_SHOW_TTOTALS   0x0100    // -T
#define LS_RECURSIVE      0x0200    // -r
#define LS_WIDE_FMT       0x0400    // -w
#define LS_FILES_ONLY     0x0800    // -F
#define LS_DIRS_ONLY      0x1000    // -D
#define LS_PAGE_OUTPUT    0x2000    // -p
#define LS_SHOW_CWD       0x4000    // -c

// Default options for 'ls'.
#define DEFAULT_OPTIONS   LS_SHOW_DIR | LS_SHOW_CWD | LS_SHOW_LOADED | LS_PAGE_OUTPUT

// Default options for 'dir'.
#define DIR_OPTIONS       LS_SHOW_DIR    | \
                          LS_SHOW_HIDDEN | \
                          LS_SHOW_FSIZE  | \
                          LS_SHOW_FTIME  | \
                          LS_SHOW_TOTALS | \
                          LS_WIDE_FMT

// Option usage for 'ls'/'dir' commands.
#define USAGE             "[-acdDfFlLprstTw?0] [<path>]"

// Defines for making get_dir() easier to use.
#define GD_FILES          0x00001
#define GD_SIZES          0x00002
#define GD_TIMES          0x00004

#endif   // LS_H
