#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 2 "lang.y"
#line 3 "prolang.y"
/* The above line is to give proper line number references. Please mail me
 * if your compiler complains about it.
 */
/*
 * This is the grammar definition of LPC. The token table is built
 * automatically by make_func. The lang.y is constructed from this file and
 * the generated token list. The reason of this is that there is no
 * #include-statment that yacc recognizes.
 */
#include <stdio.h>
#ifdef __STDC__
#include <stdarg.h>
#endif

#define LANG
#include "lint.h"
#include "lex.h"
#include "interpret.h"
#include "object.h"
#include "exec.h"
#include "config.h"
#include "instrs.h"
#include "incralloc.h"
#include "switch.h"
#include "stralloc.h"

#if defined(__GNUC__) && !defined(lint) && !defined(DEBUG)
#define INLINE inline
#else
#define INLINE
#endif

#define YYMAXDEPTH	600

/* NUMPAREAS areas are saved with the program code after compilation.
 */
#define A_PROGRAM		0
#define A_STRINGS		1
#define A_VARIABLES		2
#define A_VIRTUAL_VAR		3
#define A_LINENUMBERS		4
#define A_INHERITS		5
#define A_ARGUMENT_TYPES	6
#define A_ARGUMENT_INDEX	7
#define NUMPAREAS		8
#define A_FUNCTIONS		8




#define A_STRING_NEXT	       11
#define A_INCLUDE_NAMES        12
#define NUMAREAS	       13

#define CURRENT_PROGRAM_SIZE (mem_block[A_PROGRAM].current_size)

#define BREAK_ON_STACK		0x4000000
#define BREAK_FROM_SWITCH	0x8000000
#define CASE_LABELS_ENABLED    0x10000000
#define BREAK_DELIMITER       -0x20000000

#define CONTINUE_ADDRESS_MASK   0x3ffff
#define SWITCH_DEPTH_UNIT	0x40000
#define SWITCH_DEPTH_MASK    0x3ffc0000
#define CONTINUE_DELIMITER  -0x40000000

struct const_list { struct const_list *next; struct svalue val; };
struct const_list_svalue {
    struct svalue head;
    struct const_list list;
};

static struct mem_block mem_block[NUMAREAS];

/*
 * Some good macros to have.
 */

#define BASIC_TYPE(e,t) ((e) == TYPE_ANY ||\
			 (e) == (t) ||\
			 (t) == TYPE_ANY)

#define TYPE(e,t) (BASIC_TYPE((e) & TYPE_MOD_MASK, (t) & TYPE_MOD_MASK) ||\
		   (((e) & TYPE_MOD_POINTER) && ((t) & TYPE_MOD_POINTER) &&\
		    BASIC_TYPE((e) & (TYPE_MOD_MASK & ~TYPE_MOD_POINTER),\
			       (t) & (TYPE_MOD_MASK & ~TYPE_MOD_POINTER))))

#define MASKED_TYPE(e,t) (BASIC_TYPE( (e) , (t) ) ||\
	  ( (e) == (TYPE_MOD_POINTER|TYPE_ANY) && (t) & TYPE_MOD_POINTER ) ||\
	  ( (t) == (TYPE_MOD_POINTER|TYPE_ANY) && (e) & TYPE_MOD_POINTER )    )

#define NON_VIRTUAL_OFFSET_TAG 0x4000
#define FUNCTION(n) ((struct function *)mem_block[A_FUNCTIONS].block + (n))
#define NV_VARIABLE(n) ((struct variable *)mem_block[A_VARIABLES].block + (n))
#define V_VARIABLE(n)  ((struct variable *)mem_block[A_VIRTUAL_VAR].block + \
			(n) - VIRTUAL_VAR_TAG)
#define VARIABLE(n) ((n) & VIRTUAL_VAR_TAG ? V_VARIABLE(n) : NV_VARIABLE(n))

#define align(x) (((x) + (sizeof(char*)-1) ) & ~(sizeof(char*)-1) )

/*
 * If the type of the function is given, then strict types are
 * checked and required.
 */
static int exact_types;
extern int pragma_strict_types;		/* Maintained by lex.c */
extern int pragma_save_types;		/* Also maintained by lex.c */
extern int pragma_combine_strings;	/* Also maintained by lex.c */
int approved_object;		/* How I hate all these global variables */

extern mp_int total_num_prog_blocks, total_prog_block_size;

extern int num_parse_error;
extern int d_flag;
static int heart_beat;		/* Number of the heart beat function */

static p_int stored_bytes;	/* used by store_line_number_info to */
static p_int stored_lines;	/* keep track of the stored info     */
static p_int last_include_start;
int num_virtual_variables;
static p_int switch_pc; /* to ease relative addressing */
static p_int current_break_address;
static p_int current_continue_address;
extern struct s_case_state case_state;
extern struct case_list_entry *case_blocks;
static int current_type;

static p_int last_expression;

static char *last_string_constant = 0;

static struct program NULL_program; /* marion - clean neat empty struct */

static char *get_two_types PROT((int type1, int type2));
void add_local_name PROT((struct ident *, int)),
	smart_log PROT((char *, int, char *, char *));
extern int yylex();
extern char *last_lex_string;
static int verify_declared PROT((struct ident *));

static void copy_variables PROT((struct program *, int));
static int copy_functions PROT((struct program *, int type));




void fix_function_inherit_indices PROT((struct program *));
void fix_variable_index_offsets PROT((struct program *));
static void type_error PROT((char *, int));
static void argument_type_error PROT((int, int));

extern int current_line;
/*
 * 'inherit_file' is used as a flag. If it is set to a string
 * after yyparse(), this string should be loaded as an object,
 * and the original object must be loaded again.
 */
extern char *current_file, *inherit_file;

/*
 * The names and types of arguments and auto variables.
 */
unsigned short type_of_locals[MAX_LOCAL];
unsigned long full_type_of_locals[MAX_LOCAL];
int current_number_of_locals = 0;
int current_break_stack_need = 0  ,max_break_stack_need = 0;

/*
 * The types of arguments when calling functions must be saved,
 * to be used afterwards for checking. And because function calls
 * can be done as an argument to a function calls,
 * a stack of argument types is needed. This stack does not need to
 * be freed between compilations, but will be reused.
 */
static struct mem_block type_of_arguments;

struct program *compiled_prog;	/* Is returned to the caller of yyparse */




static struct ident *all_globals = 0;
static struct ident *all_locals = 0;
static struct efun_shadow *all_efun_shadows = 0;

#ifdef __STDC__
void yyerrorf(char *format, ...)
{
    va_list va;
    char buff[512];
    char fixed_fmt[200];

    format = limit_error_format(fixed_fmt, format);
    va_start(va, format);
    vsprintf(buff, format, va);
    va_end(va);
    yyerror(buff);
}
#else
/*VARARGS1*/
void yyerrorf(format, a1, a2, a3)
char *format;
int a1, a2, a3;
{
    char buff[512];
    char fixed_fmt[200];

    format = limit_error_format(fixed_fmt, format);
    sprintf(buff, format, a1, a2, a3);
    yyerror(buff);
}
#endif

/*
 * Compare two types, and return true if they are compatible.
 */
static int compatible_types(t1, t2)
    int t1, t2;
{
    if (t1 == TYPE_UNKNOWN || t2 == TYPE_UNKNOWN)
	return 0;
    if (t1 == t2)
	return 1;
    if (t1 == TYPE_ANY || t2 == TYPE_ANY)
	return 1;
    if ((t1 & TYPE_MOD_POINTER) && (t2 & TYPE_MOD_POINTER)) {
	if ((t1 & TYPE_MOD_MASK) == (TYPE_ANY|TYPE_MOD_POINTER) ||
	    (t2 & TYPE_MOD_MASK) == (TYPE_ANY|TYPE_MOD_POINTER))
	    return 1;
    }
    return 0;
}

/*
 * Add another argument type to the argument type stack
 */
INLINE
static void add_arg_type(type)
    unsigned short type;
{
    struct mem_block *mbp = &type_of_arguments;
    if (mbp->current_size + sizeof type > mbp->max_size) {
	mbp->max_size <<= 1;
	mbp->block = rexalloc((char *)mbp->block, mbp->max_size);
    }
    *(short*)(mbp->block + mbp->current_size)  = type;
    mbp->current_size += sizeof type;
}

/*
 * Pop the argument type stack 'n' elements.
 */
INLINE
static void pop_arg_stack(n)
    int n;
{
    type_of_arguments.current_size -= sizeof (unsigned short) * n;
}

/*
 * Get type of argument number 'arg', where there are
 * 'n' arguments in total in this function call. Argument
 * 0 is the first argument.
 */
#if 0 /* not used */
INLINE
static int get_argument_type(arg, n)
    int arg, n;
{
    return
	((unsigned short *)
	 (type_of_arguments.block + type_of_arguments.current_size))[arg - n];
}
#endif

INLINE
static unsigned short *get_argument_types_start(n)
    int n;
{
    return
	&((unsigned short *)
	 (type_of_arguments.block + type_of_arguments.current_size))[ - n];
}

INLINE
static void check_aggregate_types(n)
    int n;
{
    unsigned short *argp, mask;

    argp = (unsigned short *)
	(type_of_arguments.block +
	  (type_of_arguments.current_size -= sizeof (unsigned short) * n) );
    for(mask = ~TYPE_MOD_REFERENCE; --n >= 0; ) {
	mask |= *argp++;
    }
    if (!(~mask & 0xffff))
	yyerror("Can't trace reference assignments.");
}

static char *realloc_mem_block(mbp, size)
    struct mem_block *mbp;
    mp_int size;
{
    mp_int max_size;
    char *p;

    max_size = mbp->max_size;
    do {
	max_size <<= 1;
    } while (size > max_size);
    p = rexalloc((char *)mbp->block, max_size);
    if (!p) {
	extern void lex_close PROT((char *));

	lex_close("Out of memory");
	return 0;
    }
    mbp->block = p;
    mbp->max_size = max_size;
    return p;
}

/* add_to_mem_block must not be called with length zero, because the length
 * is passed to memcpy .
 */
INLINE
static void add_to_mem_block(n, data, size)
    int n, size;
    char *data;
{
    struct mem_block *mbp = &mem_block[n];
    if (mbp->current_size + size > mbp->max_size) {
	if (!realloc_mem_block(mbp, mbp->current_size + size))
	    return;
    }
    memcpy(mbp->block + mbp->current_size, data, size);
    mbp->current_size += size;
}

static char *realloc_a_program() {
    return realloc_mem_block(&mem_block[A_PROGRAM], 0);
}

#define byte_to_mem_block(n, b) ((void)(\
    (mem_block[n].current_size == mem_block[n].max_size ?\
      !!realloc_mem_block(&mem_block[n],0):1)?\
     (mem_block[n].block[mem_block[n].current_size++] = (b)) : 0))

#define ins_byte(b) byte_to_mem_block(A_PROGRAM, b)
#ifndef ins_byte
INLINE
static void ins_byte(b)
    char b;
{
    if (mem_block[A_PROGRAM].current_size == mem_block[A_PROGRAM].max_size ) {
	if (!realloc_a_program())
	    return;
    }
    mem_block[A_PROGRAM].block[mem_block[A_PROGRAM].current_size++] = b;
}
#endif

/*
 * Store a 2 byte number. It is stored in such a way as to be sure
 * that correct byte order is used, regardless of machine architecture.
 * Also beware that some machines can't write a word to odd addresses.
 */
static void ins_short(l)
    short l;
{
    int current_size;
    char *dest;

    current_size = CURRENT_PROGRAM_SIZE;
    CURRENT_PROGRAM_SIZE = current_size + 2;
    if (current_size +1 >= mem_block[A_PROGRAM].max_size &&
	!realloc_a_program())
    {
	return;
    }
    dest = mem_block[A_PROGRAM].block + current_size;
    *dest++ = ((char *)&l)[0];
    *dest   = ((char *)&l)[1];
}

static void upd_short(offset, l)
    int offset;
    short l;
{
    char *dest;

    dest = mem_block[A_PROGRAM].block + offset;
    *dest++ = ((char *)&l)[0];
    *dest   = ((char *)&l)[1];
}

static short read_short(offset)
    int offset;
{
    short l[2];
    char *dest;

    dest = mem_block[A_PROGRAM].block + offset;
    ((char *)l)[0] = *dest++;
    ((char *)l)[1] = *dest;
    return l[0];
}

/*
 * Store a 4 byte number. It is stored in such a way as to be sure
 * that correct byte order is used, regardless of machine architecture.
 */
static void ins_long(l)
    int l;
{
    int current_size;
    char *dest;

    current_size = CURRENT_PROGRAM_SIZE;
    CURRENT_PROGRAM_SIZE = current_size + 4;
    if (current_size +3 >= mem_block[A_PROGRAM].max_size
	&& !realloc_a_program())
    {
	return;
    }
    dest = mem_block[A_PROGRAM].block + current_size;
    *dest++ = ((char *)&l)[0];
    *dest++ = ((char *)&l)[1];
    *dest++ = ((char *)&l)[2];
    *dest   = ((char *)&l)[3];
}

#define ins_f_byte(b) (ins_byte((char)((b) - F_OFFSET)))
#ifndef ins_f_byte
static void ins_f_byte(b)
    unsigned int b;
{
    ins_byte((char)(b - F_OFFSET));
}
#endif

#define ins_f_code(b) (_ins_f_code((b) - F_OFFSET))

#define PREPARE_INSERT(n) \
    char *__PREPARE_INSERT__p = (\
      (\
        CURRENT_PROGRAM_SIZE+(n) > mem_block[A_PROGRAM].max_size ?\
          realloc_a_program()\
        :\
          0\
      ),\
      mem_block[A_PROGRAM].block + CURRENT_PROGRAM_SIZE);

#define PREPARE_S_INSERT(n) \
    short __ADD_SHORT__s[2];\
    PREPARE_INSERT(n)

#define add_byte(b) (void)(*__PREPARE_INSERT__p++ = (b))

#define add_short(s) \
{\
    char *__ADD_SHORT__p = (char *)__ADD_SHORT__s;\
    *((short*)__ADD_SHORT__p) = (s);\
    *__PREPARE_INSERT__p++ = __ADD_SHORT__p[0];\
    *__PREPARE_INSERT__p++ = __ADD_SHORT__p[1];\
}

#define add_f_byte(b) (add_byte((char)((b) - F_OFFSET)))

#define defined_function(s) \
    ((s)->type == I_TYPE_GLOBAL ? (s)->u.global.function : -1)
#if 0
/*
 * Return the index of the function found, otherwise -1.
 */
static int defined_function(s)
    char *s;
{
    int offset;
    struct function *funp;

    for (offset = 0; offset < mem_block[A_FUNCTIONS].current_size;
	 offset += sizeof (struct function)) {
	funp = (struct function *)&mem_block[A_FUNCTIONS].block[offset];
	if (funp->flags & NAME_HIDDEN)
	    continue;
        if (strcmp(funp->name, s) == 0)
	    return offset / sizeof (struct function);
    }
    return -1;
}
#endif

/* special allocate/free subroutines to be able to free intermediate results
 * that were thrown away due to an error.
 */
static char *last_yalloced = 0;

static char *yalloc(size)
unsigned long size;
{
    char **p;

    p = (char **)xalloc(size+sizeof(char*));
    if (!p) return 0;
    *p++ = last_yalloced;
    last_yalloced = (char *)p;
    return (char *)p;
}

#if 1 || defined(DEBUG)
static void yfree(block)
char *block;
{
    char **p;

    p = (char **)block;
    if (p != (char **)last_yalloced) {
	debug_message("Block mismatch");
	return;
    }
    last_yalloced = *--p;
    xfree((char*)p);
}
#else
#define yfree(block) _yfree()
static void _yfree()
{
    char **p;

    p = (char **)last_yalloced;
    last_yalloced = *--p;
    xfree((char*)p);
}
#endif

/*
 * A mechanism to remember addresses on a stack. The size of the stack is
 * defined in config.h.
 */
static int comp_stackp;
static p_int comp_stack[COMPILER_STACK_SIZE];

static void push_address() {
    if (comp_stackp >= COMPILER_STACK_SIZE) {
	yyerror("Compiler stack overflow");
	comp_stackp++;
	return;
    }
    comp_stack[comp_stackp++] = mem_block[A_PROGRAM].current_size;
}

static void push_explicit(address)
    p_int address;
{
    if (comp_stackp >= COMPILER_STACK_SIZE) {
	yyerror("Compiler stack overflow");
	comp_stackp++;
	return;
    }
    comp_stack[comp_stackp++] = address;
}

static int pop_address() {
    if (comp_stackp == 0)
	fatal("Compiler stack underflow.\n");
    if (comp_stackp > COMPILER_STACK_SIZE) {
	--comp_stackp;
	return 0;
    }
    return comp_stack[--comp_stackp];
}


/*
 * If there is any initialization of a global variable, a function which will
 * execute the initialization code. This code is spread all over the program,
 * with jumps to next initializer. The next variable keeps track of
 * the previous jump. After the last initializer, the jump will be changed
 * into a return(0) statement instead.
 *
 * A function named '__INIT' will be defined, which will contain the
 * initialization code. If there was no initialization code, then the
 * function will not be defined.
 *
 * When inheriting from another object, a call will automatically be made
 * to call __INIT in that code from the current __INIT.
 */
static int last_initializer_end;
static int first_initializer_start;
static int variables_initialized;

/*
 * Arrange a jump to the current position for the initialization code
 * to continue.
 */
static void transfer_init_control() {
    if (last_initializer_end < 0) {
#ifdef ALIGN_FUNCTIONS
	CURRENT_PROGRAM_SIZE =
	(CURRENT_PROGRAM_SIZE + sizeof(char *) - 1) & -sizeof(char *);
#endif
	{
	    char *name;
	    PREPARE_INSERT(sizeof name + 3);

	    name = make_shared_string("__INIT");
	    memcpy(__PREPARE_INSERT__p , (char *)&name, sizeof name);
	    __PREPARE_INSERT__p += sizeof(name);
	    add_byte(TYPE_ANY);
	    add_byte(0); /* num_arg */
	    add_byte(0); /* num_local */
            first_initializer_start =
	      (CURRENT_PROGRAM_SIZE += sizeof name + 3) - 2;
	}
    } else if (CURRENT_PROGRAM_SIZE - 2 == last_initializer_end) {
	mem_block[A_PROGRAM].current_size -= 3;
    } else {
	/*
	 * Change the address of the last jump after the last
	 * initializer to this point.
	 */
	upd_short(last_initializer_end,
		  mem_block[A_PROGRAM].current_size);
    }
}

void add_new_init_jump();



static char prog_string_tags[32];

/*
 * Initialize the environment that the compiler needs.
 */

static void prolog() {
    int i;

    if (type_of_arguments.block == 0) {
	type_of_arguments.max_size = 100;
	type_of_arguments.block = xalloc(type_of_arguments.max_size);
    }
    type_of_arguments.current_size = 0;
    approved_object = 0;
    last_expression = -1;
    compiled_prog = 0;		/* 0 means fail to load. */
    heart_beat = -1;
    comp_stackp = 0;	/* Local temp stack used by compiler */
    current_continue_address = 0;
    current_break_address = 0;
    num_parse_error = 0;
    free_all_local_names();	/* In case of earlier error */
    /* Initialize memory blocks where the result of the compilation
     * will be stored.
     */
    for (i=0; i < NUMAREAS; i++) {
	mem_block[i].block = xalloc(START_BLOCK_SIZE);
	mem_block[i].current_size = 0;
	mem_block[i].max_size = START_BLOCK_SIZE;
    }
    stored_lines = 0;
    stored_bytes = 0;
    last_include_start = -1;
    bzero(prog_string_tags, sizeof prog_string_tags);
    num_virtual_variables = 0;
    case_blocks = 0;
    case_state.free_block = (struct case_list_entry *)(
      ((PTRTYPE)(&case_blocks))-
      ((PTRTYPE)(&((struct case_list_entry*)0)->next)-(PTRTYPE) 0)
    );
    case_state.next_free = case_state.free_block + 1;

    last_initializer_end = -3;
    variables_initialized = 0;

}

static int
insert_inherited
    PROT((char *,char *, struct program **, struct function *, int, char *));

/*
 * The program has been compiled. Prepare a 'struct program' to be returned.
 */
int32 current_id_number = 0;

static int define_new_function PROT(( struct ident *, int,int,int,int,int));

/*
 * Define a new function. Note that this function is called at least twice
 * for all function definitions. First as a prototype, then as the real
 * function. Thus, there are tests to avoid generating error messages more
 * than once by looking at (flags & NAME_PROTOTYPE).
 */
static int define_new_function(p, num_arg, num_local, offset, flags, type)
    struct ident *p;
    int num_arg, num_local;
    int offset, flags, type;
{
    int num;
    struct function fun;
    unsigned short argument_start_index;

    flags |= type & ~TYPE_MOD_MASK;
    if (p->type == I_TYPE_GLOBAL && (num = p->u.global.function) >= 0) {
	struct function *funp;

	/*
	 * The function was already defined. It may be one of several reasons:
	 *
	 * 1.	There has been a prototype.
	 * 2.	There was the same function defined by inheritance.
	 * 3.	This function has been called, but not yet defined.
	 * 4.	The function is defined twice.
	 * 5.	A "late" prototype has been encountered.
	 */
	funp = FUNCTION(num);
	if (funp->flags & TYPE_MOD_NO_MASK &&
	  !((funp->flags|flags) & (NAME_PROTOTYPE|NAME_UNDEFINED)) )
	    yyerrorf("Illegal to redefine 'nomask' function \"%s\"", p->name);
	if (!(funp->flags & (NAME_UNDEFINED|NAME_PROTOTYPE|NAME_INHERITED) ) )
	{
	    yyerrorf("Redeclaration of function %s.", p->name);
	    if ( !(flags & NAME_PROTOTYPE) )
		free_string(p->name);
	    return num;
	}
	/*
	 * It was either an undefined but used funtion, or an inherited
	 * function. In both cases, we now consider this to be THE new
	 * definition. It might also have been a prototype to an already
	 * defined function.
	 *
	 * Check arguments only when types are supposed to be tested,
	 * and if this function really has been defined already.
	 *
	 * 'nomask' functions may not be redefined.
	 */
	if (exact_types && funp->type != TYPE_UNKNOWN) {
	    int i;
	    if (funp->num_arg != num_arg && !(funp->flags & TYPE_MOD_VARARGS))
		yyerror("Incorrect number of arguments.");
	    else if (funp->num_arg == num_arg &&
	             ((funp->flags ^ flags) & TYPE_MOD_XVARARGS) &&
		     !(funp->flags & TYPE_MOD_VARARGS))
		yyerror("Incorrect number of arguments.");
	    else {
		unsigned short first_arg;

		first_arg = 
		  ((unsigned short *)mem_block[A_ARGUMENT_INDEX].block)[num];
		if (first_arg == INDEX_START_NONE) {
		    if (num_arg && !(funp->flags & NAME_TYPES_LOST) )
			yyerror(
			  "Called function not compiled with type testing."
			);
		} else {
		    /* Now check that argument types weren't changed. */
		    for (i=0; i < num_arg; i++) {
		    }
		}
	    }
	}
	/* If it was yet another prototype, then simply return. */
	if (flags & NAME_PROTOTYPE) {
	    return num;
	}
	if (funp->num_arg != num_arg) {
	    funp->num_arg = num_arg;
	    ((unsigned short *)mem_block[A_ARGUMENT_INDEX].block)[num] =
	      INDEX_START_NONE;
	}
	funp->num_local = num_local;
	funp->flags = flags;
	funp->offset.pc = offset;
#if 0
	funp->function_index_offset = 0;
#endif
	funp->type = type;
	return num;
    }
    if (strcmp(p->name, "heart_beat") == 0)
	heart_beat = mem_block[A_FUNCTIONS].current_size /
	    sizeof (struct function);
    fun.name = p->name;
    fun.offset.pc = offset;
    fun.flags = flags;
    fun.num_arg = num_arg;
    fun.num_local = num_local;
#if 0
    fun.function_index_offset = 0;
#endif
    fun.type = type;
    num = mem_block[A_FUNCTIONS].current_size / sizeof fun;
    if (p->type != I_TYPE_GLOBAL) {
        if (p->type != I_TYPE_UNKNOWN) {
            p = make_shared_identifier(p->name, I_TYPE_GLOBAL);
        }
        /* should be I_TYPE_UNKNOWN now. */
        p->type = I_TYPE_GLOBAL;
        p->u.global.variable = -1;
        p->u.global.efun     = -1;
        p->u.global.sim_efun = -1;
        p->next_all = all_globals;
        all_globals = p;
    } else if (p->u.global.variable == -2) {
        struct efun_shadow *q;

#if 0
        fprintf(stderr,"define efun shadow function '%s'\n",p->name);
#endif
        q = (struct efun_shadow *)xalloc(sizeof(struct efun_shadow));
        q->shadow = p;
#if 0
        fprintf(stderr,"all_efun_shadows: %x\n",all_efun_shadows);
        if (all_efun_shadows) {
            fprintf(stderr,"last shadow pnt: %x\n",all_efun_shadows->shadow);
            fprintf(stderr,"last name: '%s'\n",all_efun_shadows->shadow->name);
        }
#endif
        q->next = all_efun_shadows;
        all_efun_shadows = q;
    }
    p->u.global.function = num;
    /* Number of local variables will be updated later */
    add_to_mem_block(A_FUNCTIONS, (char *)&fun, sizeof fun);

    if (exact_types == 0) {
	argument_start_index = INDEX_START_NONE;
    } else {
	int i;

	/*
	 * Save the start of argument types.
	 */
	argument_start_index =
	    mem_block[A_ARGUMENT_TYPES].current_size /
		sizeof (unsigned short);
	for (i=0; i < num_arg; i++) {
	    add_to_mem_block(A_ARGUMENT_TYPES, (char *)&type_of_locals[i],
			     sizeof type_of_locals[i]);
	}
    }
    add_to_mem_block(A_ARGUMENT_INDEX, (char *)&argument_start_index,
		     sizeof argument_start_index);
    return num;
}


static void define_variable(name, flags)
    struct ident *name;
    int32 flags;






{
    struct variable dummy;
    int n;

    if (name->type != I_TYPE_GLOBAL) {
        if (name->type != I_TYPE_UNKNOWN) {
            name = make_shared_identifier(name->name, I_TYPE_GLOBAL);
        }
        name->type = I_TYPE_GLOBAL;
        name->u.global.function = -1;
        name->u.global.variable = -1;
        name->u.global.efun     = -1;
        name->u.global.sim_efun = -1;
        name->next_all = all_globals;
        all_globals = name;
    } else if (name->u.global.function == -2) {
        struct efun_shadow *q;

        q = (struct efun_shadow *)xalloc(sizeof(struct efun_shadow));
        q->shadow = name;
        q->next = all_efun_shadows;
        all_efun_shadows = q;
    }
    if ( (n = name->u.global.variable) >= 0) {
	if ( VARIABLE(n)->flags & TYPE_MOD_NO_MASK && !(flags & NAME_HIDDEN))
            yyerrorf("Illegal to redefine 'nomask' variable \"%s\"",
	      name->name);
	if (flags & NAME_INHERITED) {
	    flags |= ~(VARIABLE(n)->flags) & TYPE_MOD_STATIC;
	} else {
	    VARIABLE(n)->flags |=   ~flags & TYPE_MOD_STATIC;
	}
    }
    dummy.name = name->name;
    increment_string_ref(dummy.name);
    dummy.flags = flags;
    if (flags & TYPE_MOD_VIRTUAL) {
	if (!(flags & NAME_HIDDEN))
	    name->u.global.variable = VIRTUAL_VAR_TAG |
		(mem_block[A_VIRTUAL_VAR].current_size / sizeof dummy);
	add_to_mem_block(A_VIRTUAL_VAR, (char *)&dummy, sizeof dummy);



    } else {
	if (!(flags & NAME_HIDDEN))
            name->u.global.variable =
		mem_block[A_VARIABLES].current_size / sizeof dummy;
	add_to_mem_block(A_VARIABLES, (char *)&dummy, sizeof dummy);



    }
}

static void redeclare_variable(name, flags, n)
    struct ident *name;
    int32 flags;
    int n;
{
    if (name->type != I_TYPE_GLOBAL) {
	/* I_TYPE_UNKNOWN */
        name->type = I_TYPE_GLOBAL;
        name->u.global.function = -1;
        name->u.global.variable = -1;
        name->u.global.efun     = -1;
        name->u.global.sim_efun = -1;
        name->next_all = all_globals;
        all_globals = name;
    } else if (name->u.global.function == -2) {
        struct efun_shadow *q;

        q = (struct efun_shadow *)xalloc(sizeof(struct efun_shadow));
        q->shadow = name;
        q->next = all_efun_shadows;
        all_efun_shadows = q;
    }
    if (flags & NAME_HIDDEN)
	return;
    if (name->u.global.variable >= 0 && name->u.global.variable != n) {
	if (VARIABLE(name->u.global.variable)->flags & TYPE_MOD_NO_MASK )
	    yyerrorf("Illegal to redefine 'nomask' variable \"%s\"",
		name->name);
    } else if ( V_VARIABLE(n)->flags & TYPE_MOD_NO_MASK &&
	 !(V_VARIABLE(n)->flags & NAME_HIDDEN) &&
	 (V_VARIABLE(n)->flags ^ flags) & TYPE_MOD_STATIC )
    {
	yyerrorf("Illegal to redefine 'nomask' variable \"%s\"", name->name);
    }
    name->u.global.variable = n;
    V_VARIABLE(n)->flags = flags;
}

static int last_string_is_new;

static int prog_string_indizes[0x100];

short store_prog_string(str)
    char *str;
{
    int size;
    long hash;
    char mask, *tagp;
    int i, *indexp;

    hash = (long)str ^ (long)str >> 16;
    hash = (hash ^ hash >> 8);
    mask = 1 << (hash & 7);
    hash = hash & 0xff;
    indexp = &prog_string_indizes[hash];
    tagp = &prog_string_tags[hash >> 3];
    if (*tagp & mask) {
	i = *indexp;
	for(;;) {
	    if ( ((char**)(mem_block[A_STRINGS].block))[i] == str ) {
		free_string(str); /* Needed as string is only free'ed once. */
		last_string_is_new = 0;
		return i;
	    }
	    if (
	      (i=*((int*)(&((char**)(mem_block[A_STRING_NEXT].block))[i]))) < 0
	    )
		break;
	}
	i = *indexp;
    } else {
	*tagp |= mask;
	i = -1;
    }
    size = mem_block[A_STRINGS].current_size;
    if (size + sizeof(char *) > mem_block[A_STRINGS].max_size) {
	if (!realloc_mem_block(&mem_block[A_STRINGS], 0) ||
	    !realloc_mem_block(&mem_block[A_STRING_NEXT], 0))
	{
	    if (i < 0)
		*tagp &= ~mask;
	    last_string_is_new = 0;
	    return 0;
	}
    }
    mem_block[A_STRING_NEXT].current_size =
      mem_block[A_STRINGS].current_size = size + sizeof str;
    *((char **)(mem_block[A_STRINGS].block+size)) = str;
    *((int *)(mem_block[A_STRING_NEXT].block+size)) = i;
    last_string_is_new = 1;
    return *indexp = size / sizeof str;
}

void delete_prog_string()
{
    char *str;
    int size;
    long hash;
    char mask, *tagp;
    int *indexp;

    size = mem_block[A_STRINGS].current_size - sizeof(char *);
    free_string(
      str = *(char**)(mem_block[A_STRINGS].block+size)
    );
    mem_block[A_STRING_NEXT].current_size =
      mem_block[A_STRINGS].current_size = size;
    hash = (long)str ^ (long)str >> 16;
    hash = (hash ^ hash >> 8);
    mask = 1 << (hash & 7);
    hash = hash & 0xff;
    indexp = &prog_string_indizes[hash];
    tagp = &prog_string_tags[hash >> 3];
    if ( ( *indexp = *((int *)(mem_block[A_STRING_NEXT].block+size)) ) < 0)
	*tagp &= ~mask;
}



























































static void insert_pop_value();

#define FIX_BRANCH(lfcode, destination, location) fix_branch(\
  (lfcode)-F_OFFSET, destination, location)

int fix_branch(ltoken, dest, loc)
    int ltoken, dest, loc;
{
    int offset;

    offset =  dest - (loc +1);
    if (offset > 0xff) {
        int i,j;
        char *p;

#ifdef DEBUG
	if (d_flag > 2) {
	    debug_message("fix_branch handles long offset.\n");
	}
#endif
	if ( current_break_address > loc &&
	    !(current_break_address & (BREAK_ON_STACK|BREAK_DELIMITER) ) )
	{
	    for(i = current_break_address; (j=read_short(i)) > loc; )
	    {
		upd_short(i, j+1);
		i = j;
	    }
	    current_break_address++;
	}
	if ( (current_continue_address & CONTINUE_ADDRESS_MASK) > loc &&
	    !(current_continue_address & CONTINUE_DELIMITER ) )
	{
	    for(i = current_continue_address & CONTINUE_ADDRESS_MASK;
	      (j=read_short(i)) > loc; )
	    {
		upd_short(i, j+1);
		i = j;
	    }
	    current_continue_address++;
	}
	ins_byte(0);
	p = mem_block[A_PROGRAM].block +
	    mem_block[A_PROGRAM].current_size-1;
	i = mem_block[A_PROGRAM].current_size - loc;
	for( ; --i >= 0; --p ) *p = p[-1];
	*p = ltoken;
	upd_short(loc, offset+2);
	if (offset > 0x7ffd)
	    yyerror("offset overflow");
	return 1;
    } else {
        mem_block[A_PROGRAM].block[loc] = offset;
	return 0;
    }
}

static char *ystring_copy(str)
char *str;
{
    char *p;

    p = yalloc(strlen(str)+1);
    (void)strcpy(p, str);
    return p;
}

static void add_string_constant() {
    mp_int len1;
    char *tmp;

    len1 = strlen(last_string_constant);
    tmp = alloca(len1 + strlen(last_lex_string) + 1);
    strcpy(tmp, last_string_constant);
    strcpy(tmp + len1, last_lex_string);
    free_string(last_string_constant);
    free_string(last_lex_string);
    last_string_constant = make_shared_string(tmp);
    last_lex_string = 0;
}

char *yyget_space(size)
    p_int size;
{
    while (CURRENT_PROGRAM_SIZE + size > mem_block[A_PROGRAM].max_size)
	realloc_a_program();
    CURRENT_PROGRAM_SIZE += size;
    return mem_block[A_PROGRAM].block + CURRENT_PROGRAM_SIZE - size;
}

void yymove_switch_instructions(len, blocklen)
    int len;
    p_int blocklen;
{
    mp_int i, j;

    if ( (CURRENT_PROGRAM_SIZE += len) > mem_block[A_PROGRAM].max_size )
	(void)realloc_a_program();
    if ( (current_continue_address & CONTINUE_ADDRESS_MASK) > switch_pc &&
	!(current_continue_address & CONTINUE_DELIMITER ) )
    {
	for(i = current_continue_address & CONTINUE_ADDRESS_MASK;
	  (j=read_short(i)) > switch_pc; )
	{
		upd_short(i, j+len);
		i = j;
	}
	current_continue_address += len;
    }
    move_memory(
      mem_block[A_PROGRAM].block + switch_pc + len,
      mem_block[A_PROGRAM].block + switch_pc,
      blocklen
    );
}

void yycerrorl(s1, s2, line1, line2)
    char *s1, *s2;
    int line1, line2;
{
    char buff[100];

    sprintf(buff, s2, line1, line2);
    yyerrorf(s1, buff);
}















































INLINE int proxy_efun PROT((int, int));

static void arrange_protected_lvalue PROT((int, int, int, int));

struct s_lrvalue {
    short type;
    unsigned short start;
    short code;
    unsigned short end;
};

static struct s_lrvalue indexing_argument, indexing_index1, indexing_index2;
static int indexing_code;
static struct svalue *currently_initialized;

#line 1415 "lang.y"
typedef union
{
#line 1288 "prolang.y"
	p_int number;
	p_uint address;	/* Address of an instruction */
	char *string;
	char *shared_string;
	short type;
	struct { p_int key; int numeric; } case_label;
	struct { int simul_efun; short start; } function_call_head;
	struct svalue svalue;



	struct svalue *initialized;
	struct {
	    int function;
	    struct svalue *initialized;
	} const_call_head;
	p_int numbers[2];
	struct {
	    p_int length;
	    struct const_list * l;
	} const_list;
	struct {
	    char *p;
	    unsigned short length;
	    unsigned short line;
	} expression;
	struct {
	    union {
		char *p, simple[2];
	    } u;
	    unsigned short length;
	    short type;
	} lvalue;
	struct {
	    char *super;
	    struct ident *real;
	} function_name;
	struct ident *ident;
	double float_number;
	struct {
	    p_int number;



	} closure;
	struct {
	    char *name;
	    int quotes;
	} symbol;
	struct s_lrvalue lrvalue;
} YYSTYPE;
#line 1347 "y.tab.c"
#define F_IF 257
#define F_ELSE 258
#define F_CASE 259
#define F_DEFAULT 260
#define F_WHILE 261
#define F_DO 262
#define F_FOR 263
#define F_CONTINUE 264
#define F_INT 265
#define F_STATUS 266
#define F_STRING_DECL 267
#define F_FLOAT_DECL 268
#define F_OBJECT 269
#define F_CLOSURE_DECL 270
#define F_SYMBOL_DECL 271
#define F_VOID 272
#define F_MIXED 273
#define F_STATIC 274
#define F_PRIVATE 275
#define F_NO_MASK 276
#define F_PROTECTED 277
#define F_PUBLIC 278
#define F_VARARGS 279
#define F_VIRTUAL 280
#define F_INHERIT 281
#define F_COLON_COLON 282
#define F_ARROW 283
#define F_QUOTED_AGGREGATE 284
#define F_ILLEGAL 285
#define F_DUMMY 286
#define F_ESCAPE 287
#define F_TEFUN 288
#define F_VEFUN 289
#define F_IDENTIFIER 290
#define F_RETURN 291
#define F_STRING 292
#define F_NUMBER 293
#define F_FLOAT 294
#define F_MAPPING 295
#define F_CLOSURE 296
#define F_SYMBOL 297
#define F_INC 298
#define F_DEC 299
#define F_LAND 300
#define F_LOR 301
#define F_ASSIGN 302
#define F_ADD 303
#define F_SUBTRACT 304
#define F_MULTIPLY 305
#define F_DIVIDE 306
#define F_LT 307
#define F_GT 308
#define F_EQ 309
#define F_GE 310
#define F_LE 311
#define F_NE 312
#define F_BREAK 313
#define F_SWITCH 314
#define F_SSCANF 315
#define F_LOCAL 316
#define F_MOD 317
#define F_COMPL 318
#define F_AND 319
#define F_OR 320
#define F_XOR 321
#define F_LSH 322
#define F_RSH 323
#define F_CATCH 324
#define F_NOT 325
#define F_RANGE 326
#define F_POP_VALUE 327
#define F_DUP 328
#define F_CALL_FUNCTION_BY_ADDRESS 329
#define F_CALL_EXPLICIT_INHERITED 330
#define F_PUSH_IDENTIFIER_LVALUE 331
#define F_VIRTUAL_VARIABLE 332
#define F_PUSH_VIRTUAL_VARIABLE_LVALUE 333
#define F_IDENTIFIER16 334
#define F_PUSH_IDENTIFIER16_LVALUE 335
#define F_PUSH_LOCAL_VARIABLE_LVALUE 336
#define F_PUSH_INDEXED_LVALUE 337
#define F_PUSH_RINDEXED_LVALUE 338
#define F_INDEX_LVALUE 339
#define F_RINDEX_LVALUE 340
#define F_INDEX 341
#define F_RINDEX 342
#define F_CONST0 343
#define F_CONST1 344
#define F_LBRANCH 345
#define F_LBRANCH_WHEN_ZERO 346
#define F_LBRANCH_WHEN_NON_ZERO 347
#define F_BRANCH 348
#define F_BRANCH_WHEN_ZERO 349
#define F_BRANCH_WHEN_NON_ZERO 350
#define F_BBRANCH_WHEN_ZERO 351
#define F_BBRANCH_WHEN_NON_ZERO 352
#define F_SIMUL_EFUN 353
#define F_RETURN0 354
#define F_PRE_INC 355
#define F_PRE_DEC 356
#define F_POST_INC 357
#define F_POST_DEC 358
#define F_CSTRING0 359
#define F_CSTRING1 360
#define F_CSTRING2 361
#define F_CSTRING3 362
#define F_CLIT 363
#define F_NCLIT 364
#define F_VOID_ASSIGN 365
#define F_VOID_ADD_EQ 366
#define F_ADD_EQ 367
#define F_SUB_EQ 368
#define F_DIV_EQ 369
#define F_MULT_EQ 370
#define F_MOD_EQ 371
#define F_AND_EQ 372
#define F_OR_EQ 373
#define F_XOR_EQ 374
#define F_LSH_EQ 375
#define F_RSH_EQ 376
#define F_RANGE_LVALUE 377
#define F_AGGREGATE 378
#define F_EXTRACT2 379
#define F_PREVIOUS_OBJECT0 380
#define F_LAMBDA_CCONSTANT 381
#define F_LAMBDA_CONSTANT 382
#define F_M_AGGREGATE 383
#define F_M_CAGGREGATE 384
#define F_MAP_INDEX 385
#define F_PUSH_INDEXED_MAP_LVALUE 386
#define F_JUMP 387
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,   38,   38,   40,   40,   41,    8,    8,   44,   17,
   17,   45,   46,   39,   39,   39,   48,   48,    5,    5,
   18,   18,   14,   14,   14,   14,   14,   14,   10,   15,
   15,   11,   11,   12,   13,   13,    6,   29,   28,   16,
   16,    7,    7,    7,    7,    7,    7,    7,    7,    7,
    7,   47,   47,   49,   50,   49,   43,   51,   51,   54,
   53,   53,   52,   52,   55,   55,   55,   55,   55,   55,
   55,   55,   55,   55,   55,   55,   55,   55,   64,   65,
   57,   66,   67,   58,   69,   70,   71,   72,   59,   68,
   68,   73,   60,   61,   61,   27,   27,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,   62,   36,
   74,   36,   35,   35,   75,   76,   35,   77,   35,   78,
   35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
   35,   35,   79,   35,   35,   35,   35,   35,   35,   35,
   35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
   37,   32,   32,   32,   32,   32,   32,   32,   32,   32,
   32,   32,   32,   32,   32,   32,   32,   32,   32,   32,
   32,   32,   32,   32,   32,   32,   32,   32,   32,   32,
   32,   32,   32,   20,   20,   63,   63,    2,    2,    2,
    9,    9,    3,    3,    3,   21,   21,   21,   21,   21,
   22,   22,   23,   23,   80,   33,   34,    4,    4,    4,
    4,    4,    4,    4,    4,    4,    4,    4,   30,   30,
   30,   30,   30,   30,   30,   30,   30,   30,   30,   42,
   42,   42,   42,   42,   81,   31,   82,   31,   26,   26,
   26,   25,   25,   25,   24,   56,   19,   83,   19,
};
short yylen[] = {                                         2,
    1,    3,    0,    0,    1,    4,    0,    1,    0,    2,
    1,    0,    0,    9,    3,    1,    3,    3,    0,    1,
    1,    3,    1,    1,    1,    1,    1,    1,    1,    0,
    2,    1,    3,    3,    1,    2,    2,    4,    6,    1,
    0,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    3,    2,    0,    5,    4,    0,    4,    2,
    1,    3,    0,    2,    2,    2,    1,    1,    1,    1,
    1,    1,    1,    2,    1,    1,    2,    2,    0,    0,
    7,    0,    0,    9,    0,    0,    0,    0,   13,    0,
    1,    0,    6,    3,    5,    1,    1,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    2,    2,    2,    1,    2,    1,
    0,    4,    3,    3,    0,    0,    7,    0,    4,    0,
    4,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    0,    4,    3,    3,    3,    3,    2,    2,
    2,    2,    5,    6,    2,    2,    2,    2,    2,    1,
    0,    1,    1,    1,    1,    1,    1,    4,    1,    1,
    6,    5,    6,    6,    7,    7,    8,    5,    6,    2,
    2,    7,    8,    9,    9,   10,   10,   11,    1,    1,
    4,    5,    6,    1,    1,    1,    2,    0,    1,    2,
    1,    3,    0,    1,    3,    0,    1,    2,    1,    2,
    2,    4,    2,    3,    0,    5,    8,    0,    3,    3,
    6,    7,    8,    7,    8,    9,    9,   10,    1,    1,
    4,    5,    6,    5,    6,    6,    7,    7,    8,    1,
    3,    2,    4,    3,    0,    5,    0,    7,    1,    2,
    1,    1,    2,    3,    4,    3,    0,    0,    3,
};
short yydefred[] = {                                      3,
    0,    0,    0,    0,   30,    0,    0,    0,   16,    8,
    0,    0,   52,   29,   30,    0,   36,    0,   43,   42,
   44,   48,   45,   46,   47,   50,   51,   24,   25,   23,
   28,   26,   27,   49,   40,   31,   37,    5,    2,    0,
   15,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   53,   34,  242,    0,    0,    6,    0,    0,    0,  244,
    0,    0,    0,    0,    0,   21,    0,    0,  161,    0,
    0,  164,  167,  165,  166,  194,  195,  161,    0,  215,
    0,    0,    0,    0,    0,    0,  245,    0,    0,    0,
    0,  162,    0,  169,  170,    0,  243,   13,    0,   30,
    0,  253,    0,  250,    0,    0,  155,  180,  181,    0,
  157,  156,  161,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  149,  150,  158,  159,    0,    0,    0,  130,
  128,    0,    0,    0,    0,    0,    0,  125,    0,    0,
    0,    0,    0,  143,    0,    0,    0,    0,    0,   17,
   18,   22,    0,    0,    0,    0,    0,    0,    0,  190,
    0,    0,    0,    0,    0,    0,    0,  161,    0,    0,
  254,    0,    0,    0,  247,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  146,  148,  147,   11,   14,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   38,  168,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   58,   10,  172,
    0,    0,  216,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  246,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  173,    0,    0,    0,   39,  171,    0,
    0,    0,    0,  178,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  182,    0,    0,    0,  179,    0,    0,
  174,  193,  248,    0,    0,    0,    0,    0,    0,   61,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   76,
   57,    0,    0,   75,   64,   67,   68,   69,   70,   71,
   72,   73,    0,    0,    0,  217,    0,    0,    0,  183,
    0,    0,    0,    0,  176,  175,    0,   60,   59,    0,
   66,    0,  118,    0,    0,    0,    0,    0,    0,    0,
  119,   85,   78,    0,   77,    0,    0,   65,   74,    0,
    0,    0,    0,    0,    0,    0,    0,  185,  184,  177,
   62,    0,    0,  116,  115,  117,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   94,    0,    0,  258,  256,    0,
    0,    0,    0,  187,  186,  255,  114,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  111,  113,  112,    0,    0,    0,   92,    0,    0,   83,
    0,    0,  188,   95,   86,    0,  259,   80,    0,    0,
    0,    0,    0,    0,   93,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   81,    0,    0,    0,    0,    0,
    0,   87,    0,    0,    0,    0,    0,   84,    0,    0,
   88,    0,   89,
};
short yydgoto[] = {                                       1,
  358,  154,  216,  288,   62,   63,   35,  308,  155,   15,
    4,    5,    6,   36,    7,   37,  198,   65,  409,   86,
  206,  207,  240,  322,   87,   88,  359,   89,   90,   91,
   92,   93,   94,   95,  166,  323,  116,    2,    8,   39,
    9,   54,  324,  199,   48,  149,   12,   66,   13,   49,
  258,  287,  309,  310,  325,  326,  327,  328,  329,  330,
  331,  332,  333,  334,  456,  335,  449,  436,  406,  454,
  477,  482,  446,  213,  186,  257,  179,  178,  192,  106,
  121,  219,  438,
};
short yysindex[] = {                                      0,
    0,    0,   20, -208,    0, -215, 2304,   42,    0,    0,
 -175,  140,    0,    0,    0,   20,    0,  -34,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   20,  317, -153, -133,  -34,  114,  120, -132, -119,
    0,    0,    0,  176, -115,    0,    0, 3362,    0,    0,
 -102,  165,   20, 2304,  174,    0,  -74,  -60,    0,    0,
  -57,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 3362,   11, 3362, 3362, 1170, 1982,    0,  -46, 3362, 3362,
  158,    0,  -72,    0,    0, 4816,    0,    0, -249,    0,
 3362,    0, 3362,    0,  235,  239,    0,    0,    0, 2244,
    0,    0,    0,  905,   20, 3362,    0,    0,  257,  -71,
  299,   70,    0,    0,    0,    0, 3362,  204, 3099,    0,
    0, 3362, 3362, 3362, 3362, 3362, 3362,    0, 3362, 3362,
 3362, 3362, 3362,    0, 3362, 3362, 3362, 3362,  303,    0,
    0,    0, 4816,  244,  326, 4816, 3362, 3362,    0,    0,
  -69, 3362,   20, 3362,  334, 4816,  343,    0, 3179, 3362,
    0, 4816,    0,  -57,    0, 3362, 1519, 3362, 3362,  625,
   12,   12,  625,  632,  632, 3362, 2345, 3539,  705,   12,
   12, 3362,  240,    0,    0,    0,    0,    0,  267,  363,
 3362, 3585,  391, 3227,  392,  348,  401, 3676,  321,  327,
    0,    0,  410, 3362, 1578,  414,  417, 4816,  423, 1809,
 2735, 3362,    0, 3328, 4104, 4816,  240,    0,    0,    0,
 4816, 3362,    0, 3362, 1647, 3362,  429, 3362, 3362,  416,
  432,  435, 3362, 1868, 2779, 3362,    0,    0, 3362, 3362,
 2818,    0, 3362,    0, 3703, 3730,  420,  905, 4816, 1937,
 2868, 3362,  438,    0, 3676, 4816, 3362,    0,    0, 4816,
 2913,    0, 3362,    0, 3766, 3795, 4816,  441, 3362,    0,
 3867, 3896,    0,    0, 3362,   20, 2552,  157, 2960,  442,
 3362, 4077, 4178,    0,  416, 4816, 3362,    0, 4205, 4232,
    0,    0,    0, 4268,    0,    0, 4816,  195,  164,    0,
   36,  447,  -32,  433,  458,  443, 3362,  444,  461,    0,
    0, 2625,  454,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  460,  250,  258,    0, 2692, 3362, 4297,    0,
 4360,  481,  483, 4387,    0,    0,    0,    0,    0,   20,
    0, 3362,    0,   28,   28,   28,  -32, 4890,  -56,  482,
    0,    0,    0,    0,    0, 3362,  268,    0,    0,  495,
 2625,    0,    0,  -63, 4414,  507,  509,    0,    0,    0,
    0,  510,   28,    0,    0,    0, 4874,   28,   28,   28,
   28,   28,   28,   28,   28,   28,   28,   28,   28,   28,
   28,   28,   28,  -32,    0, 3362,  511,    0,    0, 3362,
  275, 3273,  512,    0,    0,    0,    0, 1030,   88,   88,
 1030,  639,  639, 2645, 4911,  997,   88,   88,  359,  359,
    0,    0,    0,  503,    0,  506,    0, 2625,  522,    0,
 3362, 1715,    0,    0,    0, 2625,    0,    0,  529, 2002,
 3004, 3362,    0, 3362,    0, 2625, 3362, 3053,    0, 3362,
    0, 4580, 4696,  513,    0,  532, 3362, 4723, 4750,    0,
    0,    0,  515, 4789,    0,    0, 3362,    0,    0,  534,
    0, 2625,    0,
};
short yyrindex[] = {                                      0,
    0,   38,  287,  300,    0,    0,  -18,  147,    0,    0,
    0,    0,    0,    0,    0,  287,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 4971,
    0,  287, -269,    0,   -6,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 4882,    0,  -26,    0,
   55,    0, -225,  -33,  539,    0,    0,    0,    0,  206,
 1191,    0,    0,    0,    0,    0,    0,    0,  314,    0,
    0,    0,    0,    0, 3401,    0,    0,    0,    0,    0,
    0,    0, 1484,    0,    0,  230,    0,    0,    0,    0,
    0,    0,  477,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 3318,  567,    0, 1122, 1284, 3401,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  486,    0,
    0,    0,  107,    0,  485,  -15,    0,    0,   84,    0,
    0,  537,  497,  477,    0,  138,  583,    0,    0,  592,
    0,  109,    2,  365,    0,    0,    0,    0,    0, 1799,
 1742, 2168, 2322, 2022, 2126,    0, 3155, 1618,  305, 2195,
 2309,    0, 2029,    0,    0,    0,    0,    0,    0,    0,
  244,    0,  583,    0,  548,    0,  556,  -29,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  282,    0,    0,
    0,    0,  406,  835,  166,  593, 2099,    0,    0,    0,
  347,    0,    0,    0,    0,  557,    0,  559,    0,   14,
    0,    0,    0,    0,    0,    0, 1356,    0,    0,  592,
    0,  496,    0,  541,    0,    0,    0, 2463,  345,    0,
    0,    0,  -61,    0,    0,   79,    0,    0,    0,  338,
    0, 1425,    0,    0,    0,    0,  351,    0,    0,  601,
    0,    0,  696,  828,    0,  287, -214,    0,    0,   49,
    0,    0,    0,    0,   16,  193,    0,    0,    0,    0,
    0,    0,    0,    0,  896,  988,  197,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  594,    0,    0,    0,
    0, -214,  583,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   53,   54,    0,    0,    0, 1062,    0,    0,  287,
    0,    0,    0,    0,    0,    0,    0,  -55,    0,  -53,
    0,    0,    0,  281,    0,    0, 2507,    0,    0,    0,
 -214,  418,   61,    0,    0,   58,   64,    0,    0,    0,
    0,  583,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  595,  583,    0,    0,    0,
    0,    0,   67,    0,    0,    0,    0,  -31, 1025, 2335,
  -24,  452,  652,   45,    3,   -2, 2403, 2426,  504, 1655,
    0,    0,    0,    0,  344,    0,    0, -214,  583,    0,
    0,    0,    0,    0,    0, -214,    0,    0,    0,    0,
    0,    0,   95,  595,    0, -214,    0,    0,  201,    0,
  390,    0,    0,    0,    0,  583,    0,    0,    0,  430,
  436,    0,    0,    0,  516,  526,  615,    0,  527,    0,
    0, -214,    0,
};
short yygindex[] = {                                      0,
 4256,  493,  408,    0,    0,  127,  -64,  741,  -54,    0,
    0,    0,  666,    0,  126,    0,    0,    0,    0,    0,
    0,    0,  413,    0,  552,    0,  279,    0,    0,    0,
    0,  -70,    0,    0,  -58, -112,   48,    0,    0,    0,
    0,  -17,  489,    0,    0,    0,    0,  591,  654,    0,
    0,    0,    0,  349,  173,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -335,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,
};
#define YYTABLESIZE 5273
short yytable[] = {                                      96,
   47,  405,   96,  167,   97,   46,  101,  357,   41,  101,
   33,   33,  355,  102,  201,  120,  102,   54,  129,  169,
  115,  204,  107,   41,  111,  112,  101,  412,  201,  191,
  123,  124,   54,  102,  240,  100,  240,    1,  100,  161,
  150,  252,  153,   99,  156,  203,   79,   82,  148,  163,
  110,  240,  240,  146,  144,  100,  145,  211,  147,  212,
   99,   10,  101,  201,    7,   18,  151,  383,  172,  102,
  177,   14,  355,  180,  181,  182,  183,  184,  185,   30,
  187,  188,  189,  190,  191,   98,  193,  194,  195,  196,
    7,  100,  101,  356,  351,  241,   99,  241,  202,  102,
   38,  220,   98,  208,  220,  156,  211,  205,  212,  201,
  215,  218,  241,  241,   40,  217,  103,  220,  464,  224,
  225,  100,  213,  252,  403,  105,   99,  226,    3,  401,
  399,   16,  400,  227,  402,  221,   52,  213,  221,  192,
   43,  480,  231,  174,  193,  235,    4,  124,  176,  123,
  124,  190,  123,  356,  175,  244,   55,  177,   53,   57,
  162,  164,  255,  256,  124,  124,  123,  123,   98,   58,
   59,  213,   56,  259,  189,  260,   61,  231,  120,  265,
  266,  120,   64,   42,  270,  191,  275,  276,    4,   97,
  277,  218,  281,  286,  282,  217,  120,  336,   41,  124,
  337,  123,  292,  293,  364,   98,  129,  350,  296,  129,
  128,  128,  299,  128,  300,  164,   60,  100,   55,  128,
  304,  191,  349,  129,  129,   64,  307,  101,  129,  102,
  339,  124,  341,  123,  104,  122,  214,  127,  344,  382,
  127,  222,  189,  189,  222,  252,  189,  189,  189,  189,
  189,  214,  189,  407,  127,  127,   41,   45,  129,   45,
  353,   32,   32,  189,  189,  189,  374,  189,  189,  404,
   96,   41,   97,   56,  157,   55,  148,  101,  158,  375,
  101,  146,   41,  249,  102,  214,  147,  102,   56,  127,
  129,  192,  354,  435,  101,  360,  189,  439,  189,  189,
  108,  102,   30,   30,   30,   30,   30,   30,   30,   30,
   30,   30,   30,   30,   30,   30,   30,   30,   30,  240,
  353,  127,  204,  100,  121,  201,  109,   30,   99,  189,
  189,  192,   30,  136,  137,  174,  193,  101,  170,  197,
  176,  435,  134,  190,  466,  134,  175,  113,  134,  177,
  190,  190,  354,  442,  190,  190,  190,  190,  190,  171,
  190,  197,  134,  134,  435,  249,  189,  134,  200,  201,
   98,  190,  190,  190,  211,  190,  190,  191,  122,  168,
  241,  122,  450,  212,   91,  218,  360,  121,  218,  228,
  202,  205,  462,  463,  202,  403,  122,  134,  134,  468,
  401,  469,   91,  230,  190,  402,  190,  190,  474,  392,
  393,    4,    4,    4,    4,    4,    4,    4,    4,    4,
    4,    4,    4,    4,    4,    4,    4,    4,  134,  134,
  224,  233,  124,  224,  123,  236,    4,  190,  190,  202,
  237,    4,  191,  191,  238,  241,  191,  191,  191,  191,
  191,  242,  191,  243,  248,  125,  126,  252,  219,  127,
  249,  219,  250,  191,  191,  191,  129,  191,  191,  264,
  225,  202,  268,  225,  267,  269,  223,  285,  294,  223,
  178,  303,  340,  192,  348,   68,  352,  249,  189,  107,
  361,  129,  107,  173,  367,  174,  191,  362,  191,  191,
  366,  363,  365,  229,  229,  189,  189,  229,  189,  107,
  370,  107,  368,  107,  189,  189,  189,  189,  369,  371,
  174,  378,  127,  379,   55,  408,  193,  189,  189,  191,
  191,  189,  192,  192,  410,  440,  192,  192,  192,  192,
  192,  109,  192,  411,  109,  107,  109,  414,  109,  415,
  416,  437,  443,  192,  192,  192,  227,  192,  192,  227,
  444,  109,  448,  109,  445,  109,  226,  228,  457,  226,
  228,  472,  473,  478,  481,  107,    7,  178,  178,   20,
   35,  178,  178,  178,  178,  178,  192,  178,  192,  192,
   28,   29,   30,   31,   32,   33,  190,  109,  178,  178,
  178,  198,  178,  178,  134,  134,  176,    7,    9,  199,
  447,  230,  230,  190,  190,  230,  175,  177,  455,  192,
  192,    7,  190,  190,  190,  190,  121,  109,  465,  206,
  134,  178,  203,  178,  178,  190,  190,  179,  179,  190,
  209,  179,  179,  179,  179,  179,  251,  179,  207,  210,
  126,  208,  196,   90,  483,   90,  210,  278,  179,  179,
  179,  148,  179,  179,  178,  178,  146,  144,  148,  145,
   17,  147,  178,  146,  144,  403,  145,  295,  147,  175,
  401,  399,  434,  400,  142,  402,  143,  229,  191,  108,
  152,  179,  108,  179,  179,   51,    0,    0,  381,  249,
  189,    0,    0,  231,  231,  191,  191,  231,    0,  108,
    0,  108,  174,  108,  191,  191,  191,  191,  193,    0,
    0,    0,    0,    0,  179,  179,    0,  191,  191,    0,
    0,  191,  174,  174,    0,    0,  174,  174,  174,  174,
  174,  148,  174,   11,    0,  108,  146,  144,    0,  145,
    0,  147,    0,  174,  174,  174,   44,  174,  174,    0,
  107,  107,  107,  107,  142,    0,  143,    0,    0,    0,
    0,    0,    0,  107,  107,  108,    0,  107,  192,    0,
    0,    0,   50,    0,    0,    0,  174,    0,  174,  174,
    0,    0,    0,  232,  232,  192,  192,  232,  176,    0,
    0,    0,    0,   99,  192,  192,  192,  192,  175,  177,
    0,    0,  109,  109,  109,  109,    0,  192,  192,  174,
  174,  192,    0,  178,    0,  109,  109,    0,    0,  109,
    0,    0,    0,    0,    0,    0,    0,    0,  234,  234,
  178,  178,  234,    0,    0,    0,    0,    0,    0,  178,
  178,  178,  178,    0,    0,  165,    0,    0,    0,    0,
    0,    0,  178,  178,  193,  193,  178,    0,  193,  193,
  193,  193,  193,    0,  193,  131,    0,    0,  131,    0,
    0,    0,    0,  179,    0,  193,  193,  193,    0,  193,
  193,    0,  131,  131,    0,    0,    0,  131,  235,  235,
  179,  179,  235,  209,    0,    0,    0,    0,    0,  179,
  179,  179,  179,    0,    0,    0,    0,    0,  193,    0,
  193,  193,  179,  179,    0,    0,  179,  131,    0,    0,
    0,    0,  176,  176,  133,  134,  176,  176,  176,  176,
  176,    0,  176,    0,    0,    0,  136,  137,    0,    0,
    0,  193,  193,  176,  176,  176,    0,  176,  176,  131,
  108,  108,  108,  108,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  108,  108,    0,    0,  108,  174,    0,
    0,    0,    0,    0,    0,    0,  176,    0,  176,  176,
    0,    0,    0,  236,  236,  174,  174,  236,    0,    0,
    0,    0,    0,    0,  174,  174,  174,  174,    0,    0,
    0,    0,    0,  132,  133,  134,  135,  174,  174,  176,
  176,  174,    0,    0,  175,  175,  136,  137,  175,  175,
  175,  175,  175,  403,  175,    0,    0,    0,  401,  399,
    0,  400,    0,  402,    0,  175,  175,  175,    0,  175,
  175,    0,    0,    0,    0,    0,  397,    0,  398,    0,
    0,    0,  104,    0,    0,  104,  403,    0,    0,    0,
    0,  401,  399,    0,  400,    0,  402,    0,  175,    0,
  175,  175,  104,    0,  104,    0,  104,    0,    0,  397,
    0,  398,    0,    0,    0,    0,    0,    0,  177,  177,
    0,    0,  177,  177,  177,  177,  177,    0,  177,    0,
  193,  175,  175,    0,    0,    0,    0,    0,  104,  177,
  177,  177,    0,  177,  177,  233,  233,  193,  193,  233,
    0,    0,    0,    0,  131,  131,  193,  193,  193,  193,
    0,    0,    0,    0,    0,    0,    0,    0,  104,  193,
  193,    0,  177,  193,  177,  177,    0,    0,  151,  151,
  131,  252,  151,  151,  151,  151,  151,    0,  151,   19,
   20,   21,   22,   23,   24,   25,   26,   27,  176,  151,
  151,  151,    0,  151,  151,  177,  177,    0,    0,    0,
    0,    0,    0,  238,  238,  176,  176,  238,    0,   34,
    0,    0,    0,    0,  176,  176,  176,  176,    0,    0,
    0,    0,  189,    0,  151,  151,    0,  176,  176,    0,
    0,  176,    0,    0,    0,    0,    0,  163,  163,    0,
    0,  163,  163,  163,  163,  163,    0,  163,    0,    0,
    0,    0,    0,    0,    0,  151,  151,    0,  163,  163,
  163,    0,  163,  163,    0,    0,    0,    0,    0,    0,
  113,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  175,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  163,    0,  163,  163,  237,  237,  175,  175,  237,
    0,    0,  114,    0,    0,    0,  175,  175,  175,  175,
    0,    0,    0,    0,    0,  388,  389,  390,  391,  175,
  175,    0,    0,  175,  163,  163,    0,    0,  392,  393,
  152,  152,    0,    0,  152,  152,  152,  152,  152,    0,
  152,    0,    0,  104,  104,  104,  104,    0,    0,  389,
  390,  152,  152,  152,  177,  152,  152,    0,    0,    0,
  104,  392,  393,    0,    0,    0,    0,    0,    0,  239,
  239,  177,  177,  239,    0,    0,    0,    0,    0,    0,
  177,  177,  177,  177,  190,    0,  152,  152,    0,    0,
    0,    0,    0,  177,  177,    0,    0,  177,    0,    0,
    0,    0,  153,  153,    0,    0,  153,  153,  153,  153,
  153,    0,  153,  249,  189,    0,    0,  152,  152,    0,
    0,    0,    0,  153,  153,  153,    0,  153,  153,    0,
    0,  151,  151,    0,    0,    0,    0,    0,    0,    0,
  151,  151,  151,  151,   19,   20,   21,   22,   23,   24,
   25,   26,   27,  151,  151,    0,  191,  151,  153,  153,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  154,  154,    0,   34,  154,  154,  154,  154,  154,
    0,  154,  251,  163,    0,    0,    0,    0,    0,  153,
  153,    0,  154,  154,  154,    0,  154,  154,    0,    0,
  163,  163,    0,    0,    0,    0,    0,    0,    0,  163,
  163,  163,  163,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  163,  163,    0,  192,  163,  154,  154,    0,
  160,  160,    0,    0,  160,  160,  160,  160,  160,    0,
  160,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  160,  160,  160,    0,  160,  160,    0,  154,  154,
    0,    0,    0,    0,    0,  148,  141,    0,    0,    0,
  146,  144,  222,  145,    0,  147,  190,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  160,  160,  142,    0,
  143,  138,    0,  152,  152,    0,    0,    0,    0,    0,
    0,    0,  152,  152,  152,  152,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  152,  152,  160,  160,  152,
    0,  223,  140,    0,  148,  141,    0,    0,    0,  146,
  144,  246,  145,    0,  147,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  142,  191,  143,
  138,    0,  139,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  153,  153,    0,  133,    0,
    0,  133,    0,    0,  153,  153,  153,  153,    0,    0,
  247,  140,    0,    0,    0,  133,  133,  153,  153,    0,
  133,  153,    0,  148,  141,    0,    0,    0,  146,  144,
  262,  145,  110,  147,    0,  110,    0,  110,    0,  110,
    0,  139,    0,    0,    0,    0,  142,  192,  143,  138,
  133,  133,  110,    0,  110,    0,  110,    0,    0,    0,
    0,    0,    0,    0,  154,  154,    0,    0,    0,    0,
    0,    0,    0,  154,  154,  154,  154,    0,    0,  263,
  140,  133,  133,    0,    0,    0,  154,  154,  110,    0,
  154,  148,  141,    0,    0,    0,  146,  144,  452,  145,
    0,  147,    0,    0,    0,    0,    0,    0,    0,    0,
  139,    0,    0,    0,  142,    0,  143,  138,  110,  138,
    0,    0,  138,  160,  160,  138,    0,    0,    0,    0,
    0,    0,  160,  160,  160,  160,    0,    0,    0,  138,
  138,  138,    0,  138,  138,  160,  160,  453,  140,  160,
    0,    0,    0,    0,    0,    0,    0,    0,  130,  131,
    0,    0,    0,    0,    0,    0,    0,  132,  133,  134,
  135,    0,    0,    0,  138,  138,  135,    0,  139,  135,
  136,  137,  135,    0,  221,  148,  141,    0,    0,    0,
  146,  144,    0,  145,    0,  147,  135,  135,    0,    0,
    0,  135,    0,    0,    0,  138,  138,    0,  142,    0,
  143,  138,    0,    0,    0,    0,    0,  130,  131,    0,
    0,    0,    0,    0,    0,    0,  132,  133,  134,  135,
    0,  135,  135,    0,    0,    0,    0,    0,    0,  136,
  137,  252,  140,  245,  148,  141,    0,    0,    0,  146,
  144,    0,  145,    0,  147,    0,    0,  133,  133,    0,
    0,    0,  135,  135,    0,    0,    0,  142,    0,  143,
  138,    0,  139,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  133,    0,    0,  130,  131,    0,    0,
    0,    0,    0,    0,    0,  132,  133,  134,  135,    0,
  272,  140,    0,  110,  110,  110,  110,    0,  136,  137,
    0,    0,  261,  148,  141,    0,  110,  110,  146,  144,
  110,  145,    0,  147,    0,    0,    0,    0,    0,    0,
    0,  139,    0,    0,    0,    0,  142,    0,  143,  138,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  130,  131,    0,    0,    0,   82,
    0,  119,    0,  132,  133,  134,  135,    0,    0,  290,
  140,    0,    0,    0,    0,    0,  136,  137,  148,  141,
  451,  138,  138,  146,  144,    0,  145,    0,  147,    0,
  138,  138,  138,  138,    0,    0,    0,    0,    0,  141,
  139,  142,  141,  143,  138,  141,  145,  138,    0,  145,
    0,  145,  145,  145,    0,    0,    0,    0,    0,  141,
  141,  141,    0,  141,  141,    0,  145,  145,  145,    0,
  145,  145,    0,    0,  459,  140,    0,    0,  135,  135,
    0,    0,    0,    0,    0,    0,    0,  135,  130,  131,
  135,    0,    0,    0,  141,  141,    0,  132,  133,  134,
  135,  145,  145,    0,  135,  139,    0,    0,    0,    0,
  136,  137,    0,    0,  251,    0,  144,    0,    0,  144,
    0,  144,  144,  144,    0,  141,  141,    0,    0,    0,
    0,    0,  145,  145,    0,    0,  144,  144,  144,    0,
  144,  144,    0,  142,    0,    0,  142,  130,  131,  142,
    0,    0,    0,    0,    0,    0,  132,  133,  134,  135,
    0,    0,    0,  142,  142,  142,    0,  142,  142,  136,
  137,  144,  144,  271,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  140,    0,    0,  140,    0,
    0,  140,    0,    0,    0,    0,    0,    0,  142,  142,
    0,    0,  144,  144,    0,  140,  140,  140,    0,  140,
  140,    0,  139,    0,    0,  139,  130,  131,  139,    0,
    0,    0,    0,    0,    0,  132,  133,  134,  135,  142,
  142,    0,  139,  139,  139,    0,  139,  139,  136,  137,
  140,  140,  289,   68,    0,   69,    0,    0,    0,    0,
    0,  117,    0,   71,   72,   73,    0,   74,   75,    0,
    0,   82,    0,  119,    0,    0,    0,  139,  139,    0,
    0,  140,  140,    0,    0,    0,   78,  118,    0,    0,
    0,  130,  131,    0,    0,   80,    0,    0,    0,    0,
  132,  133,  134,  135,    0,    0,    0,    0,  139,  139,
    0,  141,  141,  136,  137,    0,    0,  458,  145,  145,
  141,  141,  141,  141,    0,    0,    0,  145,  145,  145,
  145,    0,    0,  141,  141,    0,  137,  141,    0,  137,
  145,  145,  137,    0,  145,    0,    0,    0,    0,  136,
    0,    0,  136,    0,    0,  136,  137,  137,  137,    0,
  137,  137,  106,    0,    0,  106,    0,    0,    0,  136,
  136,  148,  141,    0,  136,    0,  146,  144,    0,  145,
    0,  147,  106,    0,  106,    0,  106,    0,  144,  144,
    0,  137,  137,    0,  142,    0,  143,  144,  144,  144,
  144,    0,    0,    0,  136,  136,    0,    0,    0,    0,
  144,  144,    0,    0,  144,  142,  142,    0,  106,    0,
    0,    0,  137,  137,  142,  142,  142,  142,  140,    0,
  105,    0,    0,  105,    0,  136,  136,  142,  142,    0,
    0,  142,    0,    0,    0,    0,    0,    0,  106,    0,
  105,    0,  105,  103,  105,    0,  103,  140,  140,    0,
    0,    0,    0,    0,    0,    0,  140,  140,  140,  140,
    0,    0,    0,  103,    0,  103,    0,  103,    0,    0,
    0,    0,    0,  140,  139,  139,  105,    0,    0,    0,
   63,    0,   63,  139,  139,  139,  139,   63,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  103,
  139,   63,    0,    0,    0,   68,  105,   69,    0,    0,
    0,    0,    0,  159,    0,   71,   72,   73,    0,   74,
   75,    0,    0,    0,  257,    0,  257,    0,    0,  103,
    0,  257,    0,    0,    0,    0,    0,    0,   78,  160,
    0,    0,    0,    0,    0,  257,    0,   80,   19,   20,
   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
   31,   32,   33,    0,    0,   63,    0,   63,   63,   82,
    0,   85,    0,    0,    0,    0,   83,    0,   34,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  137,  137,
  320,    0,    0,    0,    0,    0,    0,  137,  137,  137,
  137,  136,  136,    0,    0,    0,    0,    0,    0,  257,
  136,  257,  257,  136,  137,    0,    0,    0,    0,    0,
    0,    0,    0,  106,  106,  106,  106,  136,    0,    0,
    0,    0,    0,  132,  133,  134,  135,    0,    0,    0,
  106,    0,   82,    0,   85,    0,  136,  137,    0,   83,
    0,    0,    0,    0,  228,    0,  321,   84,    0,    0,
    0,  403,  396,  320,    0,    0,  401,  399,    0,  400,
    0,  402,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  397,    0,  398,    0,    0,    0,
    0,  105,  105,  105,  105,    0,    0,    0,   63,   63,
    0,   63,   63,   63,   63,   63,   63,    0,  105,   82,
    0,  119,    0,    0,  103,  103,  103,  103,  395,    0,
    0,    0,    0,    0,   63,    0,   63,  228,    0,    0,
   84,  103,   63,   63,   63,   63,   63,    0,   63,   63,
   63,   63,  257,  257,    0,  257,  257,  257,  257,  257,
  257,    0,   82,    0,   85,   63,   63,   63,   63,   83,
    0,    0,    0,    0,    0,    0,   63,   63,  257,    0,
  257,    0,    0,    0,  253,    0,  257,  257,  257,  257,
  257,    0,  257,  257,  257,  257,    0,  311,  312,    0,
  313,  314,    0,    0,  315,  316,   82,    0,   85,  257,
  257,  257,  257,   83,    0,    0,    0,  254,    0,    0,
  257,  257,    0,   68,    0,   69,    0,    0,  273,    0,
    0,   70,  317,   71,   72,   73,    0,   74,   75,   76,
   77,    0,    0,    0,    0,   82,    0,   85,    0,    0,
   84,    0,   83,    0,  318,  319,   78,   79,    0,    0,
    0,  274,    0,    0,    0,   80,   81,  279,    0,    0,
  311,  312,    0,  313,  314,    0,    0,  315,  316,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   84,   82,   68,   85,   69,    0,
  280,    0,   83,    0,   70,  317,   71,   72,   73,    0,
   74,   75,   76,   77,    0,    0,    0,  291,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  318,  319,   78,
   79,    0,    0,   84,    0,    0,    0,    0,   80,   81,
   82,    0,   85,  388,  389,  390,  391,   83,    0,    0,
  274,    0,    0,    0,    0,    0,  392,  393,    0,    0,
    0,    0,  297,   68,    0,   69,    0,    0,    0,    0,
    0,  372,    0,   71,   72,   73,    0,   74,   75,    0,
   67,    0,    0,   84,    0,    0,    0,   82,    0,   85,
    0,    0,    0,    0,   83,  298,   78,  373,    0,    0,
    0,    0,    0,    0,    0,   80,   68,    0,   69,  338,
    0,    0,    0,    0,   70,    0,   71,   72,   73,    0,
   74,   75,   76,   77,   67,    0,    0,    0,   84,    0,
    0,   82,    0,   85,    0,    0,    0,    0,   83,   78,
   79,    0,  298,    0,    0,    0,    0,    0,   80,   81,
   68,    0,   69,  460,    0,    0,    0,    0,   70,    0,
   71,   72,   73,   67,   74,   75,   76,   77,    0,    0,
    0,    0,    0,    0,    0,   84,    0,    0,    0,    0,
   82,    0,   85,   78,   79,    0,  461,   83,    0,   68,
    0,   69,   80,   81,    0,    0,    0,   70,    0,   71,
   72,   73,  467,   74,   75,   76,   77,    0,    0,    0,
    0,    0,    0,   67,    0,    0,    0,    0,    0,   84,
    0,    0,   78,   79,    0,    0,   82,    0,   85,    0,
    0,   80,   81,   83,    0,  298,    0,    0,    0,   68,
    0,   69,    0,    0,    0,    0,    0,   70,  176,   71,
   72,   73,    0,   74,   75,   76,   77,    0,   67,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   84,    0,
    0,    0,   78,   79,    0,    0,    0,    0,    0,    0,
    0,   80,   81,    0,   68,  132,   69,    0,  132,    0,
    0,    0,   70,    0,   71,   72,   73,    0,   74,   75,
   76,   77,  132,  132,    0,   67,   82,  132,   85,    0,
    0,    0,    0,   83,   84,    0,    0,   78,   79,    0,
    0,    0,    0,    0,    0,    0,   80,   81,  214,    0,
    0,   68,    0,   69,    0,    0,    0,  132,    0,   70,
    0,   71,   72,   73,    0,   74,   75,   76,   77,   67,
    0,    0,    0,    0,   82,    0,   85,    0,    0,    0,
    0,   83,    0,    0,   78,   79,    0,    0,  132,  132,
    0,    0,    0,   80,   81,   68,  234,   69,    0,    0,
    0,    0,    0,   70,    0,   71,   72,   73,    0,   74,
   75,   76,   77,    0,   84,    0,    0,    0,   67,    0,
   82,    0,   85,    0,    0,    0,    0,   83,   78,   79,
    0,    0,    0,    0,    0,    0,    0,   80,   81,    0,
    0,    0,  441,    0,   68,    0,   69,    0,    0,    0,
    0,    0,   70,    0,   71,   72,   73,    0,   74,   75,
   76,   77,   84,    0,   67,  161,    0,  161,    0,    0,
    0,    0,  161,    0,  148,  141,    0,   78,   79,  146,
  144,    0,  145,    0,  147,    0,   80,   81,    0,    0,
   68,    0,   69,    0,    0,    0,    0,  142,   70,  143,
   71,   72,   73,    0,   74,   75,   76,   77,   84,   82,
    0,   85,    0,    0,    0,    0,   83,    0,    0,    0,
    0,    0,    0,   78,   79,    0,    0,    0,    0,    0,
    0,  140,   80,   81,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   67,    0,    0,    0,  161,    0,
  161,    0,  161,  161,    0,  161,    0,    0,    0,    0,
    0,  139,    0,    0,  132,  132,    0,    0,    0,    0,
   68,    0,   69,    0,    0,    0,    0,    0,   70,    0,
   71,   72,   73,    0,   74,   75,   76,   77,    0,    0,
  132,    0,   67,    0,    0,    0,    0,   84,    0,    0,
    0,    0,    0,   78,   79,    0,    0,    0,    0,    0,
    0,    0,   80,   81,    0,    0,    0,    0,   68,    0,
   69,    0,    0,    0,    0,    0,   70,    0,   71,   72,
   73,    0,   74,   75,   76,   77,  161,    0,   67,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   78,   79,    0,    0,    0,    0,    0,    0,    0,
   80,   81,    0,    0,   68,    0,   69,    0,    0,    0,
    0,    0,   70,    0,   71,   72,   73,    0,   74,   75,
   76,   77,    0,  161,    0,  148,  141,    0,    0,    0,
  146,  144,    0,  145,    0,  147,    0,   78,   79,    0,
    0,    0,    0,    0,    0,    0,   80,   81,  142,  161,
  143,  161,    0,    0,    0,    0,    0,  161,    0,  161,
  161,  161,    0,  161,  161,  161,  161,   67,    0,    0,
    0,  148,  141,    0,    0,    0,  146,  144,  232,  145,
    0,  147,  161,  161,    0,    0,  132,  133,  134,  135,
    0,  161,  161,   68,  142,   69,  143,  138,    0,  136,
  137,   70,    0,   71,   72,   73,  161,   74,   75,   76,
   77,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   78,   79,  140,    0,
    0,    0,  161,    0,  161,   80,   81,    0,    0,    0,
  161,    0,  161,  161,  161,    0,  161,  161,  161,  161,
    0,    0,    0,    0,    0,    0,    0,    0,  139,    0,
    0,    0,  148,  141,    0,  161,  161,  146,  144,    0,
  145,    0,  147,    0,  161,  161,    0,    0,    0,    0,
    0,    0,    0,  239,    0,  142,    0,  143,  138,  148,
  141,    0,    0,    0,  146,  144,    0,  145,    0,  147,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  142,    0,  143,  138,  148,  141,    0,  140,
    0,  146,  144,    0,  145,    0,  147,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  142,
    0,  143,  138,    0,    0,  283,  140,    0,    0,  139,
    0,    0,  148,  141,    0,    0,    0,  146,  144,    0,
  145,    0,  147,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  284,  140,    0,  142,  139,  143,  138,    0,
    0,  148,  141,    0,    0,    0,  146,  144,    0,  145,
    0,  147,    0,    0,    0,    0,    0,  132,  133,  134,
  135,    0,    0,  139,  142,    0,  143,  138,  301,  140,
  136,  137,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  130,  131,    0,  302,  140,  139,
    0,    0,    0,  132,  133,  134,  135,    0,    0,    0,
    0,    0,    0,  148,  141,    0,  136,  137,  146,  144,
    0,  145,    0,  147,    0,    0,    0,    0,  139,    0,
    0,    0,    0,    0,    0,    0,  142,    0,  143,  138,
    0,    0,  148,  141,    0,    0,    0,  146,  144,    0,
  145,    0,  147,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  142,    0,  143,  138,  305,
  140,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  130,  131,    0,    0,    0,
    0,    0,    0,    0,  132,  133,  134,  135,  306,  140,
  139,    0,    0,    0,    0,    0,    0,  136,  137,    0,
    0,    0,  130,  131,    0,    0,    0,    0,    0,    0,
    0,  132,  133,  134,  135,    0,    0,    0,    0,  139,
    0,    0,    0,    0,  136,  137,    0,    0,    0,  130,
  131,    0,    0,    0,    0,    0,    0,    0,  132,  133,
  134,  135,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  136,  137,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  130,  131,    0,    0,    0,
    0,    0,    0,    0,  132,  133,  134,  135,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  136,  137,    0,
    0,    0,    0,    0,  130,  131,    0,    0,    0,    0,
    0,    0,    0,  132,  133,  134,  135,    0,    0,    0,
    0,    0,    0,  148,  141,    0,  136,  137,  146,  144,
    0,  145,    0,  147,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  142,    0,  143,  138,
  148,  141,    0,    0,    0,  146,  144,    0,  145,    0,
  147,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  142,    0,  143,  130,  131,    0,  342,
  140,    0,    0,    0,    0,  132,  133,  134,  135,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  136,  137,
    0,    0,    0,    0,    0,  130,  131,  140,    0,    0,
  139,    0,    0,    0,  132,  133,  134,  135,    0,    0,
    0,    0,    0,    0,  148,  141,    0,  136,  137,  146,
  144,    0,  145,    0,  147,    0,    0,  139,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  142,    0,  143,
  138,  148,  141,    0,    0,    0,  146,  144,    0,  145,
    0,  147,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  142,    0,  143,  138,  148,  141,
  343,  140,    0,  146,  144,    0,  145,    0,  147,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  142,    0,  143,  138,    0,    0,  345,  140,    0,
    0,  139,    0,    0,  148,  141,    0,    0,    0,  146,
  144,    0,  145,    0,  147,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  346,  140,    0,  142,  139,  143,
  138,    0,    0,  148,  141,    0,    0,    0,  146,  144,
    0,  145,    0,  147,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  139,  142,    0,  143,  138,
  347,  140,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  130,  131,    0,    0,
    0,    0,    0,    0,    0,  132,  133,  134,  135,  376,
  140,  139,    0,    0,    0,    0,  148,  141,  136,  137,
    0,  146,  144,  130,  145,    0,  147,    0,    0,    0,
    0,    0,  132,  133,  134,  135,    0,    0,    0,  142,
  139,  143,  138,  148,  141,  136,  137,    0,  146,  144,
    0,  145,    0,  147,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  142,    0,  143,  138,
  148,  141,  377,  140,    0,  146,  144,    0,  145,    0,
  147,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  142,    0,  143,  138,  130,  131,  380,
  140,    0,    0,  139,    0,    0,  132,  133,  134,  135,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  136,
  137,    0,    0,    0,  130,  131,  413,  140,    0,    0,
  139,    0,    0,  132,  133,  134,  135,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  136,  137,    0,    0,
    0,  130,  131,    0,    0,    0,    0,  139,    0,    0,
  132,  133,  134,  135,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  136,  137,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  130,  131,    0,
    0,    0,    0,    0,    0,    0,  132,  133,  134,  135,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  136,
  137,    0,    0,    0,    0,    0,  130,  131,    0,    0,
    0,    0,    0,    0,    0,  132,  133,  134,  135,  384,
  385,  386,  387,    0,    0,    0,  148,  141,  136,  137,
    0,  146,  144,    0,  145,    0,  147,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  387,  142,
    0,  143,  138,  418,  419,  420,  421,  422,  423,  424,
  425,  426,  427,  428,  429,  430,  431,  432,  433,  130,
  131,    0,    0,    0,    0,    0,    0,    0,  132,  133,
  134,  135,  470,  140,    0,    0,    0,    0,    0,    0,
    0,  136,  137,    0,    0,    0,  130,  131,    0,    0,
    0,    0,    0,    0,    0,  132,  133,  134,  135,    0,
    0,    0,    0,  139,    0,    0,    0,    0,  136,  137,
    0,    0,    0,  130,  131,    0,    0,    0,    0,    0,
    0,    0,  132,  133,  134,  135,    0,    0,    0,    0,
    0,    0,  148,  141,    0,  136,  137,  146,  144,    0,
  145,    0,  147,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  142,    0,  143,  138,  148,
  141,    0,    0,    0,  146,  144,    0,  145,    0,  147,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  142,    0,  143,  138,  148,  141,  471,  140,
    0,  146,  144,    0,  145,    0,  147,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  142,
    0,  143,  138,    0,    0,  475,  140,    0,    0,  139,
    0,    0,    0,    0,    0,  148,  141,    0,    0,    0,
  146,  144,    0,  145,    0,  147,    0,    0,    0,    0,
    0,    0,  476,  140,    0,    0,  139,    0,  142,    0,
  143,  138,  148,  141,    0,    0,    0,  146,  144,    0,
  145,    0,  147,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  139,    0,  142,    0,  143,  138,  130,
  131,  479,  140,    0,    0,    0,    0,    0,  132,  133,
  134,  135,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  136,  137,    0,    0,    0,    0,    0,    0,  140,
  403,  396,  139,    0,  417,  401,  399,    0,  400,    0,
  402,    0,   19,   30,    0,    0,  403,  396,    0,    0,
    0,  401,  399,  397,  400,  398,  402,    0,    0,  139,
    0,    0,    0,    0,    0,    0,    0,  403,  396,  397,
    0,  398,  401,  399,    0,  400,    0,  402,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  395,    0,    0,
  397,    0,  398,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  130,  131,  394,    0,    0,
    0,    0,    0,    0,  132,  133,  134,  135,    0,    0,
   12,    0,   34,  394,   54,    0,    0,  136,  137,    0,
    0,    0,  130,  131,    0,    0,    0,    0,    0,   54,
    0,  132,  133,  134,  135,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  136,  137,    0,    0,    0,  130,
  131,    0,    0,    0,    0,    0,    0,    0,  132,  133,
  134,  135,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  136,  137,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  130,  131,
    0,    0,    0,    0,    0,    0,    0,  132,  133,  134,
  135,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  136,  137,    0,    0,    0,  130,  131,    0,    0,    0,
    0,    0,    0,    0,  132,  133,  134,  135,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  136,  137,    0,
    0,    0,    0,    0,    0,    0,   30,   30,   30,   30,
   30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
   30,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   30,    0,    0,    0,    0,   30,    0,    0,    0,
    0,    0,  388,  389,  390,  391,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  392,  393,   30,  388,  389,
  390,  391,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  392,  393,    0,    0,    0,    0,    0,    0,  388,
  389,  390,  391,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  392,  393,    0,   34,   34,   34,   34,   34,
   34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
   34,   34,    0,    0,    0,    0,    0,    0,    0,    0,
   34,    0,    0,    0,    0,   34,    0,    0,    0,    0,
    0,    0,   55,
};
short yycheck[] = {                                      58,
   18,   58,   58,  116,   58,   40,   38,   40,   42,   41,
  280,  281,   45,   38,   44,   86,   41,   44,   91,   91,
   85,   91,   81,   42,   83,   84,   58,   91,   44,   91,
   89,   90,   59,   58,   41,   38,   43,    0,   41,  110,
  290,   40,  101,   41,  103,  158,  261,  262,   37,  114,
   40,   58,   59,   42,   43,   58,   45,   44,   47,   44,
   58,   42,   94,   93,  290,  281,  316,   40,  127,   94,
  129,  280,   45,  132,  133,  134,  135,  136,  137,   42,
  139,  140,  141,  142,  143,   41,  145,  146,  147,  148,
  316,   94,  124,  126,   59,   41,   94,   43,  157,  124,
   59,   41,   58,  162,   44,  164,   93,  162,   93,  125,
  169,  170,   58,   59,  290,  170,   69,  176,  454,  178,
  179,  124,   44,   40,   37,   78,  124,  186,    2,   42,
   43,    5,   45,  192,   47,   41,  290,   59,   44,   91,
   15,  477,  201,   91,   91,  204,    0,   41,   91,   41,
   44,   91,   44,  126,   91,  214,   43,   91,  292,   40,
  113,  114,  221,  222,   58,   59,   58,   59,  124,  302,
  290,   93,   59,  232,   91,  234,  292,  236,   41,  238,
  239,   44,   57,   44,  243,   91,  245,  246,   42,  292,
  249,  250,  251,  258,  253,  250,   59,   41,   59,   93,
   44,   93,  261,  262,  317,   41,   41,   44,  267,   44,
  283,  283,  271,  283,  273,  168,   41,   44,   43,  283,
  279,  283,   59,   58,   59,  100,  285,  302,   63,  290,
  289,  125,  291,  125,  292,  282,   44,   41,  297,  352,
   44,   41,   37,   38,   44,   40,   41,   42,   43,   44,
   45,   59,   47,  366,   58,   59,  290,  292,   93,  292,
  293,  280,  281,   58,   59,   60,  337,   62,   63,  326,
  326,  290,  326,   44,   40,  302,   37,  309,   40,  338,
  312,   42,  316,  282,  309,   93,   47,  312,   59,   93,
  125,   91,  325,  406,  326,  313,   91,  410,   93,   94,
  290,  326,  265,  266,  267,  268,  269,  270,  271,  272,
  273,  274,  275,  276,  277,  278,  279,  280,  281,  326,
  293,  125,   41,  326,   44,   44,  316,  290,  326,  124,
  125,  283,  295,  322,  323,  283,  283,  302,   40,   59,
  283,  454,   38,  283,  457,   41,  283,   91,   44,  283,
   37,   38,  325,  412,   41,   42,   43,   44,   45,  290,
   47,   59,   58,   59,  477,  282,  283,   63,  125,   44,
  326,   58,   59,   60,   41,   62,   63,  283,   41,  123,
  326,   44,  441,   41,   41,   41,  404,   44,   44,  123,
   44,   41,  451,  452,   44,   37,   59,   93,   94,  458,
   42,  460,   59,   41,   91,   47,   93,   94,  467,  322,
  323,  265,  266,  267,  268,  269,  270,  271,  272,  273,
  274,  275,  276,  277,  278,  279,  280,  281,  124,  125,
   41,   41,  326,   44,  326,   44,  290,  124,  125,   93,
   93,  295,   37,   38,   44,  125,   41,   42,   43,   44,
   45,  125,   47,   44,   41,  298,  299,   40,   41,  302,
   44,   44,   40,   58,   59,   60,  301,   62,   63,   41,
   41,  125,   41,   44,   59,   41,   41,   58,   41,   44,
   91,   41,   41,  283,  290,  282,   40,  282,  283,   38,
   58,  326,   41,  290,  322,  292,   91,   40,   93,   94,
   40,   59,   59,  298,  299,  300,  301,  302,   91,   58,
  261,   60,   59,   62,  309,  310,  311,  312,   59,  262,
   91,   41,  326,   41,   43,  258,   91,  322,  323,  124,
  125,  326,   37,   38,   40,  261,   41,   42,   43,   44,
   45,   38,   47,  371,   41,   94,   43,   41,   45,   41,
   41,   41,   41,   58,   59,   60,   41,   62,   63,   44,
   58,   58,   41,   60,   59,   62,   41,   41,   40,   44,
   44,   59,   41,   59,   41,  124,  290,   37,   38,   41,
  281,   41,   42,   43,   44,   45,   91,   47,   93,   94,
  274,  275,  276,  277,  278,  279,  283,   94,   58,   59,
   60,  125,   62,   63,  300,  301,   91,   41,  123,  125,
  438,  298,  299,  300,  301,  302,   91,   91,  446,  124,
  125,  125,  309,  310,  311,  312,   44,  124,  456,   93,
  326,   91,   41,   93,   94,  322,  323,   37,   38,  326,
   93,   41,   42,   43,   44,   45,  282,   47,   93,   93,
   58,   93,   59,   59,  482,   41,  164,  250,   58,   59,
   60,   37,   62,   63,  124,  125,   42,   43,   37,   45,
    5,   47,  283,   42,   43,   37,   45,  265,   47,  128,
   42,   43,  404,   45,   60,   47,   62,  199,  283,   38,
  100,   91,   41,   93,   94,   42,   -1,   -1,  350,  282,
  283,   -1,   -1,  298,  299,  300,  301,  302,   -1,   58,
   -1,   60,  283,   62,  309,  310,  311,  312,  283,   -1,
   -1,   -1,   -1,   -1,  124,  125,   -1,  322,  323,   -1,
   -1,  326,   37,   38,   -1,   -1,   41,   42,   43,   44,
   45,   37,   47,    3,   -1,   94,   42,   43,   -1,   45,
   -1,   47,   -1,   58,   59,   60,   16,   62,   63,   -1,
  309,  310,  311,  312,   60,   -1,   62,   -1,   -1,   -1,
   -1,   -1,   -1,  322,  323,  124,   -1,  326,  283,   -1,
   -1,   -1,   42,   -1,   -1,   -1,   91,   -1,   93,   94,
   -1,   -1,   -1,  298,  299,  300,  301,  302,  283,   -1,
   -1,   -1,   -1,   63,  309,  310,  311,  312,  283,  283,
   -1,   -1,  309,  310,  311,  312,   -1,  322,  323,  124,
  125,  326,   -1,  283,   -1,  322,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  298,  299,
  300,  301,  302,   -1,   -1,   -1,   -1,   -1,   -1,  309,
  310,  311,  312,   -1,   -1,  115,   -1,   -1,   -1,   -1,
   -1,   -1,  322,  323,   37,   38,  326,   -1,   41,   42,
   43,   44,   45,   -1,   47,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,  283,   -1,   58,   59,   60,   -1,   62,
   63,   -1,   58,   59,   -1,   -1,   -1,   63,  298,  299,
  300,  301,  302,  163,   -1,   -1,   -1,   -1,   -1,  309,
  310,  311,  312,   -1,   -1,   -1,   -1,   -1,   91,   -1,
   93,   94,  322,  323,   -1,   -1,  326,   93,   -1,   -1,
   -1,   -1,   37,   38,  310,  311,   41,   42,   43,   44,
   45,   -1,   47,   -1,   -1,   -1,  322,  323,   -1,   -1,
   -1,  124,  125,   58,   59,   60,   -1,   62,   63,  125,
  309,  310,  311,  312,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  322,  323,   -1,   -1,  326,  283,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,   93,   94,
   -1,   -1,   -1,  298,  299,  300,  301,  302,   -1,   -1,
   -1,   -1,   -1,   -1,  309,  310,  311,  312,   -1,   -1,
   -1,   -1,   -1,  309,  310,  311,  312,  322,  323,  124,
  125,  326,   -1,   -1,   37,   38,  322,  323,   41,   42,
   43,   44,   45,   37,   47,   -1,   -1,   -1,   42,   43,
   -1,   45,   -1,   47,   -1,   58,   59,   60,   -1,   62,
   63,   -1,   -1,   -1,   -1,   -1,   60,   -1,   62,   -1,
   -1,   -1,   38,   -1,   -1,   41,   37,   -1,   -1,   -1,
   -1,   42,   43,   -1,   45,   -1,   47,   -1,   91,   -1,
   93,   94,   58,   -1,   60,   -1,   62,   -1,   -1,   60,
   -1,   62,   -1,   -1,   -1,   -1,   -1,   -1,   37,   38,
   -1,   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,
  283,  124,  125,   -1,   -1,   -1,   -1,   -1,   94,   58,
   59,   60,   -1,   62,   63,  298,  299,  300,  301,  302,
   -1,   -1,   -1,   -1,  300,  301,  309,  310,  311,  312,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  124,  322,
  323,   -1,   91,  326,   93,   94,   -1,   -1,   37,   38,
  326,   40,   41,   42,   43,   44,   45,   -1,   47,  265,
  266,  267,  268,  269,  270,  271,  272,  273,  283,   58,
   59,   60,   -1,   62,   63,  124,  125,   -1,   -1,   -1,
   -1,   -1,   -1,  298,  299,  300,  301,  302,   -1,  295,
   -1,   -1,   -1,   -1,  309,  310,  311,  312,   -1,   -1,
   -1,   -1,   91,   -1,   93,   94,   -1,  322,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   37,   38,   -1,
   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  124,  125,   -1,   58,   59,
   60,   -1,   62,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  283,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   91,   -1,   93,   94,  298,  299,  300,  301,  302,
   -1,   -1,  123,   -1,   -1,   -1,  309,  310,  311,  312,
   -1,   -1,   -1,   -1,   -1,  309,  310,  311,  312,  322,
  323,   -1,   -1,  326,  124,  125,   -1,   -1,  322,  323,
   37,   38,   -1,   -1,   41,   42,   43,   44,   45,   -1,
   47,   -1,   -1,  309,  310,  311,  312,   -1,   -1,  310,
  311,   58,   59,   60,  283,   62,   63,   -1,   -1,   -1,
  326,  322,  323,   -1,   -1,   -1,   -1,   -1,   -1,  298,
  299,  300,  301,  302,   -1,   -1,   -1,   -1,   -1,   -1,
  309,  310,  311,  312,   91,   -1,   93,   94,   -1,   -1,
   -1,   -1,   -1,  322,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   37,   38,   -1,   -1,   41,   42,   43,   44,
   45,   -1,   47,  282,  283,   -1,   -1,  124,  125,   -1,
   -1,   -1,   -1,   58,   59,   60,   -1,   62,   63,   -1,
   -1,  300,  301,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  309,  310,  311,  312,  265,  266,  267,  268,  269,  270,
  271,  272,  273,  322,  323,   -1,   91,  326,   93,   94,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   37,   38,   -1,  295,   41,   42,   43,   44,   45,
   -1,   47,  282,  283,   -1,   -1,   -1,   -1,   -1,  124,
  125,   -1,   58,   59,   60,   -1,   62,   63,   -1,   -1,
  300,  301,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  309,
  310,  311,  312,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  322,  323,   -1,   91,  326,   93,   94,   -1,
   37,   38,   -1,   -1,   41,   42,   43,   44,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   59,   60,   -1,   62,   63,   -1,  124,  125,
   -1,   -1,   -1,   -1,   -1,   37,   38,   -1,   -1,   -1,
   42,   43,   44,   45,   -1,   47,  283,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   93,   94,   60,   -1,
   62,   63,   -1,  300,  301,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  309,  310,  311,  312,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  322,  323,  124,  125,  326,
   -1,   93,   94,   -1,   37,   38,   -1,   -1,   -1,   42,
   43,   44,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,  283,   62,
   63,   -1,  124,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  300,  301,   -1,   41,   -1,
   -1,   44,   -1,   -1,  309,  310,  311,  312,   -1,   -1,
   93,   94,   -1,   -1,   -1,   58,   59,  322,  323,   -1,
   63,  326,   -1,   37,   38,   -1,   -1,   -1,   42,   43,
   44,   45,   38,   47,   -1,   41,   -1,   43,   -1,   45,
   -1,  124,   -1,   -1,   -1,   -1,   60,  283,   62,   63,
   93,   94,   58,   -1,   60,   -1,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  300,  301,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  309,  310,  311,  312,   -1,   -1,   93,
   94,  124,  125,   -1,   -1,   -1,  322,  323,   94,   -1,
  326,   37,   38,   -1,   -1,   -1,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  124,   -1,   -1,   -1,   60,   -1,   62,   63,  124,   38,
   -1,   -1,   41,  300,  301,   44,   -1,   -1,   -1,   -1,
   -1,   -1,  309,  310,  311,  312,   -1,   -1,   -1,   58,
   59,   60,   -1,   62,   63,  322,  323,   93,   94,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  300,  301,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,  311,
  312,   -1,   -1,   -1,   93,   94,   38,   -1,  124,   41,
  322,  323,   44,   -1,  326,   37,   38,   -1,   -1,   -1,
   42,   43,   -1,   45,   -1,   47,   58,   59,   -1,   -1,
   -1,   63,   -1,   -1,   -1,  124,  125,   -1,   60,   -1,
   62,   63,   -1,   -1,   -1,   -1,   -1,  300,  301,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,  311,  312,
   -1,   93,   94,   -1,   -1,   -1,   -1,   -1,   -1,  322,
  323,   93,   94,  326,   37,   38,   -1,   -1,   -1,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,  300,  301,   -1,
   -1,   -1,  124,  125,   -1,   -1,   -1,   60,   -1,   62,
   63,   -1,  124,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  326,   -1,   -1,  300,  301,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  309,  310,  311,  312,   -1,
   93,   94,   -1,  309,  310,  311,  312,   -1,  322,  323,
   -1,   -1,  326,   37,   38,   -1,  322,  323,   42,   43,
  326,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  124,   -1,   -1,   -1,   -1,   60,   -1,   62,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  300,  301,   -1,   -1,   -1,   38,
   -1,   40,   -1,  309,  310,  311,  312,   -1,   -1,   93,
   94,   -1,   -1,   -1,   -1,   -1,  322,  323,   37,   38,
  326,  300,  301,   42,   43,   -1,   45,   -1,   47,   -1,
  309,  310,  311,  312,   -1,   -1,   -1,   -1,   -1,   38,
  124,   60,   41,   62,   63,   44,   38,  326,   -1,   41,
   -1,   43,   44,   45,   -1,   -1,   -1,   -1,   -1,   58,
   59,   60,   -1,   62,   63,   -1,   58,   59,   60,   -1,
   62,   63,   -1,   -1,   93,   94,   -1,   -1,  300,  301,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  309,  300,  301,
  312,   -1,   -1,   -1,   93,   94,   -1,  309,  310,  311,
  312,   93,   94,   -1,  326,  124,   -1,   -1,   -1,   -1,
  322,  323,   -1,   -1,  326,   -1,   38,   -1,   -1,   41,
   -1,   43,   44,   45,   -1,  124,  125,   -1,   -1,   -1,
   -1,   -1,  124,  125,   -1,   -1,   58,   59,   60,   -1,
   62,   63,   -1,   38,   -1,   -1,   41,  300,  301,   44,
   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,  311,  312,
   -1,   -1,   -1,   58,   59,   60,   -1,   62,   63,  322,
  323,   93,   94,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   38,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   93,   94,
   -1,   -1,  124,  125,   -1,   58,   59,   60,   -1,   62,
   63,   -1,   38,   -1,   -1,   41,  300,  301,   44,   -1,
   -1,   -1,   -1,   -1,   -1,  309,  310,  311,  312,  124,
  125,   -1,   58,   59,   60,   -1,   62,   63,  322,  323,
   93,   94,  326,  282,   -1,  284,   -1,   -1,   -1,   -1,
   -1,  290,   -1,  292,  293,  294,   -1,  296,  297,   -1,
   -1,   38,   -1,   40,   -1,   -1,   -1,   93,   94,   -1,
   -1,  124,  125,   -1,   -1,   -1,  315,  316,   -1,   -1,
   -1,  300,  301,   -1,   -1,  324,   -1,   -1,   -1,   -1,
  309,  310,  311,  312,   -1,   -1,   -1,   -1,  124,  125,
   -1,  300,  301,  322,  323,   -1,   -1,  326,  300,  301,
  309,  310,  311,  312,   -1,   -1,   -1,  309,  310,  311,
  312,   -1,   -1,  322,  323,   -1,   38,  326,   -1,   41,
  322,  323,   44,   -1,  326,   -1,   -1,   -1,   -1,   38,
   -1,   -1,   41,   -1,   -1,   44,   58,   59,   60,   -1,
   62,   63,   38,   -1,   -1,   41,   -1,   -1,   -1,   58,
   59,   37,   38,   -1,   63,   -1,   42,   43,   -1,   45,
   -1,   47,   58,   -1,   60,   -1,   62,   -1,  300,  301,
   -1,   93,   94,   -1,   60,   -1,   62,  309,  310,  311,
  312,   -1,   -1,   -1,   93,   94,   -1,   -1,   -1,   -1,
  322,  323,   -1,   -1,  326,  300,  301,   -1,   94,   -1,
   -1,   -1,  124,  125,  309,  310,  311,  312,   94,   -1,
   38,   -1,   -1,   41,   -1,  124,  125,  322,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,  124,   -1,
   58,   -1,   60,   38,   62,   -1,   41,  300,  301,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,  311,  312,
   -1,   -1,   -1,   58,   -1,   60,   -1,   62,   -1,   -1,
   -1,   -1,   -1,  326,  300,  301,   94,   -1,   -1,   -1,
   38,   -1,   40,  309,  310,  311,  312,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   94,
  326,   59,   -1,   -1,   -1,  282,  124,  284,   -1,   -1,
   -1,   -1,   -1,  290,   -1,  292,  293,  294,   -1,  296,
  297,   -1,   -1,   -1,   38,   -1,   40,   -1,   -1,  124,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,  315,  316,
   -1,   -1,   -1,   -1,   -1,   59,   -1,  324,  265,  266,
  267,  268,  269,  270,  271,  272,  273,  274,  275,  276,
  277,  278,  279,   -1,   -1,  123,   -1,  125,  126,   38,
   -1,   40,   -1,   -1,   -1,   -1,   45,   -1,  295,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  300,  301,
   59,   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,  311,
  312,  300,  301,   -1,   -1,   -1,   -1,   -1,   -1,  123,
  309,  125,  126,  312,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  309,  310,  311,  312,  326,   -1,   -1,
   -1,   -1,   -1,  309,  310,  311,  312,   -1,   -1,   -1,
  326,   -1,   38,   -1,   40,   -1,  322,  323,   -1,   45,
   -1,   -1,   -1,   -1,  123,   -1,  125,  126,   -1,   -1,
   -1,   37,   38,   59,   -1,   -1,   42,   43,   -1,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   60,   -1,   62,   -1,   -1,   -1,
   -1,  309,  310,  311,  312,   -1,   -1,   -1,  256,  257,
   -1,  259,  260,  261,  262,  263,  264,   -1,  326,   38,
   -1,   40,   -1,   -1,  309,  310,  311,  312,   94,   -1,
   -1,   -1,   -1,   -1,  282,   -1,  284,  123,   -1,   -1,
  126,  326,  290,  291,  292,  293,  294,   -1,  296,  297,
  298,  299,  256,  257,   -1,  259,  260,  261,  262,  263,
  264,   -1,   38,   -1,   40,  313,  314,  315,  316,   45,
   -1,   -1,   -1,   -1,   -1,   -1,  324,  325,  282,   -1,
  284,   -1,   -1,   -1,   60,   -1,  290,  291,  292,  293,
  294,   -1,  296,  297,  298,  299,   -1,  256,  257,   -1,
  259,  260,   -1,   -1,  263,  264,   38,   -1,   40,  313,
  314,  315,  316,   45,   -1,   -1,   -1,   93,   -1,   -1,
  324,  325,   -1,  282,   -1,  284,   -1,   -1,   60,   -1,
   -1,  290,  291,  292,  293,  294,   -1,  296,  297,  298,
  299,   -1,   -1,   -1,   -1,   38,   -1,   40,   -1,   -1,
  126,   -1,   45,   -1,  313,  314,  315,  316,   -1,   -1,
   -1,   93,   -1,   -1,   -1,  324,  325,   60,   -1,   -1,
  256,  257,   -1,  259,  260,   -1,   -1,  263,  264,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  126,   38,  282,   40,  284,   -1,
   93,   -1,   45,   -1,  290,  291,  292,  293,  294,   -1,
  296,  297,  298,  299,   -1,   -1,   -1,   60,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  313,  314,  315,
  316,   -1,   -1,  126,   -1,   -1,   -1,   -1,  324,  325,
   38,   -1,   40,  309,  310,  311,  312,   45,   -1,   -1,
   93,   -1,   -1,   -1,   -1,   -1,  322,  323,   -1,   -1,
   -1,   -1,   60,  282,   -1,  284,   -1,   -1,   -1,   -1,
   -1,  290,   -1,  292,  293,  294,   -1,  296,  297,   -1,
  256,   -1,   -1,  126,   -1,   -1,   -1,   38,   -1,   40,
   -1,   -1,   -1,   -1,   45,   93,  315,  316,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  324,  282,   -1,  284,   60,
   -1,   -1,   -1,   -1,  290,   -1,  292,  293,  294,   -1,
  296,  297,  298,  299,  256,   -1,   -1,   -1,  126,   -1,
   -1,   38,   -1,   40,   -1,   -1,   -1,   -1,   45,  315,
  316,   -1,   93,   -1,   -1,   -1,   -1,   -1,  324,  325,
  282,   -1,  284,   60,   -1,   -1,   -1,   -1,  290,   -1,
  292,  293,  294,  256,  296,  297,  298,  299,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  126,   -1,   -1,   -1,   -1,
   38,   -1,   40,  315,  316,   -1,   93,   45,   -1,  282,
   -1,  284,  324,  325,   -1,   -1,   -1,  290,   -1,  292,
  293,  294,   60,  296,  297,  298,  299,   -1,   -1,   -1,
   -1,   -1,   -1,  256,   -1,   -1,   -1,   -1,   -1,  126,
   -1,   -1,  315,  316,   -1,   -1,   38,   -1,   40,   -1,
   -1,  324,  325,   45,   -1,   93,   -1,   -1,   -1,  282,
   -1,  284,   -1,   -1,   -1,   -1,   -1,  290,   60,  292,
  293,  294,   -1,  296,  297,  298,  299,   -1,  256,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  126,   -1,
   -1,   -1,  315,  316,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  324,  325,   -1,  282,   41,  284,   -1,   44,   -1,
   -1,   -1,  290,   -1,  292,  293,  294,   -1,  296,  297,
  298,  299,   58,   59,   -1,  256,   38,   63,   40,   -1,
   -1,   -1,   -1,   45,  126,   -1,   -1,  315,  316,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  324,  325,   60,   -1,
   -1,  282,   -1,  284,   -1,   -1,   -1,   93,   -1,  290,
   -1,  292,  293,  294,   -1,  296,  297,  298,  299,  256,
   -1,   -1,   -1,   -1,   38,   -1,   40,   -1,   -1,   -1,
   -1,   45,   -1,   -1,  315,  316,   -1,   -1,  124,  125,
   -1,   -1,   -1,  324,  325,  282,   60,  284,   -1,   -1,
   -1,   -1,   -1,  290,   -1,  292,  293,  294,   -1,  296,
  297,  298,  299,   -1,  126,   -1,   -1,   -1,  256,   -1,
   38,   -1,   40,   -1,   -1,   -1,   -1,   45,  315,  316,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,  325,   -1,
   -1,   -1,   60,   -1,  282,   -1,  284,   -1,   -1,   -1,
   -1,   -1,  290,   -1,  292,  293,  294,   -1,  296,  297,
  298,  299,  126,   -1,  256,   38,   -1,   40,   -1,   -1,
   -1,   -1,   45,   -1,   37,   38,   -1,  315,  316,   42,
   43,   -1,   45,   -1,   47,   -1,  324,  325,   -1,   -1,
  282,   -1,  284,   -1,   -1,   -1,   -1,   60,  290,   62,
  292,  293,  294,   -1,  296,  297,  298,  299,  126,   38,
   -1,   40,   -1,   -1,   -1,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,  315,  316,   -1,   -1,   -1,   -1,   -1,
   -1,   94,  324,  325,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  256,   -1,   -1,   -1,   38,   -1,
   40,   -1,  125,  126,   -1,   45,   -1,   -1,   -1,   -1,
   -1,  124,   -1,   -1,  300,  301,   -1,   -1,   -1,   -1,
  282,   -1,  284,   -1,   -1,   -1,   -1,   -1,  290,   -1,
  292,  293,  294,   -1,  296,  297,  298,  299,   -1,   -1,
  326,   -1,  256,   -1,   -1,   -1,   -1,  126,   -1,   -1,
   -1,   -1,   -1,  315,  316,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  324,  325,   -1,   -1,   -1,   -1,  282,   -1,
  284,   -1,   -1,   -1,   -1,   -1,  290,   -1,  292,  293,
  294,   -1,  296,  297,  298,  299,  126,   -1,  256,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  315,  316,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  324,  325,   -1,   -1,  282,   -1,  284,   -1,   -1,   -1,
   -1,   -1,  290,   -1,  292,  293,  294,   -1,  296,  297,
  298,  299,   -1,  256,   -1,   37,   38,   -1,   -1,   -1,
   42,   43,   -1,   45,   -1,   47,   -1,  315,  316,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  324,  325,   60,  282,
   62,  284,   -1,   -1,   -1,   -1,   -1,  290,   -1,  292,
  293,  294,   -1,  296,  297,  298,  299,  256,   -1,   -1,
   -1,   37,   38,   -1,   -1,   -1,   42,   43,   44,   45,
   -1,   47,  315,  316,   -1,   -1,  309,  310,  311,  312,
   -1,  324,  325,  282,   60,  284,   62,   63,   -1,  322,
  323,  290,   -1,  292,  293,  294,  256,  296,  297,  298,
  299,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  315,  316,   94,   -1,
   -1,   -1,  282,   -1,  284,  324,  325,   -1,   -1,   -1,
  290,   -1,  292,  293,  294,   -1,  296,  297,  298,  299,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  124,   -1,
   -1,   -1,   37,   38,   -1,  315,  316,   42,   43,   -1,
   45,   -1,   47,   -1,  324,  325,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   60,   -1,   62,   63,   37,
   38,   -1,   -1,   -1,   42,   43,   -1,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   60,   -1,   62,   63,   37,   38,   -1,   94,
   -1,   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,
   -1,   62,   63,   -1,   -1,   93,   94,   -1,   -1,  124,
   -1,   -1,   37,   38,   -1,   -1,   -1,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   93,   94,   -1,   60,  124,   62,   63,   -1,
   -1,   37,   38,   -1,   -1,   -1,   42,   43,   -1,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,  309,  310,  311,
  312,   -1,   -1,  124,   60,   -1,   62,   63,   93,   94,
  322,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  300,  301,   -1,   93,   94,  124,
   -1,   -1,   -1,  309,  310,  311,  312,   -1,   -1,   -1,
   -1,   -1,   -1,   37,   38,   -1,  322,  323,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,  124,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   60,   -1,   62,   63,
   -1,   -1,   37,   38,   -1,   -1,   -1,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   60,   -1,   62,   63,   93,
   94,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  300,  301,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  309,  310,  311,  312,   93,   94,
  124,   -1,   -1,   -1,   -1,   -1,   -1,  322,  323,   -1,
   -1,   -1,  300,  301,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  309,  310,  311,  312,   -1,   -1,   -1,   -1,  124,
   -1,   -1,   -1,   -1,  322,  323,   -1,   -1,   -1,  300,
  301,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,
  311,  312,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  322,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  300,  301,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  309,  310,  311,  312,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  322,  323,   -1,
   -1,   -1,   -1,   -1,  300,  301,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  309,  310,  311,  312,   -1,   -1,   -1,
   -1,   -1,   -1,   37,   38,   -1,  322,  323,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   60,   -1,   62,   63,
   37,   38,   -1,   -1,   -1,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   60,   -1,   62,  300,  301,   -1,   93,
   94,   -1,   -1,   -1,   -1,  309,  310,  311,  312,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  322,  323,
   -1,   -1,   -1,   -1,   -1,  300,  301,   94,   -1,   -1,
  124,   -1,   -1,   -1,  309,  310,  311,  312,   -1,   -1,
   -1,   -1,   -1,   -1,   37,   38,   -1,  322,  323,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,  124,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,   -1,   62,
   63,   37,   38,   -1,   -1,   -1,   42,   43,   -1,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   60,   -1,   62,   63,   37,   38,
   93,   94,   -1,   42,   43,   -1,   45,   -1,   47,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   60,   -1,   62,   63,   -1,   -1,   93,   94,   -1,
   -1,  124,   -1,   -1,   37,   38,   -1,   -1,   -1,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   93,   94,   -1,   60,  124,   62,
   63,   -1,   -1,   37,   38,   -1,   -1,   -1,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  124,   60,   -1,   62,   63,
   93,   94,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  300,  301,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  309,  310,  311,  312,   93,
   94,  124,   -1,   -1,   -1,   -1,   37,   38,  322,  323,
   -1,   42,   43,  300,   45,   -1,   47,   -1,   -1,   -1,
   -1,   -1,  309,  310,  311,  312,   -1,   -1,   -1,   60,
  124,   62,   63,   37,   38,  322,  323,   -1,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   60,   -1,   62,   63,
   37,   38,   93,   94,   -1,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   60,   -1,   62,   63,  300,  301,   93,
   94,   -1,   -1,  124,   -1,   -1,  309,  310,  311,  312,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  322,
  323,   -1,   -1,   -1,  300,  301,   93,   94,   -1,   -1,
  124,   -1,   -1,  309,  310,  311,  312,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  322,  323,   -1,   -1,
   -1,  300,  301,   -1,   -1,   -1,   -1,  124,   -1,   -1,
  309,  310,  311,  312,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  322,  323,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  300,  301,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,  311,  312,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  322,
  323,   -1,   -1,   -1,   -1,   -1,  300,  301,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  309,  310,  311,  312,  354,
  355,  356,  357,   -1,   -1,   -1,   37,   38,  322,  323,
   -1,   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,   60,
   -1,   62,   63,  388,  389,  390,  391,  392,  393,  394,
  395,  396,  397,  398,  399,  400,  401,  402,  403,  300,
  301,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,
  311,  312,   93,   94,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  322,  323,   -1,   -1,   -1,  300,  301,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  309,  310,  311,  312,   -1,
   -1,   -1,   -1,  124,   -1,   -1,   -1,   -1,  322,  323,
   -1,   -1,   -1,  300,  301,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  309,  310,  311,  312,   -1,   -1,   -1,   -1,
   -1,   -1,   37,   38,   -1,  322,  323,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   60,   -1,   62,   63,   37,
   38,   -1,   -1,   -1,   42,   43,   -1,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   60,   -1,   62,   63,   37,   38,   93,   94,
   -1,   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,
   -1,   62,   63,   -1,   -1,   93,   94,   -1,   -1,  124,
   -1,   -1,   -1,   -1,   -1,   37,   38,   -1,   -1,   -1,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,   -1,   93,   94,   -1,   -1,  124,   -1,   60,   -1,
   62,   63,   37,   38,   -1,   -1,   -1,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  124,   -1,   60,   -1,   62,   63,  300,
  301,   93,   94,   -1,   -1,   -1,   -1,   -1,  309,  310,
  311,  312,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  322,  323,   -1,   -1,   -1,   -1,   -1,   -1,   94,
   37,   38,  124,   -1,   41,   42,   43,   -1,   45,   -1,
   47,   -1,   41,   42,   -1,   -1,   37,   38,   -1,   -1,
   -1,   42,   43,   60,   45,   62,   47,   -1,   -1,  124,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   37,   38,   60,
   -1,   62,   42,   43,   -1,   45,   -1,   47,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   94,   -1,   -1,
   60,   -1,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   94,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  300,  301,  124,   -1,   -1,
   -1,   -1,   -1,   -1,  309,  310,  311,  312,   -1,   -1,
   40,   -1,   42,  124,   44,   -1,   -1,  322,  323,   -1,
   -1,   -1,  300,  301,   -1,   -1,   -1,   -1,   -1,   59,
   -1,  309,  310,  311,  312,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  322,  323,   -1,   -1,   -1,  300,
  301,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,
  311,  312,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  322,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  300,  301,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,  311,
  312,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  322,  323,   -1,   -1,   -1,  300,  301,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  309,  310,  311,  312,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  322,  323,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  265,  266,  267,  268,
  269,  270,  271,  272,  273,  274,  275,  276,  277,  278,
  279,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  290,   -1,   -1,   -1,   -1,  295,   -1,   -1,   -1,
   -1,   -1,  309,  310,  311,  312,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  322,  323,  316,  309,  310,
  311,  312,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  322,  323,   -1,   -1,   -1,   -1,   -1,   -1,  309,
  310,  311,  312,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  322,  323,   -1,  265,  266,  267,  268,  269,
  270,  271,  272,  273,  274,  275,  276,  277,  278,  279,
  280,  281,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  290,   -1,   -1,   -1,   -1,  295,   -1,   -1,   -1,   -1,
   -1,   -1,  302,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 387
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"'%'","'&'",0,"'('","')'","'*'","'+'","','","'-'",0,"'/'",0,0,0,0,0,0,0,0,
0,0,"':'","';'","'<'",0,"'>'","'?'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,"'['",0,"']'","'^'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"'{'","'|'","'}'","'~'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"F_IF","F_ELSE","F_CASE",
"F_DEFAULT","F_WHILE","F_DO","F_FOR","F_CONTINUE","F_INT","F_STATUS",
"F_STRING_DECL","F_FLOAT_DECL","F_OBJECT","F_CLOSURE_DECL","F_SYMBOL_DECL",
"F_VOID","F_MIXED","F_STATIC","F_PRIVATE","F_NO_MASK","F_PROTECTED","F_PUBLIC",
"F_VARARGS","F_VIRTUAL","F_INHERIT","F_COLON_COLON","F_ARROW",
"F_QUOTED_AGGREGATE","F_ILLEGAL","F_DUMMY","F_ESCAPE","F_TEFUN","F_VEFUN",
"F_IDENTIFIER","F_RETURN","F_STRING","F_NUMBER","F_FLOAT","F_MAPPING",
"F_CLOSURE","F_SYMBOL","F_INC","F_DEC","F_LAND","F_LOR","F_ASSIGN","F_ADD",
"F_SUBTRACT","F_MULTIPLY","F_DIVIDE","F_LT","F_GT","F_EQ","F_GE","F_LE","F_NE",
"F_BREAK","F_SWITCH","F_SSCANF","F_LOCAL","F_MOD","F_COMPL","F_AND","F_OR",
"F_XOR","F_LSH","F_RSH","F_CATCH","F_NOT","F_RANGE","F_POP_VALUE","F_DUP",
"F_CALL_FUNCTION_BY_ADDRESS","F_CALL_EXPLICIT_INHERITED",
"F_PUSH_IDENTIFIER_LVALUE","F_VIRTUAL_VARIABLE",
"F_PUSH_VIRTUAL_VARIABLE_LVALUE","F_IDENTIFIER16","F_PUSH_IDENTIFIER16_LVALUE",
"F_PUSH_LOCAL_VARIABLE_LVALUE","F_PUSH_INDEXED_LVALUE","F_PUSH_RINDEXED_LVALUE",
"F_INDEX_LVALUE","F_RINDEX_LVALUE","F_INDEX","F_RINDEX","F_CONST0","F_CONST1",
"F_LBRANCH","F_LBRANCH_WHEN_ZERO","F_LBRANCH_WHEN_NON_ZERO","F_BRANCH",
"F_BRANCH_WHEN_ZERO","F_BRANCH_WHEN_NON_ZERO","F_BBRANCH_WHEN_ZERO",
"F_BBRANCH_WHEN_NON_ZERO","F_SIMUL_EFUN","F_RETURN0","F_PRE_INC","F_PRE_DEC",
"F_POST_INC","F_POST_DEC","F_CSTRING0","F_CSTRING1","F_CSTRING2","F_CSTRING3",
"F_CLIT","F_NCLIT","F_VOID_ASSIGN","F_VOID_ADD_EQ","F_ADD_EQ","F_SUB_EQ",
"F_DIV_EQ","F_MULT_EQ","F_MOD_EQ","F_AND_EQ","F_OR_EQ","F_XOR_EQ","F_LSH_EQ",
"F_RSH_EQ","F_RANGE_LVALUE","F_AGGREGATE","F_EXTRACT2","F_PREVIOUS_OBJECT0",
"F_LAMBDA_CCONSTANT","F_LAMBDA_CONSTANT","F_M_AGGREGATE","F_M_CAGGREGATE",
"F_MAP_INDEX","F_PUSH_INDEXED_MAP_LVALUE","F_JUMP",
};
char *yyrule[] = {
"$accept : all",
"all : program",
"program : program def possible_semi_colon",
"program :",
"possible_semi_colon :",
"possible_semi_colon : ';'",
"inheritance : inheritance_qualifiers F_INHERIT string_constant ';'",
"optional_star :",
"optional_star : '*'",
"$$1 :",
"function_body : $$1 block",
"function_body : ';'",
"$$2 :",
"$$3 :",
"def : type optional_star F_IDENTIFIER $$2 '(' argument ')' $$3 function_body",
"def : type name_list ';'",
"def : inheritance",
"new_arg_name : type optional_star F_IDENTIFIER",
"new_arg_name : type optional_star F_LOCAL",
"argument :",
"argument : argument_list",
"argument_list : new_arg_name",
"argument_list : argument_list ',' new_arg_name",
"type_modifier : F_NO_MASK",
"type_modifier : F_STATIC",
"type_modifier : F_PRIVATE",
"type_modifier : F_PUBLIC",
"type_modifier : F_VARARGS",
"type_modifier : F_PROTECTED",
"inheritance_modifier : F_VIRTUAL",
"type_modifier_list :",
"type_modifier_list : type_modifier_list type_modifier",
"inheritance_modifier_list : type_modifier_list",
"inheritance_modifier_list : inheritance_modifier_list inheritance_modifier type_modifier_list",
"inheritance_qualifier : type optional_star F_IDENTIFIER",
"inheritance_qualifiers : inheritance_modifier_list",
"inheritance_qualifiers : inheritance_qualifier inheritance_qualifiers",
"type : type_modifier_list opt_basic_type",
"cast : '(' basic_type optional_star ')'",
"decl_cast : '(' '{' basic_type optional_star '}' ')'",
"opt_basic_type : basic_type",
"opt_basic_type :",
"basic_type : F_STATUS",
"basic_type : F_INT",
"basic_type : F_STRING_DECL",
"basic_type : F_OBJECT",
"basic_type : F_CLOSURE_DECL",
"basic_type : F_SYMBOL_DECL",
"basic_type : F_FLOAT_DECL",
"basic_type : F_MAPPING",
"basic_type : F_VOID",
"basic_type : F_MIXED",
"name_list : new_name",
"name_list : name_list ',' new_name",
"new_name : optional_star F_IDENTIFIER",
"$$4 :",
"new_name : optional_star F_IDENTIFIER $$4 F_ASSIGN expr0",
"block : '{' local_declarations statements '}'",
"local_declarations :",
"local_declarations : local_declarations basic_type local_name_list ';'",
"new_local_name : optional_star F_IDENTIFIER",
"local_name_list : new_local_name",
"local_name_list : local_name_list ',' new_local_name",
"statements :",
"statements : statements statement",
"statement : comma_expr ';'",
"statement : error ';'",
"statement : cond",
"statement : while",
"statement : do",
"statement : for",
"statement : switch",
"statement : case",
"statement : default",
"statement : return ';'",
"statement : block",
"statement : ';'",
"statement : F_BREAK ';'",
"statement : F_CONTINUE ';'",
"$$5 :",
"$$6 :",
"while : $$5 F_WHILE '(' comma_expr ')' $$6 statement",
"$$7 :",
"$$8 :",
"do : $$7 F_DO statement F_WHILE $$8 '(' comma_expr ')' ';'",
"$$9 :",
"$$10 :",
"$$11 :",
"$$12 :",
"for : F_FOR '(' $$9 for_expr ';' $$10 for_expr ';' $$11 for_expr ')' $$12 statement",
"for_expr :",
"for_expr : comma_expr",
"$$13 :",
"switch : F_SWITCH '(' comma_expr ')' $$13 statement",
"case : F_CASE case_label ':'",
"case : F_CASE case_label F_RANGE case_label ':'",
"case_label : constant",
"case_label : string_constant",
"constant : constant '|' constant",
"constant : constant '^' constant",
"constant : constant '&' constant",
"constant : constant F_EQ constant",
"constant : constant F_NE constant",
"constant : constant '>' constant",
"constant : constant F_GE constant",
"constant : constant '<' constant",
"constant : constant F_LE constant",
"constant : constant F_LSH constant",
"constant : constant F_RSH constant",
"constant : constant '+' constant",
"constant : constant '-' constant",
"constant : constant '*' constant",
"constant : constant '%' constant",
"constant : constant '/' constant",
"constant : '(' constant ')'",
"constant : '-' constant",
"constant : F_NOT constant",
"constant : '~' constant",
"constant : F_NUMBER",
"default : F_DEFAULT ':'",
"comma_expr : expr0",
"$$14 :",
"comma_expr : comma_expr $$14 ',' expr0",
"expr0 : lvalue F_ASSIGN expr0",
"expr0 : error F_ASSIGN expr0",
"$$15 :",
"$$16 :",
"expr0 : expr0 '?' $$15 expr0 $$16 ':' expr0",
"$$17 :",
"expr0 : expr0 F_LOR $$17 expr0",
"$$18 :",
"expr0 : expr0 F_LAND $$18 expr0",
"expr0 : expr0 '|' expr0",
"expr0 : expr0 '^' expr0",
"expr0 : expr0 '&' expr0",
"expr0 : expr0 F_EQ expr0",
"expr0 : expr0 F_NE expr0",
"expr0 : expr0 '>' expr0",
"expr0 : expr0 F_GE expr0",
"expr0 : expr0 '<' expr0",
"expr0 : expr0 F_LE expr0",
"expr0 : expr0 F_LSH expr0",
"expr0 : expr0 F_RSH expr0",
"$$19 :",
"expr0 : expr0 '+' $$19 expr0",
"expr0 : expr0 '-' expr0",
"expr0 : expr0 '*' expr0",
"expr0 : expr0 '%' expr0",
"expr0 : expr0 '/' expr0",
"expr0 : decl_cast expr0",
"expr0 : cast expr0",
"expr0 : pre_inc_dec F_IDENTIFIER",
"expr0 : pre_inc_dec F_LOCAL",
"expr0 : pre_inc_dec expr4 '[' expr0 ']'",
"expr0 : pre_inc_dec expr4 '[' '<' expr0 ']'",
"expr0 : F_NOT expr0",
"expr0 : '~' expr0",
"expr0 : '-' expr0",
"expr0 : lvalue F_INC",
"expr0 : lvalue F_DEC",
"expr0 : expr4",
"note_start :",
"expr4 : function_call",
"expr4 : F_STRING",
"expr4 : F_NUMBER",
"expr4 : F_CLOSURE",
"expr4 : F_SYMBOL",
"expr4 : F_FLOAT",
"expr4 : '(' note_start comma_expr ')'",
"expr4 : catch",
"expr4 : sscanf",
"expr4 : '(' '{' note_start expr_list '}' ')'",
"expr4 : F_QUOTED_AGGREGATE note_start expr_list '}' ')'",
"expr4 : '(' '[' note_start m_expr_list ']' ')'",
"expr4 : expr4 '[' expr0 F_RANGE expr0 ']'",
"expr4 : expr4 '[' expr0 F_RANGE '<' expr0 ']'",
"expr4 : expr4 '[' '<' expr0 F_RANGE expr0 ']'",
"expr4 : expr4 '[' '<' expr0 F_RANGE '<' expr0 ']'",
"expr4 : expr4 '[' expr0 F_RANGE ']'",
"expr4 : expr4 '[' '<' expr0 F_RANGE ']'",
"expr4 : '&' F_IDENTIFIER",
"expr4 : '&' F_LOCAL",
"expr4 : '&' '(' expr4 '[' expr0 ']' ')'",
"expr4 : '&' '(' expr4 '[' '<' expr0 ']' ')'",
"expr4 : '&' '(' expr4 '[' expr0 ',' expr0 ']' ')'",
"expr4 : '&' '(' expr4 '[' expr0 F_RANGE expr0 ']' ')'",
"expr4 : '&' '(' expr4 '[' expr0 F_RANGE '<' expr0 ']' ')'",
"expr4 : '&' '(' expr4 '[' '<' expr0 F_RANGE expr0 ']' ')'",
"expr4 : '&' '(' expr4 '[' '<' expr0 F_RANGE '<' expr0 ']' ')'",
"expr4 : F_IDENTIFIER",
"expr4 : F_LOCAL",
"expr4 : expr4 '[' expr0 ']'",
"expr4 : expr4 '[' '<' expr0 ']'",
"expr4 : expr4 '[' expr0 ',' expr0 ']'",
"pre_inc_dec : F_INC",
"pre_inc_dec : F_DEC",
"return : F_RETURN",
"return : F_RETURN comma_expr",
"expr_list :",
"expr_list : expr_list2",
"expr_list : expr_list2 ','",
"expr_list2 : expr0",
"expr_list2 : expr_list2 ',' expr0",
"expr_list3 :",
"expr_list3 : expr0",
"expr_list3 : expr_list2 ',' expr0",
"m_expr_list :",
"m_expr_list : m_expr_list2",
"m_expr_list : m_expr_list2 ','",
"m_expr_list : expr_list2",
"m_expr_list : expr_list2 ','",
"m_expr_list2 : expr0 m_expr_values",
"m_expr_list2 : m_expr_list2 ',' expr0 m_expr_values",
"m_expr_values : ':' expr0",
"m_expr_values : m_expr_values ';' expr0",
"$$20 :",
"catch : F_CATCH $$20 '(' comma_expr ')'",
"sscanf : F_SSCANF note_start '(' expr0 ',' expr0 lvalue_list ')'",
"lvalue_list :",
"lvalue_list : lvalue_list ',' F_IDENTIFIER",
"lvalue_list : lvalue_list ',' F_LOCAL",
"lvalue_list : lvalue_list ',' expr4 '[' expr0 ']'",
"lvalue_list : lvalue_list ',' expr4 '[' '<' expr0 ']'",
"lvalue_list : lvalue_list ',' expr4 '[' expr0 ',' expr0 ']'",
"lvalue_list : lvalue_list ',' expr4 '[' expr0 F_RANGE ']'",
"lvalue_list : lvalue_list ',' expr4 '[' expr0 F_RANGE expr0 ']'",
"lvalue_list : lvalue_list ',' expr4 '[' expr0 F_RANGE '<' expr0 ']'",
"lvalue_list : lvalue_list ',' expr4 '[' '<' expr0 F_RANGE expr0 ']'",
"lvalue_list : lvalue_list ',' expr4 '[' '<' expr0 F_RANGE '<' expr0 ']'",
"lvalue : F_IDENTIFIER",
"lvalue : F_LOCAL",
"lvalue : expr4 '[' expr0 ']'",
"lvalue : expr4 '[' '<' expr0 ']'",
"lvalue : expr4 '[' expr0 ',' expr0 ']'",
"lvalue : expr4 '[' expr0 F_RANGE ']'",
"lvalue : expr4 '[' '<' expr0 F_RANGE ']'",
"lvalue : expr4 '[' expr0 F_RANGE expr0 ']'",
"lvalue : expr4 '[' expr0 F_RANGE '<' expr0 ']'",
"lvalue : expr4 '[' '<' expr0 F_RANGE expr0 ']'",
"lvalue : expr4 '[' '<' expr0 F_RANGE '<' expr0 ']'",
"string_constant : F_STRING",
"string_constant : string_constant '+' F_STRING",
"string_constant : F_STRING F_STRING",
"string_constant : string_constant '+' F_STRING F_STRING",
"string_constant : '(' string_constant ')'",
"$$21 :",
"function_call : function_name $$21 '(' expr_list3 ')'",
"$$22 :",
"function_call : expr4 F_ARROW function_name $$22 '(' expr_list3 ')'",
"anchestor : F_IDENTIFIER",
"anchestor : F_STRING F_STRING",
"anchestor : F_STRING",
"function_name : F_IDENTIFIER",
"function_name : F_COLON_COLON F_IDENTIFIER",
"function_name : anchestor F_COLON_COLON F_IDENTIFIER",
"condStart : F_IF '(' comma_expr ')'",
"cond : condStart statement optional_else",
"optional_else :",
"$$23 :",
"optional_else : F_ELSE $$23 statement",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 5443 "lang.y"
#line 5319 "prolang.y"


/* 
 * Add a trailing jump after the last initialization code. 
 */ 
void add_new_init_jump() { 
    /* 
     * Add a new jump. 
     */ 
    ins_f_byte(F_JUMP); 
    last_initializer_end = mem_block[A_PROGRAM].current_size; 
    ins_short(0); 
} 


static void arrange_protected_lvalue(start, code, end, newcode)
    int start, code, end, newcode;
{
    int current;
    char *p;

    current = CURRENT_PROGRAM_SIZE;
    if (code >= 0) {
	if (end) {
	    int length;
	    char *q;

	    length = end - start + 1;
	    if (current+length >
		mem_block[A_PROGRAM].max_size)
		realloc_a_program();
	    p = mem_block[A_PROGRAM].block;
	    memcpy(p + current, p + start, length);
	    p += start;
	    q = p + length;
	    length = current - start;
	    do *p++ = *q++; while (--length);
	    switch(code) {
	      case F_PUSH_INDEXED_LVALUE - F_OFFSET:
		code = F_PUSH_PROTECTED_INDEXED_LVALUE - F_OFFSET;
		break;
	      case F_PUSH_RINDEXED_LVALUE - F_OFFSET:
		code = F_PUSH_PROTECTED_RINDEXED_LVALUE - F_OFFSET;
		break;
	      case F_PUSH_INDEXED_MAP_LVALUE - F_OFFSET:
		code = F_PUSH_PROTECTED_INDEXED_MAP_LVALUE - F_OFFSET;
		break;
	      case F_PUSH_IDENTIFIER16_LVALUE - F_OFFSET:
		p[-3] = code;
		goto code_stored;
#ifdef DEBUG
	      default:
		fatal("Unexpected lvalue code\n");
#endif
	    }
	    p[-1] = code >> F_ESCAPE_BITS;
	    *p++ = code;
	    current++;
	code_stored:
	    *p++ = newcode >> F_ESCAPE_BITS;
	    *p = newcode;
	} else {
	    int i;
	    int length;

	    if (current + 2 > mem_block[A_PROGRAM].max_size)
		realloc_a_program();
	    p = mem_block[A_PROGRAM].block + start;
	    i = p[1];
	    length = current - start - 2;
	    for( ; --length >= 0; p++) *p = p[2];
	    *p++ = code;
	    *p++ = i;
	    *p++ = newcode >> F_ESCAPE_BITS;
	    *p = newcode;
	}
    } else {
	switch(newcode) {
	  case F_PROTECTED_INDEX_LVALUE - F_OFFSET:
	    newcode = F_PUSH_PROTECTED_INDEXED_LVALUE - F_OFFSET;
	    break;
	  case F_PROTECTED_RINDEX_LVALUE - F_OFFSET:
	    newcode = F_PUSH_PROTECTED_RINDEXED_LVALUE - F_OFFSET;
	    break;
	  default:
	    yyerror("Need lvalue for range lvalue.");
	}
	if (current + 2 > mem_block[A_PROGRAM].max_size)
	    realloc_a_program();
	p = mem_block[A_PROGRAM].block + start;
	*p++ = newcode >> F_ESCAPE_BITS;
	*p = newcode;
    }
    CURRENT_PROGRAM_SIZE = current + 2;
}

void epilog() {
    extern int current_time;

    int size, i;
    mp_int num_functions, num_strings, num_variables;
    char *p;
    struct ident *g,*q;
    struct function *f;
    mp_int num_function_names;
    struct function *funname_start1, *funname_start2, **link1, **link2;
    struct program *prog;

#ifdef DEBUG
    if (num_parse_error == 0 && type_of_arguments.current_size != 0)
	fatal("Failed to deallocate argument type stack\n");
#endif
    if (last_string_constant) {
	free_string(last_string_constant);
	last_string_constant = 0;
    }
    while (case_blocks) {
	struct case_list_entry *tmp;

	tmp = case_blocks;
	case_blocks = tmp->next;
	xfree(tmp);
    }
    if (mem_block[A_VIRTUAL_VAR].current_size / sizeof (struct variable) >
	0x100)
    {
	yyerror("Too many virtual variables");
    }
    add_to_mem_block(
	A_VIRTUAL_VAR,
	mem_block[A_VARIABLES].block,
	mem_block[A_VARIABLES].current_size
    );
    mem_block[A_VARIABLES].current_size = 0;








    /* 
     * Define the __INIT function, but only if there was any code 
     * to initialize. 
     */ 
    if (last_initializer_end > 0) { 
	struct ident *ip;

	ip = make_shared_identifier("__INIT", I_TYPE_UNKNOWN);
	switch (0) { default:
	    if (!ip) {
		yyerror("Out of memory");
		break;
	    }
	    if (ip->type > I_TYPE_GLOBAL) {
		/* sigh. can't people keep from such #defines? */
		do {
		    q = ip;
		    ip = ip->inferior;
		} while (ip && ip->type > I_TYPE_GLOBAL);
		if (!ip) {
		    ip = (struct ident *)xalloc(sizeof(struct ident));
		    if (!ip) {
			yyerror("Out of memory");
			break;
		    }
		    ip->name = q->name;
		    ip->type = I_TYPE_UNKNOWN;
		    ip->inferior = 0;
		    ip->hash = q->hash;
		    q->inferior = ip;
		}
	    }
	    define_new_function(ip, 0, 0, first_initializer_start, 0, 0); 
	}
	/* ref count for ip->name was incremented by transfer_init_control() */
	/* 
	 * Change the last jump after the last initializer into a 
	 * return(1) statement. 
	 */ 
	mem_block[A_PROGRAM].block[last_initializer_end-1] = 
	    F_CONST1 - F_OFFSET; 
	mem_block[A_PROGRAM].block[last_initializer_end-0] = 
	    F_RETURN - F_OFFSET; 
    } 
 

    if (mem_block[A_STRINGS].current_size > 0x10000 * sizeof (char *))
	yyerror("Too many strings");
    while(mem_block[A_INCLUDE_NAMES].current_size) {
	add_to_mem_block(
	  A_STRINGS,
	  mem_block[A_INCLUDE_NAMES].block +
	    (mem_block[A_INCLUDE_NAMES].current_size -= sizeof(char *)),
	  sizeof(char*)
	);
    }
    num_functions = mem_block[A_FUNCTIONS].current_size /
	sizeof (struct function);
    if (num_functions > 0x10000) {
	yyerror("Too many functions");
    }
    num_strings = mem_block[A_STRINGS].current_size /
	sizeof (char *);
    num_variables = mem_block[A_VIRTUAL_VAR].current_size /
	sizeof (struct variable);
    if (num_variables >= VIRTUAL_VAR_TAG) {
	yyerror("Too many variables");
    }
    f = (struct function *)mem_block[A_FUNCTIONS].block;
    link1 = &funname_start2;
    link2 = &funname_start1;
    num_function_names = 0;
    /* Save the time to fill in undefined functions & to sort functions
     * if there was an error or unresolved inheritance.
     */
    if (!num_parse_error && !inherit_file) {
	for (i = num_functions; --i >= 0; f++) {
	    uint32 flags;

	    if ( f->flags & NAME_CROSS_DEFINED ) {
		int32 offset;

		offset = f->offset.func - (INHERIT_MASK + 1 >> 1);
		while (f[offset].flags & NAME_CROSS_DEFINED) {
		    offset =
		      (f->offset.func = offset + f[offset].offset.func) -
		      (INHERIT_MASK + 1 >> 1);
		}
	    }
	    if ((f->flags & (NAME_UNDEFINED|NAME_INHERITED)) == NAME_UNDEFINED)
	    {
		if (CURRENT_PROGRAM_SIZE + sizeof f->name + 5 >
		    mem_block[A_PROGRAM].max_size)
		{
		    realloc_a_program();
		}
		increment_string_ref(f->name);
		f->offset.pc = CURRENT_PROGRAM_SIZE + sizeof f->name + 1;
		p = mem_block[A_PROGRAM].block + CURRENT_PROGRAM_SIZE;
		memcpy(p, (char *)&f->name, sizeof f->name);
		p += sizeof f->name;
		*p++ = f->type;
		*p++ = f->num_arg;
		*p++ = f->num_local;
		*p++ = F_ESCAPE-F_OFFSET;
		*p   = F_UNDEF-F_OFFSET-0x100;
		CURRENT_PROGRAM_SIZE += sizeof f->name + 5;
	    }
	    flags = f->flags;
	    f->flags = flags & NAME_INHERITED ?
	      (flags & ~INHERIT_MASK)  | (f->offset.inherit & INHERIT_MASK) :
	      (flags & ~FUNSTART_MASK) | (f->offset.pc & FUNSTART_MASK);
	    if ( !(flags & (NAME_HIDDEN|NAME_UNDEFINED|TYPE_MOD_PRIVATE) ) ) {
		*link1 = f;
		link1 = link2;
		link2 = &f->offset.next;
		num_function_names++;
	    }
	}
	*link1 = 0;
	*link2 = 0;

	/* Store line number info for undefined functions */
	store_line_number_info();

	if (num_function_names <= 1) {
	    funname_start1 = funname_start2;
	} else {
	  int runlength;

	  runlength = 1;
	  do {
	    struct function *out_start1, *out_start2, **out1, **out2;
	    int count1, count2;
    
	    count1 = num_function_names & runlength-1;
	    count2 = num_function_names & runlength;
	    if (!count1) {
		out2 = &out_start1;
		*out2 = funname_start2;
		while (--count2 >= 0) {
		    out2 = &(*out2)->offset.next;
		}
		funname_start2 = *out2;
		count1 = count2 = runlength;
		out1 = &out_start2;
	    } else if (!count2) {
		out2 = &out_start1;
		*out2 = funname_start1;
		do {
		    out2 = &(*out2)->offset.next;
		} while (--count1);
		funname_start1 = *out2;
		count1 = count2 = runlength;
		out1 = &out_start2;
	    } else {
		out1 = &out_start1;
		out2 = &out_start2;
	    }
	    while (funname_start1) {
		while (1) {
#ifdef ALIGN_FUNCTIONS
		    if ((funname_start1->name - funname_start2->name) < 0)
#else
		    /* must use memcmp(), because it is used later for the
		     * program.    byteorder is non-portable.
		     */
		    if (memcmp(
			    &funname_start2->name,
			    &funname_start1->name,
			    sizeof(char *)
		       ) < 0)
#endif
		    {
			*out1 = funname_start2;
			out1 = &funname_start2->offset.next;
			funname_start2 = *out1;
			if (!--count2) {
			    *out1 = funname_start1;
			    do {
				out1 = &(*out1)->offset.next;
			    } while (--count1);
			    funname_start1 = *out1;
			    break;
			}
		    } else {
			*out1 = funname_start1;
			out1 = &funname_start1->offset.next;
			funname_start1 = *out1;
			if (!--count1) {
			    *out1 = funname_start2;
			    do {
				out1 = &(*out1)->offset.next;
			    } while (--count2);
			    funname_start2 = *out1;
			    break;
			}
		    }
		}
		{
		    struct function **temp;
    
		    temp = out1;
		    out1 = out2;
		    out2 = temp;
		}
		count1 = count2 = runlength;
	    }
	    *out1 = 0;
	    *out2 = 0;
	    funname_start1 = out_start1;
	    funname_start2 = out_start2;

	    runlength <<= 1;
	  } while (runlength < num_function_names);
	}
	if (CURRENT_PROGRAM_SIZE > FUNSTART_MASK) {
	    struct function *functions;

	    yyerror("Program too large");
	    for (i = num_functions; --i >= 0; functions++) {
		if ( !(functions->flags & (NAME_UNDEFINED|NAME_INHERITED)) ==
	              NAME_UNDEFINED)
		{
		    free_string(functions->name);
		}
	    }
	}
    }




    free_all_local_names();
    for (q=all_globals; g=q; ) {
         q=g->next_all;
         free_shared_identifier(g);
    }
    while(last_yalloced) { yfree(last_yalloced); debug_message("freeing lost block\n"); }
#if 0
    fprintf(stderr,"freeing efun shadows\n");
#endif
    if (all_efun_shadows) {
        struct efun_shadow *s, *t;
    	for (t=all_efun_shadows; s=t; ) {
#if 0
    fprintf(stderr,"freeing efun shadow '%s'\n",s->shadow->name);
#endif
    	    s->shadow->u.global.function = -2;
    	    s->shadow->u.global.variable = -2;
    	    t=s->next;
    	    xfree((char *)s);
    	}
    	all_efun_shadows = 0;
    }
    all_globals = 0;
    switch (0) { default:
	if (num_parse_error > 0 || inherit_file)
	    break;
	size = align(sizeof (struct program));
	if (!pragma_save_types) {
	    mem_block[A_ARGUMENT_TYPES].current_size = 0;
	    mem_block[A_ARGUMENT_INDEX].current_size = 0;
	}
	for (i=0; i<NUMPAREAS; i++)
	    size += align(mem_block[i].current_size);
	size += align(num_function_names * sizeof *prog->function_names);
	size += align(num_functions * sizeof *prog->functions);
	if ( !(p = (char *)xalloc(size)) ) {
	    yyerror("Out of memory");
	    break;
	}
	prog = (struct program *)p;
	*prog = NULL_program;
	if ( !(prog->name = string_copy(current_file)) ) {
	    xfree((char*)prog);
	    yyerror("Out of memory");
	    break;
	}
	prog->total_size = size;
	prog->ref = 0;
	prog->heart_beat = heart_beat;
	prog->id_number =
	  ++current_id_number ? current_id_number : renumber_programs();
	prog->load_time = current_time;
	total_prog_block_size += prog->total_size;
	total_num_prog_blocks += 1;

	p += align(sizeof (struct program));
	prog->program = p;
	if (mem_block[A_PROGRAM].current_size)
	    memcpy(p, mem_block[A_PROGRAM].block,
		   mem_block[A_PROGRAM].current_size);

	p += align(mem_block[A_PROGRAM].current_size);
	prog->num_function_names = num_function_names;
	prog->function_names = (unsigned short *)p;
	{
	    unsigned short *namep;

	    namep = (unsigned short *)p;
	    if ( (f = funname_start1) || (f = funname_start2) ) {
		do {
		    *namep++ =
		      f - (struct function *)mem_block[A_FUNCTIONS].block;
		} while (f = f->offset.next);
	    }
	}
	p += align(num_function_names * sizeof *prog->function_names);
	prog->num_functions = num_functions;
	prog->functions = (uint32 *)p;
	{
	    uint32 *flagp;

	    f = (struct function *)mem_block[A_FUNCTIONS].block;
	    flagp = (uint32 *)p;
	    for (i = num_functions; --i >= 0; f++)
		*flagp++ = f->flags;
	}
	p += align(num_functions * sizeof *prog->functions);
	prog->strings = (char **)p;
	prog->num_strings = num_strings;
	if (mem_block[A_STRINGS].current_size)
	    memcpy(p, mem_block[A_STRINGS].block,
		   mem_block[A_STRINGS].current_size);

	p += align(mem_block[A_STRINGS].current_size);
	prog->variable_names = (struct variable *)p;
	prog->num_variables = num_variables;
	if (mem_block[A_VIRTUAL_VAR].current_size)
	    memcpy(p, mem_block[A_VIRTUAL_VAR].block,
		   mem_block[A_VIRTUAL_VAR].current_size);

	p += align(mem_block[A_VIRTUAL_VAR].current_size);
	prog->num_inherited = mem_block[A_INHERITS].current_size /
	    sizeof (struct inherit);
	if (prog->num_inherited) {
	    memcpy(p, mem_block[A_INHERITS].block,
		   mem_block[A_INHERITS].current_size);
	    prog->inherit = (struct inherit *)p;
	} else {
	    prog->inherit = 0;
	}
	p += align(mem_block[A_INHERITS].current_size);
	if (pragma_save_types) {
	    if (mem_block[A_ARGUMENT_TYPES].current_size)
		memcpy(p, mem_block[A_ARGUMENT_TYPES].block,
		       mem_block[A_ARGUMENT_TYPES].current_size);
	    prog->argument_types = (unsigned short *)p;
	    p += align(mem_block[A_ARGUMENT_TYPES].current_size);
	    if (mem_block[A_ARGUMENT_INDEX].current_size)
		memcpy(p, mem_block[A_ARGUMENT_INDEX].block,
		       mem_block[A_ARGUMENT_INDEX].current_size);
	    prog->type_start = (unsigned short *)p;
	    p += align(mem_block[A_ARGUMENT_INDEX].current_size);
	} else {
	    prog->argument_types = 0;
	    prog->type_start = 0;
	}
	prog->line_numbers = p;
	if (mem_block[A_LINENUMBERS].current_size)
	    memcpy(p, mem_block[A_LINENUMBERS].block,
		   mem_block[A_LINENUMBERS].current_size);
	p += align(mem_block[A_LINENUMBERS].current_size);
	fix_variable_index_offsets(prog);
	prog->swap_num = -1;
	for (i=0; i<NUMAREAS; i++) {




	    xfree((char *)mem_block[i].block);
	}

	/*  marion
	    Do referencing here - avoid multiple referencing when an object
	    inherits more than one object and one of the inherited is already
	    loaded and not the last inherited
	*/
	reference_prog (prog, "epilog");
	for (i = 0; i < prog->num_inherited; i++) {
	    reference_prog (prog->inherit[i].prog, "inheritance");
	}
	compiled_prog = prog;
	return;
    }
    {
	extern void do_free_sub_strings
			PROT((int, char **, int, struct variable *));

	struct function *functions;






	/* Free all function names. */
	functions = (struct function *)mem_block[A_FUNCTIONS].block;
	for (i = num_functions; --i >= 0; functions++)
	    if ( !(functions->flags & (NAME_INHERITED|NAME_UNDEFINED)) &&
		 functions->name )
	    {
		free_string(functions->name);
	    }
	do_free_sub_strings(
	  num_strings,
	  (char **)mem_block[A_STRINGS].block,
	  num_variables,
	  (struct variable *)mem_block[A_VIRTUAL_VAR].block );
	compiled_prog = 0;
	for (i=0; i<NUMAREAS; i++)
	    xfree(mem_block[i].block);
	return;
    }
}

INLINE int proxy_efun(function, num_arg)
int function, num_arg;
{
    if (function == F_EXTRACT-F_OFFSET) {
	if (num_arg == 2) {
	    return F_EXTRACT2-F_OFFSET;
	}
	if (num_arg == 1) {
	    return F_EXTRACT1-F_OFFSET;
	}
    }
    if (function == F_PREVIOUS_OBJECT-F_OFFSET) {
	/* num_arg == 0 */
	return F_PREVIOUS_OBJECT0-F_OFFSET;
    }
    return -1;
}

static void _ins_f_code(b)
unsigned int b;
{
    if (b > 0x100)
	ins_byte(b >> F_ESCAPE_BITS);
    ins_byte(b);
}

/*
 * Patch a function definition of an inherited function, to what it really
 * should be.
 * The name of the function can be one of:
 *    object::name
 *    ::name
 *    name
 * Where 'object' is the name of the superclass.
 */
/* this function could be inline, but it is not called very often, and
   making it inline would damage the variable-register mapping in the
   calling block of yyparse.
 */
static int
insert_inherited(super_name, real_name,
		super_p, fun_p, num_arg, __prepare_insert__p)
    char *super_name;
    char *real_name;
    struct program **super_p;
    struct function *fun_p;
    int num_arg;
    char *__prepare_insert__p;
{
    extern char *findstring PROT((char *));
    struct inherit *ip;
    int num_inherits, super_length;

    while(*super_name == '/')
	super_name++;
    super_length = strlen(super_name);
    num_inherits = mem_block[A_INHERITS].current_size /
	sizeof (struct inherit);
    real_name = findstring(real_name);
    ip = (struct inherit *)mem_block[A_INHERITS].block;
    for (; num_inherits > 0; ip++, num_inherits--) {
	short i;
	uint32 flags;
	char *__PREPARE_INSERT__p = __prepare_insert__p;
	short __ADD_SHORT__s[2];

	if (*super_name) {
	    /* ip->prog->name includes .c */
	    int l = strlen(ip->prog->name + 2);

	    if (l < super_length)
		continue;
	    if (l > super_length && ip->prog->name[l-super_length-1] != '/')
		continue;
	    if (strncmp(super_name, ip->prog->name + l - super_length,
			super_length) != 0)
		continue;
	}
	if ( (i = find_function(real_name, ip->prog)) < 0)
	    continue;
	flags = ip->prog->functions[i];
	if (flags & NAME_INHERITED) {
	    struct inherit *ip2;
	    struct program *prog1, *prog2;
	    int numvar2;

	    prog1 = ip->prog;
	    ip2 = &prog1->inherit[flags & INHERIT_MASK];
	    prog2 = ip2->prog;
	    if ( (numvar2 = prog2->num_variables) &&
	      prog1->variable_names[ip2->variable_index_offset+numvar2-1].flags&
		TYPE_MOD_VIRTUAL &&
	      !(prog2->variable_names[numvar2-1].flags & TYPE_MOD_VIRTUAL) )
	    {
		/* inherited from a virtually inherited program */
		do --ip; while (ip->prog != prog2);
		i -= ip2->function_index_offset;
	    }
	}
	add_f_byte(F_CALL_EXPLICIT_INHERITED);
	add_short(ip - (struct inherit *)mem_block[A_INHERITS].block);
	add_short(i);
	add_byte(num_arg);
	*super_p = ip->prog;
	fun_p->name = real_name;
	fun_p->flags = flags & ~INHERIT_MASK;
	{
	    int i2 = i;
	    char *funstart;

	    while ( (flags = ip->prog->functions[i2]) & NAME_INHERITED) {
		ip = &ip->prog->inherit[flags & INHERIT_MASK];
		i2 -= ip->function_index_offset;
	    }
	    funstart = &ip->prog->program[flags & FUNSTART_MASK];
	    fun_p->type = funstart[-1];
	    fun_p->num_arg = (funstart[0] & 0x7f);
	    if (funstart[0] & ~0x7f)
	      fun_p->type |= TYPE_MOD_XVARARGS;
	}
	CURRENT_PROGRAM_SIZE += 6;
	return i;
    }
    if (strpbrk(super_name, "*?") && !num_arg) {
	int calls = 0;
	short i = -1;
	*super_p = 0;
	num_inherits = mem_block[A_INHERITS].current_size /
	    sizeof (struct inherit);
	ip = (struct inherit *)mem_block[A_INHERITS].block;
	for (; num_inherits > 0; ip++, num_inherits--) {
	    uint32 flags;
	    PREPARE_S_INSERT(6)

	    /* ip->prog->name includes .c */
	    int l = strlen(ip->prog->name + 2);

	    if ( !match_string(super_name, ip->prog->name, l) )
		continue;
	    if ( (i = find_function(real_name, ip->prog)) < 0)
		continue;
	    flags = ip->prog->functions[i];
	    if (flags & NAME_INHERITED) {
		struct inherit *ip2;
		struct program *prog1, *prog2;
		int numvar2;

		prog1 = ip->prog;
		ip2 = &prog1->inherit[flags & INHERIT_MASK];
		prog2 = ip2->prog;
		if ( (numvar2 = prog2->num_variables) &&
		  prog1->variable_names[ip2->variable_index_offset+numvar2-1].flags&
		    TYPE_MOD_VIRTUAL &&
		  !(prog2->variable_names[numvar2-1].flags & TYPE_MOD_VIRTUAL) )
		{
		    /* Inherited from a virtually inherited program */
		    /* The call for the virtually program itself should
		     * be sufficent. */
		    continue;
		}
	    }
	    add_f_byte(F_CALL_EXPLICIT_INHERITED);
	    add_short(ip - (struct inherit *)mem_block[A_INHERITS].block);
	    add_short(i);
	    add_byte(num_arg);
	    *super_p = ip->prog;
	    fun_p->name = real_name;
	    fun_p->flags = flags & ~INHERIT_MASK;
	    {
		struct inherit *ip2 = ip;
		int i2 = i;
		char *funstart;

		while ( (flags = ip2->prog->functions[i2]) & NAME_INHERITED) {
		    ip2 = &ip->prog->inherit[flags & INHERIT_MASK];
		    i2 -= ip->function_index_offset;
		}
		funstart = &ip->prog->program[flags & FUNSTART_MASK];
		fun_p->type = funstart[-1];
		fun_p->num_arg = funstart[0];
	    }
	    calls++;
	    CURRENT_PROGRAM_SIZE += 6;
	}
	{
	    PREPARE_S_INSERT(3)
	    add_f_byte(F_AGGREGATE);
	    add_short(calls);
	    CURRENT_PROGRAM_SIZE += 3;
	}
	return i;
    }
    return -1;
}

void yyerror(str)
char *str;
{
    extern int num_parse_error;

    char *context;

    if (num_parse_error > 5)
	return;
    context = lex_error_context();
    (void)fprintf(stderr, "%s: %s line %d %s\n", current_file, str,
		  current_line, context);
    fflush(stderr);
    smart_log(current_file, current_line, str, context);
    if (num_parse_error == 0)
	save_error(str, current_file, current_line);
    num_parse_error++;
}

#if 0
static int check_declared(varname)
    struct ident *varname;
{
    struct variable *vp;
    int offset;

    for (offset=0;
	 offset < mem_block[A_VARIABLES].current_size;
	 offset += sizeof (struct variable)) {
	vp = (struct variable *)&mem_block[A_VARIABLES].block[offset];
	if (vp->flags & NAME_HIDDEN)
	    continue;
	if (strcmp(vp->name, str) == 0)
	    return offset / sizeof (struct variable);
    }
    return -1;
}
#endif

static int verify_declared(p)
    struct ident *p;
{
    int r;

    if (p->type != I_TYPE_GLOBAL || (r = p->u.global.variable) < 0) {
        yyerrorf("Variable %s not declared !", p->name);
	return -1;
    }
    return r;
}

void free_all_local_names()
{
    struct ident *p,*q;
    for (q=all_locals; p=q;) {
        q = p->next_all;
        free_shared_identifier(p);
    }
    all_locals = 0;
    current_number_of_locals = 0;
    current_break_stack_need = 0;
    max_break_stack_need = 0;
}

void add_local_name(ident, type)
    struct ident *ident;
    int type;
{
    if (current_number_of_locals == MAX_LOCAL)
	yyerror("Too many local variables");
    else {
	if (ident->type != I_TYPE_UNKNOWN) {
	    ident = make_shared_identifier(ident->name, I_TYPE_LOCAL);
	}
	ident->type = I_TYPE_LOCAL;
	ident->u.local = current_number_of_locals;
	ident->next_all = all_locals;
	all_locals = ident;
	type_of_locals[current_number_of_locals] = type;
	full_type_of_locals[current_number_of_locals++] = type;
    }
}

static void cross_define(from, to, offset)
struct function *from, *to;
int32 offset;
{
    short nomask;

    to->flags = (to->flags & ~NAME_UNDEFINED) |
               (from->flags & (NAME_UNDEFINED|NAME_PROTOTYPE)) |
               NAME_CROSS_DEFINED | NAME_HIDDEN | NAME_INHERITED;
    to->offset.func = offset + (INHERIT_MASK + 1 >> 1);
    nomask = (from->flags|to->flags) & TYPE_MOD_NO_MASK;
    from->flags |= nomask;
    to  ->flags |= nomask;
}

static uint32 *get_function_id(progp, fx)
    struct program *progp;
    int fx;
{
    uint32 flags;

    flags = progp->functions[fx];
    if (flags & NAME_CROSS_DEFINED) {
	fx += (flags & INHERIT_MASK) - (INHERIT_MASK + 1 >> 1);
	flags = progp->functions[fx];
    }
    while(flags & NAME_INHERITED) {
        struct inherit *inheritp;

        inheritp = &progp->inherit[flags & INHERIT_MASK];
        progp = inheritp->prog;
        fx -= inheritp->function_index_offset;
        flags = progp->functions[fx];
    }
    return &progp->functions[fx];
}

#define NEW_INHERITED_INDEX (0xfffff)

/*
 * Copy all function definitions from an inherited object. They are added
 * as undefined, so that they can be redefined by a local definition.
 * If they are not redefined, then they will be updated, so that they
 * point to the inherited definition. See epilog(). Types will be copied
 * at that moment (if available).
 *
 * An explicit call to an inherited function will not be
 * done through this entry (because this entry can be replaced by a new
 * definition). If an function defined by inheritance is called,
 * this is done with F_CALL_EXPLICIT_INHERITED
 */

static int copy_functions(from, type)
    struct program *from;
    int type;
{
    int i, initializer = -1;
    uint32 first_func_index, current_func_index;








    struct function *fun_p;
    unsigned short *ixp;

    if (mem_block[A_FUNCTIONS].max_size <
        mem_block[A_FUNCTIONS].current_size +
          from->num_functions * sizeof(struct function) )
    {
	if (!realloc_mem_block(&mem_block[A_FUNCTIONS],
			  mem_block[A_FUNCTIONS].current_size +
			    from->num_functions * sizeof(struct function)))
	    return

		    0

			;
    }
    fun_p = (struct function *)
	(mem_block[A_FUNCTIONS].block + mem_block[A_FUNCTIONS].current_size);
    for (i=0; i < from->num_functions; i++,fun_p++) {
	struct program *defprog;
	struct inherit *ip;
	unsigned char *funstart;
	int i2;
	uint32 flags;

	flags = from->functions[i];
	fun_p->offset.inherit = NEW_INHERITED_INDEX;
	i2 = i;
	if (flags & NAME_INHERITED) {
	    fun_p->flags =
		(flags & ~INHERIT_MASK) | NAME_INHERITED | NAME_HIDDEN;
	    if (flags & NAME_CROSS_DEFINED) {
		i2 +=
		  (fun_p->offset.func = (flags & INHERIT_MASK)) -
		  (INHERIT_MASK + 1 >> 1);
	    }
	} else {
	    fun_p->flags =
		(flags & ~FUNSTART_MASK) | NAME_INHERITED | NAME_HIDDEN;
	}
	defprog = from;
	while ( (flags = defprog->functions[i2]) & NAME_INHERITED) {
	    ip = &defprog->inherit[flags & INHERIT_MASK];
	    i2 -= ip->function_index_offset;
	    defprog = ip->prog;
	}
	funstart = &defprog->program[flags & FUNSTART_MASK];
	memcpy(
	    (char *)&fun_p->name,
	    funstart - 1 - sizeof fun_p->name,
	    sizeof fun_p->name
	);
	fun_p->type = funstart[-1];
	fun_p->num_arg = (funstart[0] & 0x7f);
	if (funstart[0] & ~0x7f)
	  fun_p->type |= TYPE_MOD_XVARARGS;
	if (funstart[2] == F_ESCAPE - F_OFFSET &&
	    funstart[3] == F_UNDEF  - F_OFFSET -0x100)
	{
	    fun_p->flags |= NAME_UNDEFINED;
	}
    }
    fun_p = (struct function *)
	(mem_block[A_FUNCTIONS].block + mem_block[A_FUNCTIONS].current_size);
    ixp = from->function_names;
    for (i = from->num_function_names; --i >= 0; ) {
	fun_p[*ixp++].flags &= ~NAME_HIDDEN;
    }
    first_func_index = current_func_index =
      mem_block[A_FUNCTIONS].current_size / sizeof (struct function);
    mem_block[A_FUNCTIONS].current_size += sizeof *fun_p * from->num_functions;
    for (i=0; i < from->num_functions; i++,current_func_index++) {
	/* Do not call define_new_function() from here, as duplicates would
	 * be removed.
	 */
	struct function fun;
	int new_type;
	unsigned short tmp_short;

	struct ident* p;

	fun = fun_p[i];	/* Make a copy */
	/* Prepare some data to be used if this function will not be
	 * redefined.
	 */
	/* fun.name has already it's ref as a newly defined function in from */
	fun.flags |= type & TYPE_MOD_NO_MASK;
	switch (0) { default:
	    if ( (fun.flags & (NAME_HIDDEN|TYPE_MOD_NO_MASK|NAME_UNDEFINED) ) ==
		 (NAME_HIDDEN|TYPE_MOD_NO_MASK) )
		break;
	    /* this function is either visible or subject to redefinition */
	    p = make_shared_identifier(fun.name, I_TYPE_GLOBAL);
	    if (!p) {
		yyerror("Out of memory");
		break;
	    }
	    if (p->type > I_TYPE_GLOBAL) {
		/* sigh. can't people keep from such #defines? */
		struct ident *q;

		do {
		    q = p;
		    p = p->inferior;
		}
		while (p && p->type > I_TYPE_GLOBAL);
		if (!p) {
		    p = (struct ident *)xalloc(sizeof(struct ident));
		    if (!p) {
			yyerror("Out of memory");
			break;
		    }
		    p->name = q->name;
		    p->type = I_TYPE_UNKNOWN;
		    p->inferior = 0;
		    p->hash = q->hash;
		    q->inferior = p;
		}
	    }
	    if (p->type != I_TYPE_UNKNOWN) {
		int32 n;

		if ( (n = p->u.global.function) >= 0) {
		  /* already inherited from somewhere else */
		  /* Don't try to resolve cross-references inside the
		   * currently inherited program; not only is this superflous,
		   * but it can also lead to circular cross-inheritance
		   * when there was in misplaced prototype or an explicit
		   * directive to inherit a multiply inherited function
		   * from a particular base class (the latter is not
		   * implemented). In these cases, the information that lead
		   * to the non-standard preference would be very hard to
		   * reconstruct.
		   */
		  if (n < first_func_index) {
		    struct function *OldFunction = FUNCTION(n);

		    if ( !(OldFunction->flags & NAME_INHERITED) ) {
			debug_message(
			  "Misplaced prototype for %s in %s\n",
			  fun.name, current_file
			);
			cross_define( &fun, OldFunction,
			  current_func_index - n );
			p->u.global.function = current_func_index;
		    } else if (fun.flags & TYPE_MOD_VIRTUAL &&
			OldFunction->flags & TYPE_MOD_VIRTUAL &&
			!((fun.flags | OldFunction->flags) & NAME_HIDDEN) &&
			get_function_id(from, i) ==
			get_function_id(
			  ((struct inherit *)mem_block[A_INHERITS].block)
			    [OldFunction->offset.inherit].prog,
			  n - ((struct inherit *)mem_block[A_INHERITS].block)
			   [OldFunction->offset.inherit].function_index_offset)
		    ) {
			/* Entries denote the same function. We have to use
			 * cross_define nonetheless, to get consistant
			 * redefinition.
			 */
			/* prefer the first one */
			OldFunction->flags |= fun.flags &
			    (TYPE_MOD_PUBLIC|TYPE_MOD_NO_MASK);
			OldFunction->flags &= fun.flags | ~TYPE_MOD_STATIC;
			cross_define( OldFunction, &fun,
			  n - current_func_index );
		    } else if
		      ( (fun.flags & OldFunction->flags & TYPE_MOD_NO_MASK) &&
		      /* both are nomask... */
		      !( (fun.flags|OldFunction->flags) & NAME_UNDEFINED ) )
		    {
	                yyerrorf(
	                  "Illegal to inherit 'nomask' function '%s' twice",
	                  fun.name);
		    }
		    else if ((fun.flags & TYPE_MOD_NO_MASK ||
		        OldFunction->flags & (NAME_HIDDEN|NAME_UNDEFINED)) &&
		      !(         fun.flags & (NAME_HIDDEN|NAME_UNDEFINED)) ) {
			/* silently prefer this one */
			cross_define( &fun, OldFunction,
			  current_func_index - n );
			p->u.global.function = current_func_index;
		    } else {
			/* prefer the first one */
			cross_define( OldFunction, &fun,
			  n - current_func_index );
		    }
		  } else if ( !(fun.flags & NAME_CROSS_DEFINED) ) {
		    /* This is the dominant definition in the superclass,
		     * inherit this one.
		     */
#ifdef DEBUG
		    /* The definition we picked before should be cross-defined
		     * to the definition we have now.
		     */
		    if ( !(FUNCTION(n)->flags & NAME_CROSS_DEFINED) ||
			 FUNCTION(n)->offset.func !=
			   current_func_index - n + (INHERIT_MASK + 1 >> 1) )
		    {
			fatal(
			  "inconsistent function definition in superclass\n"
			);
		    }
#endif
		    p->u.global.function = current_func_index;
		  }
		} else {
		    if (n == -2) {
		        struct efun_shadow *q;

        	        q =(struct efun_shadow *)
        	           xalloc(sizeof(struct efun_shadow));
			if (!q) {
			    yyerror("Out of memory");
			    break;
			}
        	        q->shadow = p;
        	        q->next = all_efun_shadows;
        	        all_efun_shadows = q;
        	    }
		    p->u.global.function = current_func_index;
		}
	    } else {
		/* was I_TYPE_UNKNOWN */
		p->type = I_TYPE_GLOBAL;
		p->u.global.variable = -1;
		p->u.global.efun     = -1;
		p->u.global.sim_efun = -1;
		p->u.global.function = current_func_index;
		p->next_all = all_globals;
		all_globals = p;
	    }
	    /*
	     * public functions should not become private when inherited
	     * 'private'
	     */
	    new_type = type;
	    if (fun.flags & TYPE_MOD_PUBLIC)
		new_type &= ~(TYPE_MOD_PRIVATE|TYPE_MOD_STATIC);
	    fun.flags |= new_type;
	    /* marion
	     * this should make possible to inherit a heart beat function, and
	     * thus to mask it if wanted.
	     */
	    if ((heart_beat == -1) && fun.name[0] == 'h' &&
	      (strcmp(fun.name, "heart_beat") == 0))
		heart_beat = current_func_index;

	    else if (fun.name[0] == '_' && strcmp(fun.name+1, "_INIT") == 0) {
	        initializer = i;
	        fun.flags |= NAME_UNDEFINED;
	    }

	} /* end of visibility/redifinability - only code */
	/*
	 * Copy information about the types of the arguments, if it is
	 * available.
	 */
	tmp_short = INDEX_START_NONE;	/* Presume not available. */
	if (from->type_start != 0) {
	  if (from->type_start[i] != INDEX_START_NONE)
	  {
	    /*
	     * They are available for function number 'i'. Copy types of
	     * all arguments, and remember where they started.
	     */
	    tmp_short = mem_block[A_ARGUMENT_TYPES].current_size /
		sizeof from->argument_types[0];
	    if (fun.num_arg)
		add_to_mem_block(
		  A_ARGUMENT_TYPES,
		  (char *)&from->argument_types[from->type_start[i]],
		  (sizeof (unsigned short)) * fun.num_arg
		);
	  }
	} else
	    fun.flags |= NAME_TYPES_LOST;
	/*
	 * Save the index where they started. Every function will have an
	 * index where the type info of arguments starts.
	 */
	add_to_mem_block(A_ARGUMENT_INDEX, (char*)&tmp_short, sizeof tmp_short);
	fun_p[i] = fun;
    }

    return initializer;

}

void fix_function_inherit_indices(from)
    struct program *from;
{
    int i, inherit_index;
    struct function *funp;

    inherit_index =
      mem_block[A_INHERITS].current_size / sizeof (struct inherit);
    funp =
      (struct function *)
	(mem_block[A_FUNCTIONS].block+mem_block[A_FUNCTIONS].current_size) -
      from->num_functions;
    for(i = from->num_functions; --i>=0; funp++) {
	if ( funp->offset.inherit == NEW_INHERITED_INDEX &&
	     !(funp->flags & NAME_CROSS_DEFINED) )
	{
	    funp->offset.inherit = inherit_index;
	}
    }
}

void fix_variable_index_offsets(new_prog)
    struct program *new_prog;
{
    int i;
    struct inherit *inheritp;

    i = new_prog->num_inherited;
    for (inheritp = new_prog->inherit; --i >= 0; inheritp++) {
	if (inheritp->variable_index_offset & NON_VIRTUAL_OFFSET_TAG) {
	    inheritp->variable_index_offset += num_virtual_variables;
	    inheritp->variable_index_offset &= ~NON_VIRTUAL_OFFSET_TAG;
	}
    }
}

/*
 * Copy all variable names from the object that is inherited from.
 * It is very important that they are stored in the same order with the
 * same index.
 */






static void copy_variables(from, type)
    struct program *from;
    int type;

{
    int i, j;
    int new_bound, last_bound;
    int variable_index_offset, function_index_offset;
    int inheritc;
    struct inherit *inheritp;
    int from_variable_index_offset = -1;
    int previous_variable_index_offset;

    type &= ~TYPE_MOD_VARARGS;
    if (type & TYPE_MOD_VIRTUAL) {
	inheritp = (struct inherit *)(mem_block[A_INHERITS].block);
	j = mem_block[A_INHERITS].current_size;
	for (; (j -= sizeof(struct inherit)) >= 0; inheritp++) {
	    if (inheritp->prog == from &&
		!(inheritp->variable_index_offset & NON_VIRTUAL_OFFSET_TAG) )
	    {
		from_variable_index_offset =
		  inheritp->variable_index_offset + VIRTUAL_VAR_TAG;
		break;
	    }
	}
#ifdef INITIALIZATION_BY___INIT
	if (variables_initialized && from_variable_index_offset < 0)
	    yyerror(
	      "illegal to inherit virtually after initializing variables\n"
	    );
#endif
    }
    function_index_offset =
      mem_block[A_FUNCTIONS].current_size / sizeof(struct function) -
      from->num_functions;
    variable_index_offset =
	mem_block[A_VIRTUAL_VAR].current_size / sizeof(struct variable);
    i = from->num_inherited;
    last_bound = 0;
    for (inheritc = 0, inheritp = from->inherit; ; inheritc++, inheritp++) {
	if (--i >= 0) {
	    struct program *progp;

	    progp = inheritp->prog;
	    new_bound =
	      inheritp->variable_index_offset + progp->num_variables;

	    /* has a new virtual variable been introduced in this program? */
	    if (progp->num_variables &&
		from->variable_names[new_bound-1].flags & TYPE_MOD_VIRTUAL &&
		!(progp->variable_names[progp->num_variables-1].flags &
		  TYPE_MOD_VIRTUAL) )
	    {
		struct inherit inherit, *inheritp2;
		int k, inherit_index;
		uint32 *flagp;
		struct function *funp;
    
#ifdef INITIALIZATION_BY___INIT
		if (variables_initialized)
		    yyerror(
"illegal to inherit virtually after initializing variables\n"
		    );
#endif
		inherit = *inheritp;
		inheritp2 = (struct inherit *)(mem_block[A_INHERITS].block);
		j = mem_block[A_INHERITS].current_size;
		for (; (j -= sizeof(struct inherit)) >= 0; inheritp2++) {
		    if (inheritp2->prog == inherit.prog &&
			!(inheritp2->variable_index_offset &
			  NON_VIRTUAL_OFFSET_TAG) )
		    {
			inherit.variable_index_offset =
			  inheritp2->variable_index_offset;
			break;
		    }
		}
		if (j < 0) {
		    variable_index_offset += new_bound - last_bound;
		    inherit.variable_index_offset =
		      variable_index_offset - progp->num_variables;
		}
		inherit_index = (mem_block[A_INHERITS].current_size - j) /
		   sizeof(struct inherit) - 1;
		inherit.function_index_offset += function_index_offset;
		add_to_mem_block(A_INHERITS, (char *)&inherit, sizeof inherit);
		/* If a function is directly inherited from a program that
		 * introduces a virtual variable, the code therein is not
		 * aware of virtual inheritance. For this reason, there are
		 * the extra struct inherits with an appropriate
		 * variable_index_offset; we have to redirect inheritance
		 * to these struct inherits.
		 */
		flagp = from->functions + inheritp->function_index_offset;
		funp = (struct function *)mem_block[A_FUNCTIONS].block +
		    inherit.function_index_offset;
		for (k = inherit.prog->num_functions; --k >= 0; funp++) {
		    if ( !(funp->flags & NAME_CROSS_DEFINED) &&
			   (*flagp & (NAME_INHERITED|NAME_CROSS_DEFINED)) ==
			   NAME_INHERITED &&
			 (*flagp & INHERIT_MASK) == inheritc )
		    {
			funp->offset.inherit = inherit_index;
		    }
		    flagp++;
		}
		if (j >= 0) {
		    /* There has been another instance
		     * of this virtual superclass before
		     */
		    if (new_bound > last_bound)
			last_bound = new_bound;
		    continue;
		}
		previous_variable_index_offset = -1;
	    } else {
		continue;	/* Not a virtual superclass */
	    }
	} else {
	    previous_variable_index_offset = from_variable_index_offset;
	    new_bound = from->num_variables;
	    if (new_bound == last_bound)
		break;
	}
	for (j = last_bound; j < new_bound; j++) {
	    extern struct svalue const0;
	    struct ident *p;
	    int new_type;
    
	    p = make_shared_identifier(from->variable_names[j].name,
		I_TYPE_GLOBAL);
	    if (!p) {
		yyerror("Out of memory");
		return;
	    }
	    if (p->type > I_TYPE_GLOBAL) {
		/* sigh. can't people keep from such #defines? */
		struct ident *q;
    
		do {
			q = p;
			p = p->inferior;
		} while (p && p->type > I_TYPE_GLOBAL);
		if (!p) {
		    p = (struct ident *)xalloc(sizeof(struct ident));
		    if (!p) {
			yyerror("Out of memory");
			return;
		    }
		    p->name = q->name;
		    p->type = I_TYPE_UNKNOWN;
		    p->inferior = 0;
		    p->hash = q->hash;
		    q->inferior = p;
		}
	    }
	    new_type = type;
	    /*
	     * 'public' variables should not become private when inherited
	     * 'private'.
	     */
	    if (from->variable_names[j].flags & TYPE_MOD_PUBLIC)
		new_type &= ~TYPE_MOD_PRIVATE;
	    /* define_variable checks for previous 'nomask' definition. */
	    if (previous_variable_index_offset >= 0) {
		if ( !(from->variable_names[j].flags & TYPE_MOD_PRIVATE) )
		    redeclare_variable(p,
		      new_type | from->variable_names[j].flags | NAME_INHERITED,
		      previous_variable_index_offset + j
		    );
	    } else {
		define_variable(p,
		  new_type | from->variable_names[j].flags |
		  (from->variable_names[j].flags & TYPE_MOD_PRIVATE ?
		    (NAME_HIDDEN|NAME_INHERITED)  :  NAME_INHERITED )




		);
	    }
	} /* end loop through functions */
	last_bound = new_bound;
    } /* end of loop through inherits */
}

/*
 * This function is called from lex.c for every new line read from the
 * "top" file (means not included files). Some new lines are missed,
 * as with #include statements, so it is compensated for.
 */
void store_line_number_info()
{
    char c;
    short offset = mem_block[A_PROGRAM].current_size - stored_bytes;
    if (offset <= 0) return;
    stored_bytes = mem_block[A_PROGRAM].current_size;

    if (offset <= 8 &&
      current_line - stored_lines >= 2 && current_line - stored_lines <= 9) {
	c = offset + 8*(current_line - stored_lines) + 47;
	byte_to_mem_block(A_LINENUMBERS, c);
	stored_lines = current_line;
	return;
    }
    stored_lines++;
    while(stored_lines < current_line)
    {
	int lines;

	lines = current_line - stored_lines;
	if (lines > LI_MAXEMPTY) lines = LI_MAXEMPTY;
	stored_lines += lines;
	c = 256 - lines;
	byte_to_mem_block(A_LINENUMBERS, c);
    }
    while(offset >= LI_MAXOFFSET) {
	byte_to_mem_block(A_LINENUMBERS, LI_MAXOFFSET);
	offset -= LI_MAXOFFSET;
    }
    byte_to_mem_block(A_LINENUMBERS, offset);
}

void store_line_number_relocation(relocated_from)
    int relocated_from;
{
    int save_current, offset;

    save_current = current_line;
    stored_lines -= 2;
    current_line = stored_lines+1;
    offset = current_line - relocated_from;
    if (offset >= LI_SMALL_REL) {
	byte_to_mem_block(A_LINENUMBERS, LI_L_RELOCATED);
	byte_to_mem_block(A_LINENUMBERS, offset >> 8);
	byte_to_mem_block(A_LINENUMBERS, offset);
	/* trailing LI_L_RELOCATED allows bidirectional traversal */
	byte_to_mem_block(A_LINENUMBERS, LI_L_RELOCATED);
    } else {
	byte_to_mem_block(A_LINENUMBERS, LI_RELOCATED + offset);
    }
    store_line_number_info();
    current_line = save_current;
}

static int simple_includes;

void store_include_info(name)
    char *name;
{
    extern char *out_of_memory_string;

    if (last_include_start == mem_block[A_LINENUMBERS].current_size) {
	simple_includes++;
    } else {
	simple_includes = 0;
    }
    stored_lines++;
    while(stored_lines < current_line) {
	int lines;

	lines = current_line - stored_lines;
	if (lines > LI_MAXEMPTY) lines = LI_MAXEMPTY;
	stored_lines += lines;
	byte_to_mem_block(A_LINENUMBERS, 256 - lines);
    }
    byte_to_mem_block(A_LINENUMBERS, LI_INCLUDE);
    last_include_start = mem_block[A_LINENUMBERS].current_size;
    name = make_shared_string(name);
    if (!name) {
	increment_string_ref(name = out_of_memory_string);
	yyerror("Out of memory");
    }
    add_to_mem_block(A_INCLUDE_NAMES, (char *)&name, sizeof name);
    stored_lines = 0;
}

void store_include_end() {
    unsigned char c;

    stored_lines = current_line-1;
    if (last_include_start == mem_block[A_LINENUMBERS].current_size) {
	last_include_start = mem_block[A_LINENUMBERS].current_size - 1;
	stored_lines--;
	while(last_include_start &&
	  (c = mem_block[A_LINENUMBERS].block[last_include_start - 1]) >=
	  0x100 - LI_MAXEMPTY)
	{
	    stored_lines += c - 0x100;
	    last_include_start--;
	}
	mem_block[A_LINENUMBERS].current_size = last_include_start;
	if (--simple_includes < 0) {
	    last_include_start--;
	}
	free_string( *(char **)
	  (mem_block[A_INCLUDE_NAMES].block +
	   (mem_block[A_INCLUDE_NAMES].current_size -= sizeof(char *)))
	);
    } else {
	byte_to_mem_block(A_LINENUMBERS, LI_INCLUDE_END);
    }
}

static char *get_type_name(type)
    int type;
{
    static char buff[100];
    static char *type_name[] = { "unknown", "int", "string", "void", "object",
				 "mapping", "float", "mixed", 0, "closure",
				 "symbol", "quoted_array", };
    int pointer = 0, reference = 0;

    buff[0] = 0;
    if (type & TYPE_MOD_STATIC)
	strcat(buff, "static ");
    if (type & TYPE_MOD_NO_MASK)
	strcat(buff, "nomask ");
    if (type & TYPE_MOD_PRIVATE)
	strcat(buff, "private ");
    if (type & TYPE_MOD_PROTECTED)
	strcat(buff, "protected ");
    if (type & TYPE_MOD_PUBLIC)
	strcat(buff, "public ");
    if (type & TYPE_MOD_VARARGS)
	strcat(buff, "varargs ");
    type &= TYPE_MOD_MASK;
    if (type & TYPE_MOD_POINTER) {
	pointer = 1;
	type &= ~TYPE_MOD_POINTER;
    }
    if (type & TYPE_MOD_REFERENCE) {
	reference = 1;
	type &= ~TYPE_MOD_REFERENCE;
    }
    if (type >= sizeof type_name / sizeof type_name[0])
	fatal("Bad type\n");
    strcat(buff, type_name[type]);
    strcat(buff," ");
    if (pointer)
	strcat(buff, "* ");
    if (reference)
	strcat(buff, "& ");
    return buff;
}

static void type_error(str, type)
    char *str;
    int type;
{
    char *p;

    p = get_type_name(type);
    yyerrorf("%s: \"%s\"", str, p);
}

static void argument_type_error(i, type)
    int i, type;
{
    char *p;

    p = get_type_name(type);
    yyerrorf("Bad argument to %s: \"%s\"", instrs[i].name, p);
}

/*
 * Compile an LPC file.
 */
void compile_file() {
    int yyparse();

    prolog();
    yyparse();
    epilog();
}

static char *get_two_types(type1, type2)
    int type1, type2;
{
    static char buff[100];

    strcpy(buff, "( ");
    strcat(buff, get_type_name(type1));
    strcat(buff, "vs ");
    strcat(buff, get_type_name(type2));
    strcat(buff, ")");
    return buff;
}

static void insert_pop_value()
{
    if (last_expression == mem_block[A_PROGRAM].current_size-1) {
        switch ( mem_block[A_PROGRAM].block[last_expression]+F_OFFSET ) {
        case F_ASSIGN:
            mem_block[A_PROGRAM].block[last_expression] =
                F_VOID_ASSIGN - F_OFFSET;
	    break;
	case F_ADD_EQ:
            mem_block[A_PROGRAM].block[last_expression] =
                F_VOID_ADD_EQ - F_OFFSET;
	    break;
	case F_PRE_INC:
	case F_POST_INC:
            mem_block[A_PROGRAM].block[last_expression] =
                F_INC - F_OFFSET;
	    break;
	case F_PRE_DEC:
	case F_POST_DEC:
            mem_block[A_PROGRAM].block[last_expression] =
                F_DEC - F_OFFSET;
	    break;
	case F_CONST0:
	case F_CONST1:
	    mem_block[A_PROGRAM].current_size = last_expression;
	    break;
        default: ins_f_byte(F_POP_VALUE);
	}
	last_expression = -1;
    } else ins_f_byte(F_POP_VALUE);
}

#define SH(x) - -(1 << (x))
short hook_type_map[NUM_CLOSURE_HOOKS] =
{
 0,
 0,
      SH(T_CLOSURE),
     SH(T_CLOSURE),
                 SH(T_STRING),
                    SH(T_STRING),
                 SH(T_STRING),
                        SH(T_STRING),
       SH(T_CLOSURE) SH(T_STRING),
 SH(T_CLOSURE) SH(T_STRING) SH(T_MAPPING),
    SH(T_CLOSURE) SH(T_STRING),
                  SH(T_STRING),
   SH(T_CLOSURE)              SH(T_POINTER),
     SH(T_CLOSURE) SH(T_STRING),
         SH(T_CLOSURE) SH(T_STRING),
       SH(T_CLOSURE),
 SH(T_STRING),
};
#if defined( DEBUG ) && defined ( TRACE_CODE )

int code_window_offset = -1;

void set_code_window() {
    code_window_offset = CURRENT_PROGRAM_SIZE;
}

void show_code_window() {
    int i;
    unsigned char *p;

    if (code_window_offset < 0) return;
    p = (unsigned char *)mem_block[A_PROGRAM].block + code_window_offset;
    for (i=0; i<16; i++) {
        printf("%3d ", p[i]);
    }
    printf("\n");
    fflush(stdout);
}

#endif

#ifdef MALLOC_smalloc
void count_compiler_refs() {
    if (type_of_arguments.block) {
	note_malloced_block_ref(type_of_arguments.block);
    }
}
#endif
#line 4810 "y.tab.c"
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 5:
#line 1536 "lang.y"
{ yyerror("Extra ';'. Ignored."); }
break;
case 6:
#line 1539 "lang.y"
{
#line 1411 "prolang.y"
/* copy_variables might add extra inherits for virtual inheritance.
 * For this reason, copy_functions() can't know the actual index
 * of the new inherit, so it sets it to NEW_INHERITED_INDEX instead.
 * This is changed later to the actual value by
 * fix_function_inherit_indices() .
 */
		    struct object *ob;
		    struct inherit inherit;

		    int initializer;


		    if (CURRENT_PROGRAM_SIZE)

		      if (!(((struct function *)(mem_block[A_FUNCTIONS].block+
			 mem_block[A_FUNCTIONS].current_size))[-1].flags &
			 NAME_INHERITED))

		        yyerror(
		          "illegal to inherit after defining functions"
		        );
		    ob = find_object2(last_string_constant);
		    if (ob == 0) {
			inherit_file = last_string_constant;
			last_string_constant = 0;
			/* Return back to load_object() */
			YYACCEPT;
		    }
		    /* We want to refer to the program;
		       variables are needed too if they are initialized. */
		    if (ob->flags & O_SWAPPED && load_ob_from_swap(ob) < 0) {
			free_string(last_string_constant);
			last_string_constant = 0;
			yyerror("Out of memory");
			YYACCEPT;
		    }
		    free_string(last_string_constant);
		    last_string_constant = 0;
		    if (ob->flags & O_APPROVED)
			approved_object = 1;
		    inherit.prog = ob->prog;
		    inherit.function_index_offset =
			mem_block[A_FUNCTIONS].current_size /
			  sizeof(struct function);

		    initializer = copy_functions(ob->prog, yyvsp[-3].numbers[0]);
		    if (initializer > 0) {
			transfer_init_control();
			ins_f_byte(F_CALL_EXPLICIT_INHERITED);
			ins_short(mem_block[A_INHERITS].current_size /
			    sizeof inherit);
			ins_short(initializer);
			ins_byte(0);	/* Actual number of arguments */
			ins_f_byte(F_POP_VALUE);
			add_new_init_jump();
		    }
		    copy_variables(ob->prog, yyvsp[-3].numbers[1]);




		    fix_function_inherit_indices(ob->prog);
		    inherit.variable_index_offset = 
		      yyvsp[-3].numbers[1] & TYPE_MOD_VIRTUAL ?
			mem_block[A_VIRTUAL_VAR].current_size /
			  sizeof(struct variable) - ob->prog->num_variables
		      :
			mem_block[A_VARIABLES].current_size /
			  sizeof(struct variable) - ob->prog->num_variables |
			NON_VIRTUAL_OFFSET_TAG;
		    add_to_mem_block(
		      A_INHERITS,
		      (char *)&inherit,
		      sizeof inherit
		    );
		    num_virtual_variables =
		      mem_block[A_VIRTUAL_VAR].current_size /
			sizeof (struct variable);
		}
break;
case 7:
#line 1620 "lang.y"
{ yyval.number = 0; }
break;
case 8:
#line 1620 "lang.y"
{ yyval.number = TYPE_MOD_POINTER; }
break;
case 9:
#line 1623 "lang.y"
{
#line 1495 "prolang.y"

#ifdef ALIGN_FUNCTIONS
	    CURRENT_PROGRAM_SIZE =
	      (CURRENT_PROGRAM_SIZE + sizeof(char *) - 1) & -sizeof(char *);
#endif
	    yyval.number = CURRENT_PROGRAM_SIZE;
	    if (
	      (CURRENT_PROGRAM_SIZE += sizeof(char *) + 3) >
	      mem_block[A_PROGRAM].max_size )
	    {
		(void)realloc_a_program();
	    }
	}
break;
case 11:
#line 1641 "lang.y"
{ yyval.number = -1; }
break;
case 12:
#line 1644 "lang.y"
{
	    yyvsp[-1].number |= yyvsp[-2].number;

	    if (yyvsp[-2].number & TYPE_MOD_MASK) {
		exact_types = yyvsp[-1].number;
	    } else {
		if (pragma_strict_types)
		    yyerror("\"#pragma strict_types\" requires type of function");
		exact_types = 0;
	    }
	    if (yyvsp[0].ident->type == I_TYPE_UNKNOWN) {
		/* prevent freeing by exotic name clashes */
		struct ident *p = yyvsp[0].ident;
		p->type = I_TYPE_GLOBAL;
		p->u.global.variable = -1;
		p->u.global.efun     = -1;
		p->u.global.sim_efun = -1;
		p->u.global.function = -1;
		p->next_all = all_globals;
		all_globals = p;
	    }
	}
break;
case 13:
#line 1667 "lang.y"
{
	    if (current_number_of_locals &&
		  (full_type_of_locals[current_number_of_locals-1] &
		    TYPE_MOD_VARARGS))
	    {
#line 1543 "prolang.y"
		/* The last argument has to allow an array. */
		short *t;
#ifdef NO_XVARARGS
		yyerror ("variadic function supporty disabled");
#endif
		yyvsp[-5].number |= TYPE_MOD_XVARARGS;
		t = type_of_locals + (current_number_of_locals-1);
		if (!(*t & TYPE_MOD_POINTER) && (*t & TYPE_MOD_RMASK)!=TYPE_ANY)
		{
		    if ((*t & TYPE_MOD_RMASK) != TYPE_UNKNOWN)
		        yyerror(
		          "varargs parameter must be declared array or mixed");
		    *t &= ~TYPE_MOD_RMASK;
		    *t |= TYPE_ANY;
		}
	    }
	    /*
	     * Define a prototype. If it is a real function, then the
	     * prototype will be replaced below.
	     */
	    define_new_function(yyvsp[-4].ident, yyvsp[-1].number, 0, 0,
				NAME_UNDEFINED|NAME_PROTOTYPE, yyvsp[-5].number);
	}
break;
case 14:
#line 1697 "lang.y"
{
	    int start;
	    char *p;
#line 1571 "prolang.y"
	    if ( (start = yyvsp[0].number) < 0) {
		/* function_body was a ';' ==> prototype */
		uint32 *flagp;

		flagp = &FUNCTION(yyvsp[-6].ident->u.global.function)->flags;
		*flagp |= yyvsp[-8].number &
		  (*flagp & TYPE_MOD_PUBLIC ?
		    (TYPE_MOD_NO_MASK) :
		    (TYPE_MOD_NO_MASK|TYPE_MOD_PRIVATE|TYPE_MOD_STATIC|
		     TYPE_MOD_PROTECTED|TYPE_MOD_PUBLIC) );
	    } else {
		/* function_body was a block */
		p = &mem_block[A_PROGRAM].block[start];
		memcpy(p, (char *)&yyvsp[-6].ident->name, sizeof yyvsp[-6].ident->name);
		p += sizeof yyvsp[-6].ident->name;
		*p++ = yyvsp[-7].number;
		if (yyvsp[-7].number & TYPE_MOD_XVARARGS)
		  *p++ = yyvsp[-3].number | ~0x7f;
		else
		  *p++ = yyvsp[-3].number;
		*p   = current_number_of_locals - yyvsp[-3].number+ max_break_stack_need;
		define_new_function(yyvsp[-6].ident, yyvsp[-3].number, current_number_of_locals - yyvsp[-3].number+
			max_break_stack_need,
			start + sizeof yyvsp[-6].ident->name + 1, 0, yyvsp[-7].number);
		increment_string_ref(yyvsp[-6].ident->name);
		ins_f_byte(F_RETURN0);
	    }
	    free_all_local_names();
	}
break;
case 15:
#line 1730 "lang.y"
{ if (yyvsp[-2].number == 0) yyerror("Missing type"); }
break;
case 17:
#line 1734 "lang.y"
{
	    if (exact_types && yyvsp[-2].number == 0) {
		yyerror("Missing type for argument");
		add_local_name(yyvsp[0].ident, TYPE_ANY);	/* Supress more errors */
	    } else {
		add_local_name(yyvsp[0].ident, yyvsp[-2].number | yyvsp[-1].number);
	    }
	}
break;
case 18:
#line 1743 "lang.y"
{
	    yyerror("Illegal to redeclare local name");
	}
break;
case 19:
#line 1747 "lang.y"
{ yyval.number = 0; }
break;
case 21:
#line 1750 "lang.y"
{ yyval.number = 1; }
break;
case 22:
#line 1751 "lang.y"
{ yyval.number = yyvsp[-2].number + 1; }
break;
case 23:
#line 1753 "lang.y"
{ yyval.number = TYPE_MOD_NO_MASK; }
break;
case 24:
#line 1754 "lang.y"
{ yyval.number = TYPE_MOD_STATIC; }
break;
case 25:
#line 1755 "lang.y"
{ yyval.number = TYPE_MOD_PRIVATE; }
break;
case 26:
#line 1756 "lang.y"
{ yyval.number = TYPE_MOD_PUBLIC; }
break;
case 27:
#line 1757 "lang.y"
{ yyval.number = TYPE_MOD_VARARGS; }
break;
case 28:
#line 1758 "lang.y"
{ yyval.number = TYPE_MOD_PROTECTED; }
break;
case 29:
#line 1761 "lang.y"
{ yyval.number = TYPE_MOD_VIRTUAL; }
break;
case 30:
#line 1763 "lang.y"
{ yyval.number = 0; }
break;
case 31:
#line 1764 "lang.y"
{ yyval.number = yyvsp[-1].number | yyvsp[0].number; }
break;
case 33:
#line 1768 "lang.y"
{ yyval.number = yyvsp[-2].number | yyvsp[-1].number | yyvsp[0].number; }
break;
case 34:
#line 1771 "lang.y"
{
	    static struct ident *last_identifier;
	    static uint32 last_modifier;
#line 1645 "prolang.y"
	    if (yyvsp[-2].number & TYPE_MOD_MASK) {
		yyerror("syntax error");
	    }
	    if ( !(yyvsp[-2].number & ~TYPE_MOD_MASK) ) {
		/* take lookahead into account */
		if (yyvsp[0].ident == last_identifier) {
		    last_identifier = 0;
		    yyval.numbers[0] = yyval.numbers[1] = 0;
		    break;
		}
	    } else {
		last_modifier = yyvsp[-2].number & ~TYPE_MOD_MASK;
	    }
	    last_identifier = yyvsp[0].ident;
	    if (yyvsp[-1].number) {
		yyerror("syntax error");
	    }
	    if (strcmp(last_identifier->name, "functions") == 0) {
		yyval.numbers[0] = last_modifier;
		yyval.numbers[1] = 0;
	    } else if (strcmp(last_identifier->name, "variables") == 0) {
		yyval.numbers[0] = 0;
		yyval.numbers[1] = last_modifier;
	    } else {
		yyerror("Unrecognized inheritance modifier qualifier");
		yyval.numbers[0] = yyval.numbers[1] = 0;
	    }
	    if (last_identifier->type == I_TYPE_UNKNOWN)
		free_shared_identifier(last_identifier);
	}
break;
case 35:
#line 1807 "lang.y"
{
	    yyval.numbers[0] = yyval.numbers[1] = yyvsp[0].number;
	}
break;
case 36:
#line 1811 "lang.y"
{
	    yyval.numbers[0] = yyvsp[-1].numbers[0] | yyvsp[0].numbers[0];
	    yyval.numbers[1] = yyvsp[-1].numbers[1] | yyvsp[0].numbers[1];
	}
break;
case 37:
#line 1816 "lang.y"
{ yyval.number = yyvsp[-1].number | yyvsp[0].number; current_type = yyval.number; }
break;
case 38:
#line 1819 "lang.y"
{
	    yyval.type = yyvsp[-2].number | yyvsp[-1].number;
	}
break;
case 39:
#line 1824 "lang.y"
{
	    yyval.type = yyvsp[-3].number | yyvsp[-2].number;
	}
break;
case 41:
#line 1828 "lang.y"
{ yyval.number = TYPE_UNKNOWN; }
break;
case 42:
#line 1830 "lang.y"
{ yyval.number = TYPE_NUMBER; current_type = yyval.number; }
break;
case 43:
#line 1831 "lang.y"
{ yyval.number = TYPE_NUMBER; current_type = yyval.number; }
break;
case 44:
#line 1832 "lang.y"
{ yyval.number = TYPE_STRING; current_type = yyval.number; }
break;
case 45:
#line 1833 "lang.y"
{ yyval.number = TYPE_OBJECT; current_type = yyval.number; }
break;
case 46:
#line 1834 "lang.y"
{ yyval.number = TYPE_CLOSURE; current_type = yyval.number; }
break;
case 47:
#line 1835 "lang.y"
{ yyval.number = TYPE_SYMBOL; current_type = yyval.number; }
break;
case 48:
#line 1837 "lang.y"
{ yyval.number = TYPE_FLOAT; current_type = yyval.number; }
break;
case 49:
#line 1840 "lang.y"
{ yyval.number = TYPE_MAPPING; current_type = yyval.number; }
break;
case 50:
#line 1842 "lang.y"
{yyval.number = TYPE_VOID; current_type = yyval.number; }
break;
case 51:
#line 1843 "lang.y"
{ yyval.number = TYPE_ANY; current_type = yyval.number; }
break;
case 54:
#line 1849 "lang.y"
{
	    extern struct svalue const0;
#line 1722 "prolang.y"
	    if (current_type & TYPE_MOD_VARARGS) {
		yyerror("can't declare a variable as varargs");
		current_type &= ~TYPE_MOD_VARARGS;
	    }

	    define_variable(yyvsp[0].ident, current_type | yyvsp[-1].number);
	}
break;
case 55:
#line 1860 "lang.y"
{
	    define_variable(yyvsp[0].ident, current_type | yyvsp[-1].number);
	    yyval.number = verify_declared(yyvsp[0].ident);
	    transfer_init_control();
	}
break;
case 56:
#line 1866 "lang.y"
{
	    int i = yyvsp[-2].number;
	    PREPARE_S_INSERT(4)

#ifdef DEBUG
	    if (i & VIRTUAL_VAR_TAG) {
		/* When we want to allow 'late' initializers for
		 * inherited variables, it must have a distinct syntax,
		 * lest name clashs remain undetected, making LPC code
		 * hard to debug.
		 */
		fatal("Newly declared variable is virtual\n");
	    }
#endif
	    variables_initialized = 1;
	    if (i + num_virtual_variables > 0xff) {
		add_f_byte(F_PUSH_IDENTIFIER16_LVALUE);
		add_short(i + num_virtual_variables);
		CURRENT_PROGRAM_SIZE += 1;
	    } else {
		add_f_byte(F_PUSH_IDENTIFIER_LVALUE);
		add_byte(i + num_virtual_variables);
	    }
	    if (yyvsp[-1].number != F_ASSIGN-F_OFFSET) yyerror("Illegal initialization");
	    if (!compatible_types((current_type | yyvsp[-4].number) & TYPE_MOD_MASK, yyvsp[0].lrvalue.type)){
		yyerrorf("Type mismatch %s when initializing %s",
			get_two_types(current_type | yyvsp[-4].number, yyvsp[0].lrvalue.type), yyvsp[-3].ident->name);
	    }
	    add_f_byte(F_VOID_ASSIGN);
	    CURRENT_PROGRAM_SIZE += 3;
	    add_new_init_jump();
	}
break;
case 60:
#line 1934 "lang.y"
{
	    add_local_name(yyvsp[0].ident, current_type | yyvsp[-1].number);
	}
break;
case 65:
#line 1945 "lang.y"
{
	    insert_pop_value();
	    if (d_flag)
		ins_f_byte(F_BREAK_POINT);
	    /* if (exact_types && !BASIC_TYPE($1.type, TYPE_VOID))
		yyerror("Value thrown away"); */
	}
break;
case 77:
#line 1957 "lang.y"
{
		    if (current_break_address == 0)
			yyerror("break statement outside loop");
		    if (current_break_address & BREAK_ON_STACK) {
			ins_f_byte(F_BREAK);
		    } else {
		        ins_f_byte(F_LBRANCH);
			ins_short(current_break_address);
			current_break_address =
			  mem_block[A_PROGRAM].current_size - 2;
		    }
		}
break;
case 78:
#line 1970 "lang.y"
{
		    int depth;
#line 1843 "prolang.y"
		    if (current_continue_address == 0)
			yyerror("continue statement outside loop");
		    if (depth = (current_continue_address & SWITCH_DEPTH_MASK))
		    {
			while(depth > SWITCH_DEPTH_UNIT*256) {
			    ins_f_code(F_BREAKN_CONTINUE);
			    ins_byte(255);
			    ins_short(2);
			    depth -= SWITCH_DEPTH_UNIT*256;
			}
			if (depth > SWITCH_DEPTH_UNIT) {
			    depth /= SWITCH_DEPTH_UNIT;
			    ins_f_code(F_BREAKN_CONTINUE);
			    ins_byte(depth-1);
			} else {
			    ins_f_code(F_BREAK_CONTINUE);
			}
		    } else {
			ins_f_byte(F_LBRANCH);
		    }
		    ins_short(current_continue_address);
		    current_continue_address =
			( current_continue_address & SWITCH_DEPTH_MASK ) |
			( mem_block[A_PROGRAM].current_size - 2 );
		}
break;
case 79:
#line 2000 "lang.y"
{   yyval.numbers[0] = current_continue_address;
	    yyval.numbers[1] = current_break_address;
	    push_address();
	}
break;
case 80:
#line 2004 "lang.y"
{
#line 1876 "prolang.y"
	    int addr = pop_address();
	    int length = CURRENT_PROGRAM_SIZE - addr;
	    char *expression;

	    expression = yalloc(length+2);
	    memcpy(expression, mem_block[A_PROGRAM].block+addr, length);
	    if (last_expression == CURRENT_PROGRAM_SIZE - 1 &&
	        expression[length-1] == F_NOT - F_OFFSET	)
	    {
		length--;
		expression[length] = F_BBRANCH_WHEN_ZERO - F_OFFSET;
	    } else {
		expression[length] = F_BBRANCH_WHEN_NON_ZERO - F_OFFSET;
	    }
	    yyval.expression.p = expression;
	    yyval.expression.length = length;
	    yyval.expression.line = current_line;
	    CURRENT_PROGRAM_SIZE = addr;
	    last_expression = -1;
	    ins_f_byte(F_BRANCH); /* to expression */
	    push_address();
	    ins_byte(0);
	    current_continue_address = CONTINUE_DELIMITER;
	    current_break_address = BREAK_DELIMITER;
	}
break;
case 81:
#line 2032 "lang.y"
{
#line 1904 "prolang.y"
	  int offset;
	  int next_addr;
	  int addr = pop_address();

	  for(;current_continue_address > 0;
	    current_continue_address = next_addr) {
	      next_addr = read_short(current_continue_address);
	      upd_short(current_continue_address,
		  CURRENT_PROGRAM_SIZE - current_continue_address);
	  }
	  offset = FIX_BRANCH( F_LBRANCH, CURRENT_PROGRAM_SIZE, addr);
	  if (yyvsp[-1].expression.line != current_line)
	      store_line_number_info();
	  add_to_mem_block(A_PROGRAM, yyvsp[-1].expression.p, yyvsp[-1].expression.length+2);
	  yfree(yyvsp[-1].expression.p);
	  offset += addr + 1 - ( CURRENT_PROGRAM_SIZE - 1 );
	  if (offset < -0xff) {
	      char * codep;

	      if (offset < -0x8000)
		  yyerror("offset overflow");
	      codep = mem_block[A_PROGRAM].block + --CURRENT_PROGRAM_SIZE - 1;
	      *codep = *codep == F_BBRANCH_WHEN_NON_ZERO - F_OFFSET ?
	        F_LBRANCH_WHEN_NON_ZERO - F_OFFSET :
	        F_LBRANCH_WHEN_ZERO - F_OFFSET
	      ;
	      ins_short(offset);
	  } else {
	      mem_block[A_PROGRAM].block[CURRENT_PROGRAM_SIZE-1] = -offset;
	  }
	  if (yyvsp[-1].expression.line != current_line)
	      store_line_number_relocation(yyvsp[-1].expression.line);
	  for(;current_break_address > 0;current_break_address = next_addr) {
	      next_addr = read_short(current_break_address);
	      upd_short(current_break_address,
		  CURRENT_PROGRAM_SIZE - current_break_address);
	  }
	  current_continue_address = yyvsp[-6].numbers[0];
	  current_break_address    = yyvsp[-6].numbers[1];
        }
break;
case 82:
#line 2075 "lang.y"
{
	yyval.numbers[0] = current_continue_address;
	yyval.numbers[1] = current_break_address;
	current_break_address = BREAK_DELIMITER;
	current_continue_address = CONTINUE_DELIMITER;
	push_address();
    }
break;
case 83:
#line 2082 "lang.y"
{
	int next_addr;
	int current;
#line 1956 "prolang.y"
	current = CURRENT_PROGRAM_SIZE;
	for(;current_continue_address > 0;
	  current_continue_address = next_addr) {
	    next_addr = read_short(current_continue_address);
	    upd_short(current_continue_address,
		current - current_continue_address);
	}
    }
break;
case 84:
#line 2094 "lang.y"
{
#line 1966 "prolang.y"
	int offset;
	int next_addr;
	int addr = pop_address();
	int current;
	char *dest;
	char tmp_short[2];

	current = CURRENT_PROGRAM_SIZE;
	if (current + 3 > mem_block[A_PROGRAM].max_size)
	    realloc_a_program();
	dest = mem_block[A_PROGRAM].block + current;
	if (current == last_expression + 1 && dest[-1] == F_NOT - F_OFFSET) {
	    offset = addr - current;
	    if (offset < -0xff) {
		if (offset < -0x8000)
		    yyerror("offset overflow");
		*((short *)tmp_short) = offset;
		dest[-1] = F_LBRANCH_WHEN_ZERO - F_OFFSET;
		*dest++ = tmp_short[0];
		*dest   = tmp_short[1];
		current += 2;
	    } else {
		dest[-1] = F_BBRANCH_WHEN_ZERO - F_OFFSET;
		*dest = -offset;
		current++;
	    }
	} else {
	    offset = addr - ( current + 1 );
	    if (offset < -0xff) {
		if (offset < -0x8000)
		    yyerror("offset overflow");
		*((short *)tmp_short) = offset;
		*dest++ = F_LBRANCH_WHEN_NON_ZERO - F_OFFSET;
		*dest++ = tmp_short[0];
		*dest   = tmp_short[1];
		current += 3;
	    } else {
		*dest++ = F_BBRANCH_WHEN_NON_ZERO - F_OFFSET;
		*dest = -offset;
		current += 2;
	    }
	}
	CURRENT_PROGRAM_SIZE = current;
	for(;current_break_address > 0;current_break_address = next_addr) {
	    next_addr = read_short(current_break_address);
	    upd_short(current_break_address,
		current - current_break_address);
	}
	current_continue_address = yyvsp[-8].numbers[0];
	current_break_address    = yyvsp[-8].numbers[1];
    }
break;
case 85:
#line 2148 "lang.y"
{ yyval.numbers[0] = current_continue_address;
		    yyval.numbers[1] = current_break_address; }
break;
case 86:
#line 2151 "lang.y"
{   insert_pop_value();
	    current_continue_address = CONTINUE_DELIMITER;
	    yyval.number = CURRENT_PROGRAM_SIZE;
	}
break;
case 87:
#line 2156 "lang.y"
{
#line 2028 "prolang.y"
	    int start, length;
	    char *expression;

	    start = yyvsp[-2].number;
	    length = CURRENT_PROGRAM_SIZE - start;
	    expression = yalloc(length+2);
	    memcpy(expression,
	      mem_block[A_PROGRAM].block + start, length );
	    if (last_expression == CURRENT_PROGRAM_SIZE - 1 &&
	        expression[length-1] == F_NOT - F_OFFSET	)
	    {
		length--;
		expression[length] = F_BBRANCH_WHEN_ZERO - F_OFFSET;
	    } else {
		expression[length] = F_BBRANCH_WHEN_NON_ZERO - F_OFFSET;
	    }
	    yyval.expression.p = expression;
	    yyval.expression.length = length;
	    yyval.expression.line = current_line;
	    CURRENT_PROGRAM_SIZE = start;
	    last_expression = -1;
	}
break;
case 88:
#line 2181 "lang.y"
{
#line 2053 "prolang.y"
	    int length;

	    insert_pop_value();
	    length = CURRENT_PROGRAM_SIZE - yyvsp[-5].number;
	    yyval.expression.p = yalloc(length);
	    if (length)
		memcpy(yyval.expression.p,
		  mem_block[A_PROGRAM].block + yyvsp[-5].number, length );
	    yyval.expression.length = length;
	    yyval.expression.line = current_line;
	    CURRENT_PROGRAM_SIZE = yyvsp[-5].number;
	    last_expression = -1;
	    ins_f_byte(F_BRANCH); /* to expression */
	    ins_byte(0);
	    current_break_address = BREAK_DELIMITER;
	}
break;
case 89:
#line 2200 "lang.y"
{
#line 2072 "prolang.y"
	  int offset;
	  int next_addr;

	  for(;current_continue_address > 0;
	    current_continue_address = next_addr) {
	      next_addr = read_short(current_continue_address);
	      upd_short(current_continue_address,
		  CURRENT_PROGRAM_SIZE - current_continue_address);
	  }
	  if ( yyvsp[-4].expression.line != current_line ||
	       (yyvsp[-1].expression.line != current_line &&
	        yyvsp[-1].expression.length) )
	      store_line_number_info();
	  if (yyvsp[-1].expression.length) {
	      add_to_mem_block(A_PROGRAM,
	        yyvsp[-1].expression.p, yyvsp[-1].expression.length);
	      if (yyvsp[-1].expression.line != yyvsp[-4].expression.line)
		  store_line_number_relocation(yyvsp[-1].expression.line);
	  }
	  yfree(yyvsp[-1].expression.p);
	  offset =
	    FIX_BRANCH( F_LBRANCH, CURRENT_PROGRAM_SIZE, yyvsp[-7].number + 1);
	  add_to_mem_block(A_PROGRAM, yyvsp[-4].expression.p, yyvsp[-4].expression.length+2);
	  yfree(yyvsp[-4].expression.p);
	  offset += yyvsp[-7].number + 2 - ( CURRENT_PROGRAM_SIZE - 1 );
	  if (offset < -0xff) {
	      char * codep;

	      if (offset < -0x8000)
		  yyerror("offset overflow");
	      codep = mem_block[A_PROGRAM].block + --CURRENT_PROGRAM_SIZE - 1;
	      *codep = *codep == F_BBRANCH_WHEN_NON_ZERO - F_OFFSET ?
	        F_LBRANCH_WHEN_NON_ZERO - F_OFFSET :
	        F_LBRANCH_WHEN_ZERO - F_OFFSET
	      ;
	      ins_short(offset);
	  } else {
	      mem_block[A_PROGRAM].block[CURRENT_PROGRAM_SIZE-1] = -offset;
	  }
	  if (yyvsp[-4].expression.line != current_line)
	      store_line_number_relocation(yyvsp[-4].expression.line);
	  for(;current_break_address > 0;current_break_address = next_addr) {
	      next_addr = read_short(current_break_address);
	      upd_short(current_break_address,
		  CURRENT_PROGRAM_SIZE - current_break_address);
	  }
       current_continue_address = yyvsp[-10].numbers[0];
       current_break_address	= yyvsp[-10].numbers[1];
   }
break;
case 90:
#line 2253 "lang.y"
{
	    last_expression = mem_block[A_PROGRAM].current_size;
	    ins_f_byte(F_CONST1);
	}
break;
case 92:
#line 2260 "lang.y"
{
	struct s_case_state *statep;
#line 2133 "prolang.y"
        current_break_stack_need++;
        if ( current_break_stack_need > max_break_stack_need )
            max_break_stack_need = current_break_stack_need;
	if ( !(statep = (struct s_case_state *)
				yalloc(sizeof(struct s_case_state))) )
	{
	    yyerror("Out of memory");
	    YYACCEPT;
	}
	*statep = case_state;
	case_state.previous = statep;
	push_explicit(current_break_address);
	push_explicit(switch_pc);
	ins_f_byte(F_SWITCH);
	switch_pc = mem_block[A_PROGRAM].current_size;
	case_state.list0 = case_state.list1 = 0;
	case_state.zero = 0;
	case_state.no_string_labels = 1;
	case_state.some_numeric_labels = 0;
	case_state.default_addr = 0;
	ins_short(0);
	current_break_address =
		BREAK_ON_STACK | BREAK_FROM_SWITCH | CASE_LABELS_ENABLED ;
	if (current_continue_address)
	    current_continue_address += SWITCH_DEPTH_UNIT;
    }
break;
case 93:
#line 2290 "lang.y"
{
#line 2162 "prolang.y"
	struct s_case_state *statep;

	current_break_address &=
	    ~(BREAK_ON_STACK|BREAK_FROM_SWITCH|CASE_LABELS_ENABLED);

	if (!case_state.default_addr) {
	    /* no default given -> create one */
	    case_state.default_addr = CURRENT_PROGRAM_SIZE-switch_pc;
	}
	/* it isn't unusual that the last case/default has no break */
	ins_f_byte(F_BREAK);
	store_case_labels(
	  CURRENT_PROGRAM_SIZE-switch_pc,
	  case_state.default_addr,
	  case_state.no_string_labels | case_state.some_numeric_labels,
	  case_state.zero,
	  yyget_space, yymove_switch_instructions, yyerror, yycerrorl
	);
	switch_pc = pop_address();
	current_break_address = pop_address();
	statep = case_state.previous;
	case_state = *statep;
	yfree((char *)statep);
	if (current_continue_address)
	    current_continue_address -= SWITCH_DEPTH_UNIT;
        current_break_stack_need--;
    }
break;
case 94:
#line 2321 "lang.y"
{
#line 2193 "prolang.y"
	struct case_list_entry *temp;

	if ( !( current_break_address & CASE_LABELS_ENABLED ) ) {
	    yyerror("Case outside switch");
	    break;
	}
	if ( !(temp = new_case_entry()) ) {
	    yyerror("Out of memory");
	    break;
	}
	if ( !(temp->key = yyvsp[-1].case_label.key) ) {
	    case_state.zero = temp;
	}
	temp->addr = mem_block[A_PROGRAM].current_size - switch_pc;
	temp->line = current_line;
    }
break;
case 95:
#line 2340 "lang.y"
{
#line 2212 "prolang.y"
	struct case_list_entry *temp;

	if ( !yyvsp[-3].case_label.numeric || !yyvsp[-1].case_label.numeric )
	    yyerror("String case labels not allowed as range bounds");
	if ( !( current_break_address & CASE_LABELS_ENABLED ) ) {
	    yyerror("Case range outside switch");
	    break;
	}
	if (yyvsp[-3].case_label.key >= yyvsp[-1].case_label.key) {
	    if (yyvsp[-3].case_label.key > yyvsp[-1].case_label.key)
		break;
	    if ( !(temp = new_case_entry()) ) {
		yyerror("Out of memory");
		break;
	    }
	    temp->key = yyvsp[-3].case_label.key;
	    temp->addr = mem_block[A_PROGRAM].current_size - switch_pc;
	    temp->line = current_line;
	}
	if ( !(temp = new_case_entry()) ) {
	    yyerror("Out of memory");
	    break;
	}
	temp->key = yyvsp[-3].case_label.key;
	temp->addr = 1;
	temp->line = current_line;
	if ( !(temp = new_case_entry()) ) {
	    yyerror("Out of memory");
	    break;
	}
	temp->key = yyvsp[-1].case_label.key;
	temp->addr = mem_block[A_PROGRAM].current_size - switch_pc;
	temp->line = 0;
    }
break;
case 96:
#line 2378 "lang.y"
{
#line 2250 "prolang.y"
	    if ( yyval.case_label.key = yyvsp[0].number ) {
		if ( !(case_state.no_string_labels) )
		    yyerror("Mixed case label list not allowed");
		case_state.some_numeric_labels = 1;
	    }
	    yyval.case_label.numeric = 1;
	}
break;
case 97:
#line 2388 "lang.y"
{
#line 2260 "prolang.y"
	    if ( case_state.some_numeric_labels )
		yyerror("Mixed case label list not allowed");
	    case_state.no_string_labels = 0;
            store_prog_string(last_string_constant);
            yyval.case_label.key = (int)last_string_constant;
	    yyval.case_label.numeric = 0;
	    last_string_constant = 0;
        }
break;
case 98:
#line 2401 "lang.y"
{ yyval.number = yyvsp[-2].number |  yyvsp[0].number; }
break;
case 99:
#line 2402 "lang.y"
{ yyval.number = yyvsp[-2].number ^  yyvsp[0].number; }
break;
case 100:
#line 2403 "lang.y"
{ yyval.number = yyvsp[-2].number &  yyvsp[0].number; }
break;
case 101:
#line 2404 "lang.y"
{ yyval.number = yyvsp[-2].number == yyvsp[0].number; }
break;
case 102:
#line 2405 "lang.y"
{ yyval.number = yyvsp[-2].number != yyvsp[0].number; }
break;
case 103:
#line 2406 "lang.y"
{ yyval.number = yyvsp[-2].number >  yyvsp[0].number; }
break;
case 104:
#line 2407 "lang.y"
{ yyval.number = yyvsp[-2].number >= yyvsp[0].number; }
break;
case 105:
#line 2408 "lang.y"
{ yyval.number = yyvsp[-2].number <  yyvsp[0].number; }
break;
case 106:
#line 2409 "lang.y"
{ yyval.number = yyvsp[-2].number <= yyvsp[0].number; }
break;
case 107:
#line 2410 "lang.y"
{ yyval.number = yyvsp[0].number > MAX_SHIFT ? 0 : yyvsp[-2].number << yyvsp[0].number; }
break;
case 108:
#line 2411 "lang.y"
{ yyval.number = yyvsp[-2].number >> (yyvsp[0].number > MAX_SHIFT ? MAX_SHIFT : yyvsp[0].number); }
break;
case 109:
#line 2412 "lang.y"
{ yyval.number = yyvsp[-2].number +  yyvsp[0].number; }
break;
case 110:
#line 2413 "lang.y"
{ yyval.number = yyvsp[-2].number -  yyvsp[0].number; }
break;
case 111:
#line 2414 "lang.y"
{ yyval.number = yyvsp[-2].number *  yyvsp[0].number; }
break;
case 112:
#line 2416 "lang.y"
{
	if (yyvsp[0].number) {
	    yyval.number = yyvsp[-2].number % yyvsp[0].number;
	} else {
	    yyerror("division by zero");
	    yyval.number = 0;
	}
      }
break;
case 113:
#line 2425 "lang.y"
{
	if (yyvsp[0].number) {
	    yyval.number = yyvsp[-2].number / yyvsp[0].number;
	} else {
	    yyerror("division by zero");
	    yyval.number = 0;
	}
      }
break;
case 114:
#line 2433 "lang.y"
{ yyval.number = yyvsp[-1].number; }
break;
case 115:
#line 2434 "lang.y"
{ yyval.number = -yyvsp[0].number; }
break;
case 116:
#line 2435 "lang.y"
{ yyval.number = !yyvsp[0].number; }
break;
case 117:
#line 2436 "lang.y"
{ yyval.number = ~yyvsp[0].number; }
break;
case 119:
#line 2441 "lang.y"
{
#line 2313 "prolang.y"
	if ( !( current_break_address & CASE_LABELS_ENABLED ) ) {
	    yyerror("Default outside switch");
	    break;
	}
	if (case_state.default_addr)
	    yyerror("Duplicate default");
	case_state.default_addr =
	  mem_block[A_PROGRAM].current_size - switch_pc;
    }
break;
case 121:
#line 2456 "lang.y"
{
	    insert_pop_value();
	}
break;
case 122:
#line 2460 "lang.y"
{ yyval.lrvalue.type = yyvsp[0].lrvalue.type; }
break;
case 123:
#line 2464 "lang.y"
{
	    int length;
	    int type2;
#line 2338 "prolang.y"
	    type2 = yyvsp[0].lrvalue.type;
	    if (exact_types && !compatible_types(yyvsp[-2].lvalue.type, type2) &&
		!( yyvsp[-2].lvalue.type == TYPE_STRING &&
		   (type2 == TYPE_NUMBER || type2 == TYPE_FLOAT) &&
		   yyvsp[-1].number == F_ADD_EQ-F_OFFSET))
	    {
		type_error("Bad assignment. Rhs", yyvsp[0].lrvalue.type);
	    }
	    if (type2 & TYPE_MOD_REFERENCE)
		yyerror("Can't trace reference assignments.");
	    length = yyvsp[-2].lvalue.length;
	    if (length) {
		add_to_mem_block
		  (A_PROGRAM, yyvsp[-2].lvalue.u.p, length+1);
		yfree(yyvsp[-2].lvalue.u.p);
		mem_block[A_PROGRAM].block[
		  last_expression = CURRENT_PROGRAM_SIZE-1
		] = yyvsp[-1].number;
	    } else {
		char *source, *dest;
		int current_size;

		source = yyvsp[-2].lvalue.u.simple;
		current_size = CURRENT_PROGRAM_SIZE;
		CURRENT_PROGRAM_SIZE = (last_expression = current_size + 2) + 1;
		if (current_size + 3 > mem_block[A_PROGRAM].max_size )
		    if (!realloc_a_program()) {
			yyerror("Out of memory");
			YYACCEPT;
		    }
		dest = mem_block[A_PROGRAM].block + current_size;
		*dest++ = *source++;
		*dest++ = *source;
		*dest = yyvsp[-1].number;
	    }
	    yyval.lrvalue.type = type2;
	}
break;
case 124:
#line 2506 "lang.y"
{   yyerror("Illegal LHS"); yyval.lrvalue.type = TYPE_ANY; }
break;
case 125:
#line 2509 "lang.y"
{
	    ins_f_byte(F_BRANCH_WHEN_ZERO);
	    yyval.number = CURRENT_PROGRAM_SIZE;
	    ins_byte(0);
	}
break;
case 126:
#line 2515 "lang.y"
{
	    int address, offset;

	    address = yyvsp[-1].number;
	    ins_f_byte(F_BRANCH);
	    yyval.number = CURRENT_PROGRAM_SIZE;
	    ins_byte(0);
	    offset = CURRENT_PROGRAM_SIZE - ( address + 1);
	    if (offset > 0xff - 1) {
		int i;
		char *p;

		yyval.number = CURRENT_PROGRAM_SIZE;
		ins_byte(0);
		p = mem_block[A_PROGRAM].block +
		    mem_block[A_PROGRAM].current_size-1;
		for( i = offset; --i >= 0; --p ) *p = p[-1];
		p[-2] = F_LBRANCH_WHEN_ZERO-F_OFFSET;
		upd_short(address, offset+2);
		if (offset > 0x7ffd)
		    yyerror("offset overflow");
	    } else {
		mem_block[A_PROGRAM].block[address] = offset;
	    }
	}
break;
case 127:
#line 2541 "lang.y"
{
	    int address, old_address;
	    int offset;
	    int type1, type2;

	    last_expression = -1;
	    old_address = yyvsp[-4].number;
	    address = yyvsp[-2].number;
	    offset = mem_block[A_PROGRAM].current_size - ( address + 1);
	    if (offset > 0xff) {
		int i;
		char *p;

		ins_byte(0);
		p = mem_block[A_PROGRAM].block +
		    mem_block[A_PROGRAM].current_size-1;
		for( i = offset; --i >= 0; --p ) *p = p[-1];
		p[-2] = F_LBRANCH-F_OFFSET;
		upd_short(address, offset+2);
		if (offset > 0x7ffd)
		    yyerror("offset overflow");
		if ( mem_block[A_PROGRAM].block[old_address-1] ==
		    F_BRANCH_WHEN_ZERO-F_OFFSET )
		    mem_block[A_PROGRAM].block[old_address]++;
		else
		    upd_short(old_address,read_short(old_address)+1);
	    } else {
		mem_block[A_PROGRAM].block[address] = offset;
	    }
	    type1 = yyvsp[-3].lrvalue.type;
	    type2 = yyvsp[0].lrvalue.type;
	    if (exact_types && !compatible_types(type1, type2) &&
		instrs[F_CALL_OTHER-F_OFFSET].ret_type != TYPE_ANY)
	    {
		type_error("Different types in ?: expr", type1);
		type_error("                      and ", type2);
	    }
	    if (type1 == TYPE_ANY) yyval.lrvalue.type = type2;
	    else if (type2 == TYPE_ANY) yyval.lrvalue.type = type1;
	    else if (type1 == (TYPE_MOD_POINTER|TYPE_ANY) )
		yyval.lrvalue.type = type2;
	    else if (type2 == (TYPE_MOD_POINTER|TYPE_ANY) )
		yyval.lrvalue.type = type1;
	    else
		yyval.lrvalue.type = type1;
	}
break;
case 128:
#line 2589 "lang.y"
{
	    ins_f_byte(F_LOR);
	    yyval.number = CURRENT_PROGRAM_SIZE;
	    ins_byte(0);
	}
break;
case 129:
#line 2595 "lang.y"
{
	    int address, offset;

	    last_expression = -1;
	    address = yyvsp[-1].number;
	    offset = mem_block[A_PROGRAM].current_size - ( address + 1);
	    if (offset > 0xff) {
		int i;
		char *p;

		ins_short(0);
		ins_byte(0);
		p = mem_block[A_PROGRAM].block +
		    mem_block[A_PROGRAM].current_size-1;
		for( i = offset; --i >= 0; --p ) *p = p[-3];
		p[-4] = F_DUP-F_OFFSET;
		p[-3] = F_LBRANCH_WHEN_NON_ZERO-F_OFFSET;
		upd_short(address+1, offset+3);
		if (offset > 0x7ffc)
		    yyerror("offset overflow");
		p[0]  = F_POP_VALUE-F_OFFSET;
	    } else {
		mem_block[A_PROGRAM].block[address] = offset;
	    }
	    if (yyvsp[-3].lrvalue.type == yyvsp[0].lrvalue.type)
		yyval.lrvalue.type = yyvsp[-3].lrvalue.type;
	    else
		yyval.lrvalue.type = TYPE_ANY;	/* Return type can't be known */
	}
break;
case 130:
#line 2626 "lang.y"
{
	    ins_f_byte(F_LAND);
	    yyval.number = CURRENT_PROGRAM_SIZE;
	    ins_byte(0);
	}
break;
case 131:
#line 2632 "lang.y"
{
	    int address, offset;

	    last_expression = -1;
	    address = yyvsp[-1].number;
	    offset = mem_block[A_PROGRAM].current_size - ( address + 1);
	    if (offset > 0xff) {
		int i;
		char *p;

		ins_short(0);
		ins_byte(0);
		p = mem_block[A_PROGRAM].block +
		    mem_block[A_PROGRAM].current_size-1;
		for( i = offset; --i >= 0; --p ) *p = p[-3];
		p[-4] = F_DUP-F_OFFSET;
		p[-3] = F_LBRANCH_WHEN_ZERO-F_OFFSET;
		upd_short(address+1, offset+3);
		if (offset > 0x7ffc)
		    yyerror("offset overflow");
		p[0]  = F_POP_VALUE-F_OFFSET;
	    } else {
		mem_block[A_PROGRAM].block[address] = offset;
	    }
	    if (yyvsp[-3].lrvalue.type == yyvsp[0].lrvalue.type)
		yyval.lrvalue.type = yyvsp[-3].lrvalue.type;
	    else
		yyval.lrvalue.type = TYPE_ANY;	/* Return type can't be known */
	}
break;
case 132:
#line 2663 "lang.y"
{
	      if (exact_types && !BASIC_TYPE(yyvsp[-2].lrvalue.type,TYPE_NUMBER))
		  type_error("Bad argument 1 to |", yyvsp[-2].lrvalue.type);
	      if (exact_types && !BASIC_TYPE(yyvsp[0].lrvalue.type,TYPE_NUMBER))
		  type_error("Bad argument 2 to |", yyvsp[0].lrvalue.type);
	      yyval.lrvalue.type = TYPE_NUMBER;
	      ins_f_byte(F_OR);
	  }
break;
case 133:
#line 2673 "lang.y"
{
	      if (exact_types && !BASIC_TYPE(yyvsp[-2].lrvalue.type,TYPE_NUMBER))
		  type_error("Bad argument 1 to ^", yyvsp[-2].lrvalue.type);
	      if (exact_types && !BASIC_TYPE(yyvsp[0].lrvalue.type,TYPE_NUMBER))
		  type_error("Bad argument 2 to ^", yyvsp[0].lrvalue.type);
	      yyval.lrvalue.type = TYPE_NUMBER;
	      ins_f_byte(F_XOR);
	  }
break;
case 134:
#line 2683 "lang.y"
{
	    ins_f_byte(F_AND);
	    yyval.lrvalue.type = TYPE_ANY;
	    if (exact_types) {
		short first_type  = yyvsp[-2].lrvalue.type;
		short second_type = yyvsp[0].lrvalue.type;

		if ( first_type == TYPE_ANY &&
		    second_type == TYPE_ANY )
		{
		    /* $$ == TYPE_ANY is correct */
		} else if ( (first_type | second_type) & TYPE_MOD_POINTER) {
		    if (first_type  == TYPE_NUMBER ||
			second_type == TYPE_NUMBER)
		    {
			yyerrorf("Incompatible types for arguments to & %s"
			  ,get_two_types(first_type, second_type));
		    } else if ( ( !( first_type  & TYPE_MOD_POINTER ) ||
				  first_type  & TYPE_MOD_REFERENCE) &&
				first_type  != TYPE_ANY              ) {
			type_error("Bad argument 1 to &", first_type );
		    } else if ( ( !( second_type & TYPE_MOD_POINTER ) ||
				  second_type & TYPE_MOD_REFERENCE) &&
				second_type != TYPE_ANY	       ) {
			type_error("Bad argument 2 to &", first_type );
		    } else if ( !BASIC_TYPE(first_type &~TYPE_MOD_POINTER,
					   second_type &~TYPE_MOD_POINTER) )
		    {
			yyerrorf("Incompatible types for arguments to & %s"
			  ,get_two_types(first_type, second_type));
		    } else {
			yyval.lrvalue.type = TYPE_ANY | TYPE_MOD_POINTER;
		    }
		} else {
		    if ( !BASIC_TYPE(first_type ,TYPE_NUMBER) )
			type_error("Bad argument 1 to &", first_type );
		    if ( !BASIC_TYPE(second_type,TYPE_NUMBER) )
			type_error("Bad argument 2 to &", second_type);
		    yyval.lrvalue.type = TYPE_NUMBER;
		}
	    } /* end of exact_types code */
	}
break;
case 135:
#line 2727 "lang.y"
{
	    int t1 = yyvsp[-2].lrvalue.type, t2 = yyvsp[0].lrvalue.type;
	    if (exact_types && t1 != t2 && t1 != TYPE_ANY && t2 != TYPE_ANY) {
		type_error("== always false because of different types",
		  yyvsp[-2].lrvalue.type);
		type_error("                               compared to",
		  yyvsp[0].lrvalue.type);
	    }
	    ins_f_byte(F_EQ);
	    yyval.lrvalue.type = TYPE_NUMBER;
	}
break;
case 136:
#line 2739 "lang.y"
{
	    int t1 = yyvsp[-2].lrvalue.type, t2 = yyvsp[0].lrvalue.type;
	    if (exact_types && t1 != t2 && t1 != TYPE_ANY && t2 != TYPE_ANY) {
		type_error("!= always true because of different types",
		  yyvsp[-2].lrvalue.type);
		type_error("                              compared to",
		  yyvsp[0].lrvalue.type);
	    }
	    ins_f_byte(F_NE);
	    yyval.lrvalue.type = TYPE_NUMBER;
	}
break;
case 137:
#line 2752 "lang.y"
{ yyval.lrvalue.type = TYPE_NUMBER; ins_f_byte(F_GT); }
break;
case 138:
#line 2754 "lang.y"
{ yyval.lrvalue.type = TYPE_NUMBER; ins_f_byte(F_GE); }
break;
case 139:
#line 2756 "lang.y"
{ yyval.lrvalue.type = TYPE_NUMBER; ins_f_byte(F_LT); }
break;
case 140:
#line 2758 "lang.y"
{ yyval.lrvalue.type = TYPE_NUMBER; ins_f_byte(F_LE); }
break;
case 141:
#line 2761 "lang.y"
{
	    ins_f_byte(F_LSH);
	    yyval.lrvalue.type = TYPE_NUMBER;
	    if (exact_types) {
		if (!BASIC_TYPE(yyvsp[-2].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad argument number 1 to '<<'", yyvsp[-2].lrvalue.type);
		if (!BASIC_TYPE(yyvsp[0].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad argument number 2 to '<<'", yyvsp[0].lrvalue.type);
	    }
	}
break;
case 142:
#line 2772 "lang.y"
{
	    ins_f_byte(F_RSH);
	    yyval.lrvalue.type = TYPE_NUMBER;
	    if (exact_types) {
		if (!BASIC_TYPE(yyvsp[-2].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad argument number 1 to '>>'", yyvsp[-2].lrvalue.type);
		if (!BASIC_TYPE(yyvsp[0].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad argument number 2 to '>>'", yyvsp[0].lrvalue.type);
	    }
	}
break;
case 143:
#line 2784 "lang.y"
{
#line 2656 "prolang.y"
	    yyval.numbers[0] = last_expression;
	    yyval.numbers[1] = last_string_is_new;
	}
break;
case 144:
#line 2790 "lang.y"
{
	    /* Type checks of this case are complicated */
	    int current_size;
	    unsigned char *p;
#line 2665 "prolang.y"
	    if (pragma_combine_strings &&
		last_expression + 2 == (current_size = CURRENT_PROGRAM_SIZE) &&
		yyvsp[-1].numbers[0] + 4 == current_size &&
		(((p = &mem_block[A_PROGRAM].block[current_size])[-2] -
		  (F_CSTRING0 - F_OFFSET)) & ~3) == 0 &&
		((p[-4] - (F_CSTRING0 - F_OFFSET)) & ~3) == 0
	    ) {
		char *str1, *str2, *sum;
		int i;

		str1 = ((char**)(mem_block[A_STRINGS].block))
		  [p[-3] | (p[-4]-(F_CSTRING0-F_OFFSET))<<8 ];
		str2 = ((char**)(mem_block[A_STRINGS].block))
		  [p[-1] | (p[-2]-(F_CSTRING0-F_OFFSET))<<8 ];
		sum = xalloc(strlen(str1) + strlen(str2) + 1);
		strcpy(sum, str1);
		strcat(sum, str2);
		if (last_string_is_new) {
		    delete_prog_string();
		}
		if (yyvsp[-1].numbers[1]) {
		    delete_prog_string();
		}
		i = store_prog_string(make_shared_string(sum));
		xfree(sum);
		last_expression = current_size - 4;
		if (i < 0x400) {
		    p[-4] = F_CSTRING0 - F_OFFSET + (i>>8);
		    p[-3] = i;
		    CURRENT_PROGRAM_SIZE = current_size - 2;
		} else {
		    p[-4] = F_STRING-F_OFFSET;
		    upd_short(current_size - 3, i);
		    CURRENT_PROGRAM_SIZE = current_size - 1;
		}
		yyval.lrvalue.type = TYPE_STRING;
	    } else {
		ins_f_byte(F_ADD);
		yyval.lrvalue.type = TYPE_ANY;
	    }
	}
break;
case 145:
#line 2837 "lang.y"
{
#line 2709 "prolang.y"
	    yyval.lrvalue.type = TYPE_ANY;
	    if (exact_types) {
		int type1 = yyvsp[-2].lrvalue.type;
		int type2 = yyvsp[0].lrvalue.type;

		if (type1 == type2) {
		    static char matchok[] =
{0,1,0,0,0,1,1,1,0,0,0,0,0,};

		    if ( type1 & (TYPE_MOD_POINTER|TYPE_MOD_REFERENCE) ?
			   (type1 & (TYPE_MOD_POINTER|TYPE_MOD_REFERENCE)) ==
			     TYPE_MOD_POINTER :
			   matchok[type1]
		       )
		    {
			yyval.lrvalue.type = type1;
		    } else {
			type_error("Bad arguments to '-'", type1);
		    }
		} else if ( (type1 & (TYPE_MOD_POINTER|TYPE_MOD_REFERENCE)) ==
		    TYPE_MOD_POINTER)
		{

		    if ( (type2 | TYPE_MOD_POINTER) ==
			 (TYPE_MOD_POINTER|TYPE_ANY)   ||
			 type2 & TYPE_MOD_POINTER &&
			 type1 == (TYPE_MOD_POINTER|TYPE_ANY)
		       )
		    {
			yyval.lrvalue.type = type1;
		    } else {
			yyerror("Arguments to '-' don't match");
		    }
		} else switch (type1) {
		  case TYPE_ANY:
		    switch (type2) {
		      case TYPE_NUMBER:
			/* number or float -> TYPE_ANY */
			break;
		      case TYPE_MAPPING:
		      case TYPE_FLOAT:
			yyval.lrvalue.type = type2;
			break;
		      default:
			if ( (type2 & (TYPE_MOD_POINTER|TYPE_MOD_REFERENCE)) ==
			     TYPE_MOD_POINTER)
			{
			    yyval.lrvalue.type = TYPE_ANY | TYPE_MOD_POINTER;
			    break;
			} else {
			    type_error("Bad argument number 2 to '-'", type2);
			    break;
			}
		    }
		    break;
		  case TYPE_NUMBER:
		    if (type2 == TYPE_FLOAT || type2 == TYPE_ANY) {
			yyval.lrvalue.type = type2;
		    } else {
			yyerror("Arguments to '-' don't match");
		    }
		    break;
		  case TYPE_FLOAT:
		    if (type2 == TYPE_NUMBER || type2 == TYPE_ANY) {
			yyval.lrvalue.type = TYPE_FLOAT;
		    } else {
			yyerror("Arguments to '-' don't match");
		    }
		    break;
		  case TYPE_MAPPING:
		    if (type2 == TYPE_ANY) {
			yyval.lrvalue.type = type1;
		    } else {
			yyerror("Arguments to '-' don't match");
		    }
		    break;
		  default:
                    type_error("Bad argument number 1 to '-'", type1);
		    break;
		}
	    }
	    ins_f_byte(F_SUBTRACT);
	}
break;
case 146:
#line 2924 "lang.y"
{
	    int type1, type2;

	    type1 = yyvsp[-2].lrvalue.type;
	    type2 = yyvsp[0].lrvalue.type;
	    if (exact_types) {
		if ( !BASIC_TYPE(type1, TYPE_NUMBER) && type1 != TYPE_FLOAT)
		    type_error("Bad argument number 1 to '*'", type1);
		if ( !BASIC_TYPE(type2, TYPE_NUMBER) && type2 != TYPE_FLOAT)
		    type_error("Bad argument number 2 to '*'", type2);
	    }
	    ins_f_byte(F_MULTIPLY);
	    if (type1 == TYPE_FLOAT || type2 == TYPE_FLOAT )
	    {
	        yyval.lrvalue.type = TYPE_FLOAT;
	    } else {
	        yyval.lrvalue.type = TYPE_NUMBER;
	    }
	}
break;
case 147:
#line 2944 "lang.y"
{
	    if (exact_types) {
		if (!BASIC_TYPE(yyvsp[-2].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad argument number 1 to '%'", yyvsp[-2].lrvalue.type);
		if (!BASIC_TYPE(yyvsp[0].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad argument number 2 to '%'", yyvsp[0].lrvalue.type);
	    }
	    ins_f_byte(F_MOD);
	    yyval.lrvalue.type = TYPE_NUMBER;
	}
break;
case 148:
#line 2955 "lang.y"
{
	    int type1, type2;

	    type1 = yyvsp[-2].lrvalue.type;
	    type2 = yyvsp[0].lrvalue.type;
	    if (exact_types) {
		if ( !BASIC_TYPE(type1, TYPE_NUMBER) && type1 != TYPE_FLOAT)
		    type_error("Bad argument number 1 to '/'", type1);
		if ( !BASIC_TYPE(type2, TYPE_NUMBER) && type2 != TYPE_FLOAT)
		    type_error("Bad argument number 2 to '/'", type2);
	    }
	    ins_f_byte(F_DIVIDE);
	    if (type1 == TYPE_FLOAT || type2 == TYPE_FLOAT )
	    {
	        yyval.lrvalue.type = TYPE_FLOAT;
	    } else {
	        yyval.lrvalue.type = TYPE_NUMBER;
	    }
	}
break;
case 149:
#line 2976 "lang.y"
{
		  yyval.lrvalue.type = yyvsp[-1].type;
		  if (exact_types && yyvsp[0].lrvalue.type != TYPE_ANY &&
		      yyvsp[0].lrvalue.type != TYPE_UNKNOWN && yyvsp[-1].type != TYPE_VOID)
		      type_error("Casts are only legal for type mixed, or when unknown", yyvsp[0].lrvalue.type);
	      }
break;
case 150:
#line 2984 "lang.y"
{
		  yyval.lrvalue.type = yyvsp[-1].type;
		  if (yyvsp[0].lrvalue.type != TYPE_ANY &&
		      yyvsp[0].lrvalue.type != TYPE_UNKNOWN && yyvsp[-1].type != TYPE_VOID) {
		      switch(yyvsp[-1].type) {
		        default:
			  type_error("Illegal cast", yyvsp[-1].type);
			  break;
		        case TYPE_NUMBER:
			  ins_f_code(F_TO_INT);
			  break;
		        case TYPE_FLOAT:
			  ins_f_code(F_TO_FLOAT);
			  break;
		        case TYPE_STRING:
			  ins_f_code(F_TO_STRING);
			  break;
		        case TYPE_OBJECT:
			  ins_f_code(F_TO_OBJECT);
			  break;
		        case TYPE_NUMBER|TYPE_MOD_POINTER:
			  ins_f_code(F_TO_ARRAY);
			  break;
		      }
		  }
	      }
break;
case 151:
#line 3012 "lang.y"
{
	    int i;
	    PREPARE_S_INSERT(4)
#line 2886 "prolang.y"
	    i = verify_declared(yyvsp[0].ident);
	    if (i != -1) {
		if (i & VIRTUAL_VAR_TAG) {
		    add_f_byte(F_PUSH_VIRTUAL_VARIABLE_LVALUE);
		    add_byte(i);
		    i = V_VARIABLE(i)->flags & TYPE_MOD_MASK;
		} else {
		    if (i + num_virtual_variables & ~0xff) {
			add_f_byte(F_PUSH_IDENTIFIER16_LVALUE);
			add_short(i + num_virtual_variables);
		    } else {
			add_f_byte(F_PUSH_IDENTIFIER_LVALUE);
			add_byte(i + num_virtual_variables);
		    }
		    i = NV_VARIABLE(i)->flags & TYPE_MOD_MASK;
		}
		if (exact_types && !BASIC_TYPE(i, TYPE_NUMBER)) {
		    argument_type_error(yyvsp[-1].number, i);
		}
	    }
	    CURRENT_PROGRAM_SIZE =
	      (last_expression = CURRENT_PROGRAM_SIZE + 2) + 1;
	    add_byte(yyvsp[-1].number);
	    yyval.lrvalue.type = TYPE_NUMBER;
	}
break;
case 152:
#line 3042 "lang.y"
{
	    int i;
	    PREPARE_INSERT(3)
#line 2916 "prolang.y"
	    add_f_byte(F_PUSH_LOCAL_VARIABLE_LVALUE);
	    add_byte(yyvsp[0].number);
	    CURRENT_PROGRAM_SIZE =
	      (last_expression = CURRENT_PROGRAM_SIZE + 2) + 1;
	    add_byte(yyvsp[-1].number);
	    i = type_of_locals[yyvsp[0].number];
	    if (exact_types && !BASIC_TYPE(i, TYPE_NUMBER)) {
		argument_type_error(yyvsp[-1].number, i);
	    }
	    yyval.lrvalue.type = TYPE_NUMBER;
	}
break;
case 153:
#line 3058 "lang.y"
{
	    /* the ',' operator is reserved for indexing on multi-valued
	     * mappings and other multi-dimensional data
	     */
	    int current;
	    char *p;
	    int start;
#line 2936 "prolang.y"
	    if (yyvsp[-1].lrvalue.type & TYPE_MOD_REFERENCE)
		yyerror("Reference used as index");
	    if (exact_types) {
		int type;

		type = yyvsp[-3].lrvalue.type;
		switch (type) {
		  default:
		    if (type & TYPE_MOD_POINTER)
			argument_type_error(yyvsp[-4].number, type);
		    else
			type_error("Bad type to indexed lvalue", type);
		    break;
		  case TYPE_ANY:
#ifdef MAPPINGS
		  case TYPE_MAPPING:
		    break;
#endif
		  case TYPE_STRING:
		  case TYPE_MOD_POINTER|TYPE_ANY:
		  case TYPE_MOD_POINTER|TYPE_NUMBER:
		    if (!BASIC_TYPE(yyvsp[-1].lrvalue.type, TYPE_NUMBER))
			type_error("Bad type of index", yyvsp[-1].lrvalue.type);
		  ;
		}
	    }
	    current = CURRENT_PROGRAM_SIZE;
	    start = yyvsp[-3].lrvalue.start;
	    if (yyvsp[-3].lrvalue.code >= 0) {
		if (yyvsp[-3].lrvalue.end) {
		    int length;
		    char *q;

		    length = yyvsp[-3].lrvalue.end - start + 1;
		    if (current+length > mem_block[A_PROGRAM].max_size)
			realloc_a_program();
		    p = mem_block[A_PROGRAM].block;
		    memcpy(
			p + current,
			p + start,
			length
		    );
		    p += start;
		    q = p + length;
		    length = current - start;
		    for( ; --length >= 0; ) *p++ = *q++;
		    if (yyvsp[-3].lrvalue.code == F_PUSH_IDENTIFIER16_LVALUE - F_OFFSET)
			p[-3] = yyvsp[-3].lrvalue.code;
		    else
			p[-1] = yyvsp[-3].lrvalue.code;
		    *p++ = F_INDEX_LVALUE - F_OFFSET;
		} else {
		    int i;
		    int length;

		    if (current + 2 > mem_block[A_PROGRAM].max_size)
			realloc_a_program();
		    p = mem_block[A_PROGRAM].block + start;
		    i = p[1];
		    length = current - start - 2;
		    for( ; --length >= 0; p++) *p = p[2];
		    *p++ = yyvsp[-3].lrvalue.code;
		    *p++ = i;
		    *p++ = F_INDEX_LVALUE - F_OFFSET;
		}
	    } else {
		if (current + 2 > mem_block[A_PROGRAM].max_size)
		    realloc_a_program();
		p = mem_block[A_PROGRAM].block + start;
		*p++ = F_PUSH_INDEXED_LVALUE - F_OFFSET;
	    }
	    *p = yyvsp[-4].number;
	    last_expression = current + 1;
	    CURRENT_PROGRAM_SIZE = current + 2;
	    yyval.lrvalue.type = TYPE_NUMBER;
	}
break;
case 154:
#line 3143 "lang.y"
{
	    /* the ',' operator is reserved for indexing on multi-valued
	     * mappings and other multi-dimensional data
	     */
	    int current;
	    char *p;
	    int start;
#line 3021 "prolang.y"
	    if (exact_types) {
		int type;

		type = yyvsp[-4].lrvalue.type;
		if (type & TYPE_MOD_POINTER) {
		    if (type != (TYPE_MOD_POINTER|TYPE_ANY) &&
			type != (TYPE_MOD_POINTER|TYPE_NUMBER) )
			argument_type_error(yyvsp[-5].number, type);
		} else switch (type) {
		  default:
		    type_error("Bad type to indexed lvalue", type);
		  case TYPE_STRING:
		  case TYPE_ANY:
		    if (!BASIC_TYPE(yyvsp[-1].lrvalue.type, TYPE_NUMBER))
			type_error("Bad type of index", yyvsp[-1].lrvalue.type);
		  ;
		}
	    }
	    current = CURRENT_PROGRAM_SIZE;
	    start = yyvsp[-4].lrvalue.start;
	    if (yyvsp[-4].lrvalue.code >= 0) {
		if (yyvsp[-4].lrvalue.end) {
		    int length;
		    char *q;

		    length = yyvsp[-4].lrvalue.end - start + 1;
		    if (current+length > mem_block[A_PROGRAM].max_size)
			realloc_a_program();
		    p = mem_block[A_PROGRAM].block;
		    memcpy(
			p + current,
			p + start,
			length
		    );
		    p += start;
		    q = p + length;
		    length = current - start;
		    for( ; --length >= 0; ) *p++ = *q++;
		    if (yyvsp[-4].lrvalue.code == F_PUSH_IDENTIFIER16_LVALUE - F_OFFSET)
			p[-3] = yyvsp[-4].lrvalue.code;
		    else
			p[-1] = yyvsp[-4].lrvalue.code;
		    *p++ = F_RINDEX_LVALUE - F_OFFSET;
		} else {
		    int i;
		    int length;

		    if (current + 2 > mem_block[A_PROGRAM].max_size)
			realloc_a_program();
		    p = mem_block[A_PROGRAM].block + start;
		    i = p[1];
		    length = current - start - 2;
		    for( ; --length >= 0; p++) *p = p[2];
		    *p++ = yyvsp[-4].lrvalue.code;
		    *p++ = i;
		    *p++ = F_RINDEX_LVALUE - F_OFFSET;
		}
	    } else {
		if (current + 2 > mem_block[A_PROGRAM].max_size)
		    realloc_a_program();
		p = mem_block[A_PROGRAM].block + start;
		*p++ = F_PUSH_RINDEXED_LVALUE - F_OFFSET;
	    }
	    *p = yyvsp[-5].number;
	    last_expression = current + 1;
	    CURRENT_PROGRAM_SIZE = current + 2;
	    yyval.lrvalue.type = TYPE_NUMBER;
	}
break;
case 155:
#line 3220 "lang.y"
{
	    last_expression = CURRENT_PROGRAM_SIZE;
	    ins_f_byte(F_NOT);	/* Any type is valid here. */
	    yyval.lrvalue.type = TYPE_NUMBER;
	}
break;
case 156:
#line 3226 "lang.y"
{
	    ins_f_byte(F_COMPL);
	    if (exact_types && !BASIC_TYPE(yyvsp[0].lrvalue.type, TYPE_NUMBER))
		type_error("Bad argument to ~", yyvsp[0].lrvalue.type);
	    yyval.lrvalue.type = TYPE_NUMBER;
	}
break;
case 157:
#line 3233 "lang.y"
{
	    int type;

	    if (CURRENT_PROGRAM_SIZE - last_expression == 2 &&
		mem_block[A_PROGRAM].block[last_expression] ==
		  F_CLIT - F_OFFSET )
	    {
		mem_block[A_PROGRAM].block[last_expression] =
		  F_NCLIT - F_OFFSET;
	    } else {
		ins_f_byte(F_NEGATE);
	    }
	    type = yyvsp[0].lrvalue.type;
	    yyval.lrvalue.type = type;
	    if (exact_types && !BASIC_TYPE(type, TYPE_NUMBER) &&
		type != TYPE_FLOAT )
		type_error("Bad argument to unary '-'", yyvsp[0].lrvalue.type);
	}
break;
case 158:
#line 3253 "lang.y"
{
	    if (yyvsp[-1].lvalue.length) {
		add_to_mem_block(A_PROGRAM, yyvsp[-1].lvalue.u.p, yyvsp[-1].lvalue.length);
		yfree(yyvsp[-1].lvalue.u.p);
		last_expression = CURRENT_PROGRAM_SIZE;
		ins_f_byte(F_POST_INC);
	    } else {
		PREPARE_INSERT(3)
		char *source;

		CURRENT_PROGRAM_SIZE =
		  (last_expression = CURRENT_PROGRAM_SIZE+2) + 1;
		source = yyvsp[-1].lvalue.u.simple;
		add_byte(*source++);
		add_byte(*source);
		add_f_byte(F_POST_INC);
	    }
	    if (exact_types && !BASIC_TYPE(yyvsp[-1].lvalue.type, TYPE_NUMBER))
		type_error("Bad argument to ++", yyvsp[-1].lvalue.type);
	    yyval.lrvalue.type = TYPE_NUMBER;
	}
break;
case 159:
#line 3275 "lang.y"
{
	    if (yyvsp[-1].lvalue.length) {
		add_to_mem_block(A_PROGRAM, yyvsp[-1].lvalue.u.p, yyvsp[-1].lvalue.length+1);
		yfree(yyvsp[-1].lvalue.u.p);
		mem_block[A_PROGRAM].block[
		  last_expression = CURRENT_PROGRAM_SIZE-1
		] = F_POST_DEC - F_OFFSET;
	    } else {
		PREPARE_INSERT(3)
		char *source;

		CURRENT_PROGRAM_SIZE =
		  (last_expression = CURRENT_PROGRAM_SIZE+2) + 1;
		source = yyvsp[-1].lvalue.u.simple;
		add_byte(*source++);
		add_byte(*source);
		add_f_byte(F_POST_DEC);
	    }
	    if (exact_types && !BASIC_TYPE(yyvsp[-1].lvalue.type, TYPE_NUMBER))
		type_error("Bad argument to --", yyvsp[-1].lvalue.type);
	    yyval.lrvalue.type = TYPE_NUMBER;
	}
break;
case 161:
#line 3300 "lang.y"
{ yyval.lrvalue.start = CURRENT_PROGRAM_SIZE; }
break;
case 163:
#line 3304 "lang.y"
{
	    int string_number;
	    PREPARE_S_INSERT(3)
	    char *p;
#line 3181 "prolang.y"
	    p = last_lex_string;
	    last_lex_string = 0;
	    yyval.lrvalue.start = last_expression = CURRENT_PROGRAM_SIZE;
	    yyval.lrvalue.type = TYPE_STRING;
	    yyval.lrvalue.code = -1;
	    string_number = store_prog_string(p);
	    if ( string_number <= 0xff ) {
	        add_f_byte(F_CSTRING0);
	        add_byte(string_number);
	    } else if ( string_number <= 0x1ff ) {
	        add_f_byte(F_CSTRING1);
	        add_byte(string_number);
	    } else if ( string_number <= 0x2ff ) {
	        add_f_byte(F_CSTRING2);
	        add_byte(string_number);
	    } else if ( string_number <= 0x3ff ) {
	        add_f_byte(F_CSTRING3);
	        add_byte(string_number);
	    } else {
	        add_f_byte(F_STRING);
	        add_short(string_number);
	        CURRENT_PROGRAM_SIZE++;
	    }
	    CURRENT_PROGRAM_SIZE += 2;
	}
break;
case 164:
#line 3335 "lang.y"
{
	    int current;
	    int number;
	    PREPARE_INSERT(5)
#line 3212 "prolang.y"
	    yyval.lrvalue.start = last_expression = current = CURRENT_PROGRAM_SIZE;
	    yyval.lrvalue.code = -1;
	    number = yyvsp[0].number;
	    if ( number == 0 ) {
		current++;
		add_f_byte(F_CONST0);
		yyval.lrvalue.type = TYPE_ANY;
	    } else if ( number == 1 ) {
		add_f_byte(F_CONST1);
		current++;
		yyval.lrvalue.type = TYPE_NUMBER;
	    } else if ( number >= 0 && number <= 0xff ) {
		add_f_byte(F_CLIT);
		add_byte(number);
		current += 2;
		yyval.lrvalue.type = TYPE_NUMBER;
	    } else {
		char * source = (char*)&yyvsp[0].number;

		add_f_byte(F_NUMBER);
		add_byte(*source++);
		add_byte(*source++);
		add_byte(*source++);
		add_byte(*source);
		current += 5;
		yyval.lrvalue.type = TYPE_NUMBER;
	    }
	    CURRENT_PROGRAM_SIZE = current;
	}
break;
case 165:
#line 3370 "lang.y"
{
	    int ix;

	    yyval.lrvalue.start = CURRENT_PROGRAM_SIZE;
	    yyval.lrvalue.code = -1;
	    ix = yyvsp[0].closure.number;
	    ins_f_byte(F_CLOSURE);
	    ins_short(ix);
	    yyval.lrvalue.type = TYPE_CLOSURE;
	}
break;
case 166:
#line 3381 "lang.y"
{
	    int string_number;
	    int quotes;

	    yyval.lrvalue.start = CURRENT_PROGRAM_SIZE;
	    yyval.lrvalue.code = -1;
	    quotes = yyvsp[0].symbol.quotes;
	    string_number = store_prog_string(yyvsp[0].symbol.name);
	    if (quotes == 1 && string_number < 0x100) {
		ins_f_byte(F_CSTRING0);
		ins_byte(string_number);
		ins_f_byte(F_QUOTE);
	    } else {
		ins_f_byte(F_SYMBOL);
		ins_short(string_number);
		ins_byte(quotes);
	    }
	    yyval.lrvalue.type = TYPE_SYMBOL;
	}
break;
case 167:
#line 3402 "lang.y"
{
	    int exponent;

	    yyval.lrvalue.start = CURRENT_PROGRAM_SIZE;
	    yyval.lrvalue.code = -1;
	    ins_f_byte(F_FLOAT);
	    ins_long ( SPLIT_DOUBLE( yyvsp[0].float_number, &exponent) );
	    ins_short( exponent );
	    yyval.lrvalue.type = TYPE_FLOAT;
	}
break;
case 168:
#line 3414 "lang.y"
{
	    yyval.lrvalue.type = yyvsp[-1].lrvalue.type;
	    yyval.lrvalue.start = yyvsp[-2].lrvalue.start;
	    yyval.lrvalue.code = -1;
	}
break;
case 171:
#line 3425 "lang.y"
{
	    check_aggregate_types(yyvsp[-2].number);	/* We don't care about these types,
					 * unless a reference appears */
	    ins_f_byte(F_AGGREGATE);
	    ins_short(yyvsp[-2].number);
	    if (yyvsp[-2].number > MAX_ARRAY_SIZE)
		yyerror("Illegal array size");
	    yyval.lrvalue.type = TYPE_MOD_POINTER | TYPE_ANY;
	    yyval.lrvalue.start = yyvsp[-3].lrvalue.start;
	    yyval.lrvalue.code = -1;
	}
break;
case 172:
#line 3437 "lang.y"
{
	    int quotes;

	    check_aggregate_types(yyvsp[-2].number);	/* We don't care about these types,
					 * unless a reference appears */
	    ins_f_byte(F_AGGREGATE);
	    ins_short(yyvsp[-2].number);
	    if (yyvsp[-2].number > MAX_ARRAY_SIZE)
		yyerror("Illegal array size");
	    yyval.lrvalue.type = TYPE_QUOTED_ARRAY;
	    yyval.lrvalue.start = yyvsp[-3].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    quotes = yyvsp[-4].number;
	    do {
		ins_f_byte(F_QUOTE);
	    } while (--quotes);
	}
break;
case 173:
#line 3456 "lang.y"
{
	    mp_int num_keys;

	    check_aggregate_types(yyvsp[-2].numbers[0]);
	    num_keys = yyvsp[-2].numbers[0] / (yyvsp[-2].numbers[1]+1);
	    if ( (num_keys | yyvsp[-2].numbers[1]) &~0xff) {
		ins_f_byte(F_M_AGGREGATE);
		ins_short(num_keys);
		ins_short(yyvsp[-2].numbers[1]);
	    } else {
		ins_f_byte(F_M_CAGGREGATE);
		ins_byte(num_keys);
		ins_byte(yyvsp[-2].numbers[1]);
	    }
	    yyval.lrvalue.type = TYPE_MAPPING;
	    yyval.lrvalue.start = yyvsp[-3].lrvalue.start;
	    yyval.lrvalue.code = -1;
	}
break;
case 174:
#line 3476 "lang.y"
{
#line 3350 "prolang.y"
	    yyval.lrvalue.start = yyvsp[-5].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    ins_f_byte(F_RANGE);
	    if (exact_types) {
		int type;

		yyval.lrvalue.type = type = yyvsp[-5].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		    yyval.lrvalue.type = TYPE_ANY;
		}
		type = yyvsp[-3].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = yyvsp[-1].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    } else {
		yyval.lrvalue.type = TYPE_ANY;
	    }
	}
break;
case 175:
#line 3502 "lang.y"
{
#line 3376 "prolang.y"
	    yyval.lrvalue.start = yyvsp[-6].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    ins_f_code(F_NR_RANGE);
	    if (exact_types) {
		int type;

		yyval.lrvalue.type = type = yyvsp[-6].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		    yyval.lrvalue.type = TYPE_ANY;
		}
		type = yyvsp[-4].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = yyvsp[-1].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    } else {
		yyval.lrvalue.type = TYPE_ANY;
	    }
	}
break;
case 176:
#line 3528 "lang.y"
{
#line 3402 "prolang.y"
	    yyval.lrvalue.start = yyvsp[-6].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    ins_f_code(F_RN_RANGE);
	    if (exact_types) {
		int type;

		yyval.lrvalue.type = type = yyvsp[-6].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		    yyval.lrvalue.type = TYPE_ANY;
		}
		type = yyvsp[-3].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = yyvsp[-1].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    } else {
		yyval.lrvalue.type = TYPE_ANY;
	    }
	}
break;
case 177:
#line 3554 "lang.y"
{
#line 3428 "prolang.y"
	    yyval.lrvalue.start = yyvsp[-7].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    ins_f_code(F_RR_RANGE);
	    if (exact_types) {
		int type;

		yyval.lrvalue.type = type = yyvsp[-7].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		    yyval.lrvalue.type = TYPE_ANY;
		}
		type = yyvsp[-4].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = yyvsp[-1].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    } else {
		yyval.lrvalue.type = TYPE_ANY;
	    }
	}
break;
case 178:
#line 3580 "lang.y"
{
#line 3454 "prolang.y"
	    yyval.lrvalue.start = yyvsp[-4].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    ins_f_byte(F_EXTRACT2);
	    if (exact_types) {
		int type;

		yyval.lrvalue.type = type = yyvsp[-4].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		    yyval.lrvalue.type = TYPE_ANY;
		}
		type = yyvsp[-2].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    } else {
		yyval.lrvalue.type = TYPE_ANY;
	    }
	}
break;
case 179:
#line 3603 "lang.y"
{
#line 3477 "prolang.y"
	    yyval.lrvalue.start = yyvsp[-5].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    if (CURRENT_PROGRAM_SIZE - last_expression == 2 &&
		mem_block[A_PROGRAM].block[last_expression] ==
		  F_CLIT - F_OFFSET )
	    {
		mem_block[A_PROGRAM].block[last_expression] =
		  F_NCLIT - F_OFFSET;
	    } else {
		ins_f_byte(F_NEGATE);
	    }
	    ins_f_byte(F_EXTRACT2);
	    if (exact_types) {
		int type;

		yyval.lrvalue.type = type = yyvsp[-5].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		    yyval.lrvalue.type = TYPE_ANY;
		}
		type = yyvsp[-2].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    } else {
		yyval.lrvalue.type = TYPE_ANY;
	    }
	}
break;
case 180:
#line 3635 "lang.y"
{
	    int i;
	    int current;
	    char *p;
#line 3512 "prolang.y"
	    i = verify_declared(yyvsp[0].ident);
	    yyval.lrvalue.start = current = CURRENT_PROGRAM_SIZE;
	    yyval.lrvalue.code = -1;
	    if (current + 3 > mem_block[A_PROGRAM].max_size)
	        realloc_a_program();
	    p = mem_block[A_PROGRAM].block + current;
	    if (i & VIRTUAL_VAR_TAG) {
		*p++ = F_PUSH_VIRTUAL_VARIABLE_LVALUE - F_OFFSET;
		*p = i;
	    } else {
		if (i + num_virtual_variables & ~0xff) {
		    *p = F_PUSH_IDENTIFIER16_LVALUE - F_OFFSET;
		    upd_short(++current, i + num_virtual_variables);
		} else {
		    *p++ = F_PUSH_IDENTIFIER_LVALUE - F_OFFSET;
		    *p = i + num_virtual_variables;
		}
	    }
	    CURRENT_PROGRAM_SIZE = current + 2;
	    if (i == -1)
		yyval.lrvalue.type = TYPE_ANY | TYPE_MOD_REFERENCE;
	    else
		yyval.lrvalue.type = (VARIABLE(i)->flags & TYPE_MOD_MASK) |
		  TYPE_MOD_REFERENCE;
	}
break;
case 181:
#line 3666 "lang.y"
{
	    int current;
	    char *p;
#line 3542 "prolang.y"
	    yyval.lrvalue.start = current = CURRENT_PROGRAM_SIZE;
	    yyval.lrvalue.code = -1;
	    if (current + 2 > mem_block[A_PROGRAM].max_size)
	        realloc_a_program();
	    p = mem_block[A_PROGRAM].block + current;
	    *p++ = F_PUSH_LOCAL_VARIABLE_LVALUE - F_OFFSET;
	    *p = yyvsp[0].number;
	    CURRENT_PROGRAM_SIZE = current + 2;
	    yyval.lrvalue.type = type_of_locals[yyvsp[0].number] | TYPE_MOD_REFERENCE;
	}
break;
case 182:
#line 3681 "lang.y"
{
#line 3555 "prolang.y"
	    /* the ',' operator is reserved for indexing on multi-valued
	     * mappings and other multi-dimensional data
	     */
	    arrange_protected_lvalue(yyvsp[-4].lrvalue.start, yyvsp[-4].lrvalue.code, yyvsp[-4].lrvalue.end,
	      F_PROTECTED_INDEX_LVALUE - F_OFFSET
	    );
	    yyval.lrvalue.start = yyvsp[-4].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    if (yyvsp[-2].lrvalue.type & TYPE_MOD_REFERENCE)
		yyerror("Reference used as index");
	    if (!exact_types) {
		yyval.lrvalue.type = TYPE_ANY | TYPE_MOD_REFERENCE;
	    } else {
		int type;

		type = yyvsp[-4].lrvalue.type;
		if (type & TYPE_MOD_POINTER) {
		    yyval.lrvalue.type = type & ~TYPE_MOD_POINTER;
		} else switch (type) {
		  default:
		    type_error("Bad type to indexed reference", type);
		  case TYPE_ANY:
#ifdef MAPPINGS
		  case TYPE_MAPPING:
		    yyvsp[-2].lrvalue.type = TYPE_ANY;
#endif
		    yyval.lrvalue.type = TYPE_ANY | TYPE_MOD_REFERENCE;
		    break;
		  case TYPE_STRING:
		    yyval.lrvalue.type = TYPE_NUMBER | TYPE_MOD_REFERENCE;
		    break;
		}
		if (!BASIC_TYPE(yyvsp[-2].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad type of index", yyvsp[-2].lrvalue.type);
	    }
	}
break;
case 183:
#line 3720 "lang.y"
{
#line 3594 "prolang.y"
	    arrange_protected_lvalue(yyvsp[-5].lrvalue.start, yyvsp[-5].lrvalue.code, yyvsp[-5].lrvalue.end,
	      F_PROTECTED_RINDEX_LVALUE - F_OFFSET
	    );
	    yyval.lrvalue.start = yyvsp[-5].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    if (!exact_types) {
		yyval.lrvalue.type = TYPE_ANY | TYPE_MOD_REFERENCE;
	    } else {
		int type;

		type = yyvsp[-5].lrvalue.type;
		if (type & TYPE_MOD_POINTER) {
		    yyval.lrvalue.type = type & ~TYPE_MOD_POINTER;
		} else switch (type) {
		  default:
		    type_error("Bad type to indexed reference", type);
		  case TYPE_ANY:
		    yyval.lrvalue.type = TYPE_ANY | TYPE_MOD_REFERENCE;
		    break;
		  case TYPE_STRING:
		    yyval.lrvalue.type = TYPE_NUMBER | TYPE_MOD_REFERENCE;
		    break;
		}
		if (!BASIC_TYPE(yyvsp[-2].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad type of index", yyvsp[-2].lrvalue.type);
	    }
	}
break;
case 184:
#line 3751 "lang.y"
{
#line 3625 "prolang.y"
	    yyval.lrvalue.start = yyvsp[-6].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    yyval.lrvalue.type = TYPE_ANY | TYPE_MOD_REFERENCE;
	    ins_f_code(F_PUSH_PROTECTED_INDEXED_MAP_LVALUE);
	    if (yyvsp[-4].lrvalue.type & TYPE_MOD_REFERENCE)
		yyerror("Reference used as index");
	    if (exact_types) {
		int type;

		type = yyvsp[-6].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_MAPPING)
		{
		    type_error("Bad type to indexed value", type);
		}
		type = yyvsp[-2].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 185:
#line 3774 "lang.y"
{
#line 3648 "prolang.y"
	    arrange_protected_lvalue(yyvsp[-6].lrvalue.start, yyvsp[-6].lrvalue.code, yyvsp[-6].lrvalue.end,
	      F_PROTECTED_RANGE_LVALUE - F_OFFSET
	    );
	    yyval.lrvalue.start = yyvsp[-6].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    if (!exact_types) {
		yyval.lrvalue.type = TYPE_ANY | TYPE_MOD_REFERENCE;
	    } else {
		int type;

		yyval.lrvalue.type = type = yyvsp[-6].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		    yyval.lrvalue.type = TYPE_ANY;
		}
		type = yyvsp[-4].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = yyvsp[-2].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 186:
#line 3802 "lang.y"
{
#line 3676 "prolang.y"
	    arrange_protected_lvalue(yyvsp[-7].lrvalue.start, yyvsp[-7].lrvalue.code, yyvsp[-7].lrvalue.end,
	      F_PROTECTED_NR_RANGE_LVALUE - F_OFFSET
	    );
	    yyval.lrvalue.start = yyvsp[-7].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    if (!exact_types) {
		yyval.lrvalue.type = TYPE_ANY | TYPE_MOD_REFERENCE;
	    } else {
		int type;

		yyval.lrvalue.type = type = yyvsp[-7].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		    yyval.lrvalue.type = TYPE_ANY;
		}
		type = yyvsp[-5].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = yyvsp[-2].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 187:
#line 3830 "lang.y"
{
#line 3704 "prolang.y"
	    arrange_protected_lvalue(yyvsp[-7].lrvalue.start, yyvsp[-7].lrvalue.code, yyvsp[-7].lrvalue.end,
	      F_PROTECTED_RN_RANGE_LVALUE - F_OFFSET
	    );
	    yyval.lrvalue.start = yyvsp[-7].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    if (!exact_types) {
		yyval.lrvalue.type = TYPE_ANY | TYPE_MOD_REFERENCE;
	    } else {
		int type;

		yyval.lrvalue.type = type = yyvsp[-7].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		    yyval.lrvalue.type = TYPE_ANY;
		}
		type = yyvsp[-4].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = yyvsp[-2].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 188:
#line 3858 "lang.y"
{
#line 3732 "prolang.y"
	    arrange_protected_lvalue(yyvsp[-8].lrvalue.start, yyvsp[-8].lrvalue.code, yyvsp[-8].lrvalue.end,
	      F_PROTECTED_RR_RANGE_LVALUE - F_OFFSET
	    );
	    yyval.lrvalue.start = yyvsp[-8].lrvalue.start;
	    yyval.lrvalue.code = -1;
	    if (!exact_types) {
		yyval.lrvalue.type = TYPE_ANY | TYPE_MOD_REFERENCE;
	    } else {
		int type;

		yyval.lrvalue.type = type = yyvsp[-8].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		    yyval.lrvalue.type = TYPE_ANY;
		}
		type = yyvsp[-5].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = yyvsp[-2].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 189:
#line 3886 "lang.y"
{
	    int i;
	    int current;
	    char *p;
#line 3764 "prolang.y"
	    i = verify_declared(yyvsp[0].ident);
	    yyval.lrvalue.start = current = CURRENT_PROGRAM_SIZE;
	    yyval.lrvalue.end = 0;
	    if (current + 3 > mem_block[A_PROGRAM].max_size)
	        realloc_a_program();
	    p = mem_block[A_PROGRAM].block + current;
	    if (i & VIRTUAL_VAR_TAG) {
		yyval.lrvalue.code = F_PUSH_VIRTUAL_VARIABLE_LVALUE - F_OFFSET;
		*p++ = F_VIRTUAL_VARIABLE - F_OFFSET;
		*p = i;
		yyval.lrvalue.type = V_VARIABLE(i)->flags & TYPE_MOD_MASK;
	    } else {
		if (i + num_virtual_variables & ~0xff) {
		    yyval.lrvalue.code = F_PUSH_IDENTIFIER16_LVALUE - F_OFFSET;
		    *p = F_IDENTIFIER16 - F_OFFSET;
		    upd_short(++current, i + num_virtual_variables);
		    yyval.lrvalue.end = current+1;
		} else {
		    yyval.lrvalue.code = F_PUSH_IDENTIFIER_LVALUE - F_OFFSET;
		    *p++ = F_IDENTIFIER - F_OFFSET;
		    *p = i + num_virtual_variables;
		}
		yyval.lrvalue.type = NV_VARIABLE(i)->flags & TYPE_MOD_MASK;
	    }
	    CURRENT_PROGRAM_SIZE = current + 2;
	    if (i == -1)
		yyval.lrvalue.type = TYPE_ANY;
	}
break;
case 190:
#line 3920 "lang.y"
{
	    int current;
	    char *p;
#line 3797 "prolang.y"
	    yyval.lrvalue.start = current = CURRENT_PROGRAM_SIZE;
	    yyval.lrvalue.code = F_PUSH_LOCAL_VARIABLE_LVALUE - F_OFFSET;
	    yyval.lrvalue.end = 0;
	    if (current + 2 > mem_block[A_PROGRAM].max_size)
	        realloc_a_program();
	    p = mem_block[A_PROGRAM].block + current;
	    *p++ = F_LOCAL - F_OFFSET;
	    *p = yyvsp[0].number;
	    CURRENT_PROGRAM_SIZE = current + 2;
	    yyval.lrvalue.type = type_of_locals[yyvsp[0].number];
	}
break;
case 191:
#line 3936 "lang.y"
{
#line 3811 "prolang.y"
	    /* the ',' operator is reserved for indexing on multi-valued
	     * mappings and other multi-dimensional data
	     */
	    yyval.lrvalue.start = yyvsp[-3].lrvalue.start;
	    yyval.lrvalue.end = CURRENT_PROGRAM_SIZE;
	    yyval.lrvalue.code = F_PUSH_INDEXED_LVALUE - F_OFFSET;
	    ins_f_byte(F_INDEX);
	    if (yyvsp[-1].lrvalue.type & TYPE_MOD_REFERENCE)
		yyerror("Reference used as index");
	    if (exact_types) {
		int type;

		type = yyvsp[-3].lrvalue.type;
		if (type & TYPE_MOD_POINTER) {
		    yyval.lrvalue.type = type & ~TYPE_MOD_POINTER;
		} else switch (type) {
		  default:
		    type_error("Bad type to indexed value", type);
#ifdef MAPPINGS
		  case TYPE_MAPPING:
#endif
		  case TYPE_ANY:
		    yyvsp[-1].lrvalue.type = TYPE_ANY;
		    yyval.lrvalue.type = TYPE_ANY;
		    break;
		  case TYPE_STRING:
		    yyval.lrvalue.type = TYPE_NUMBER;
		    break;
		}
		if (!BASIC_TYPE(yyvsp[-1].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad type of index", yyvsp[-1].lrvalue.type);
	    }
	}
break;
case 192:
#line 3972 "lang.y"
{
#line 3847 "prolang.y"
	    yyval.lrvalue.start = yyvsp[-4].lrvalue.start;
	    yyval.lrvalue.end = CURRENT_PROGRAM_SIZE;
	    yyval.lrvalue.code = F_PUSH_RINDEXED_LVALUE - F_OFFSET;
	    ins_f_byte(F_RINDEX);
	    if (exact_types) {
		int type;

		type = yyvsp[-4].lrvalue.type;
		if (type & TYPE_MOD_POINTER) {
		    yyval.lrvalue.type = type & ~TYPE_MOD_POINTER;
		} else switch (type) {
		  default:
		    type_error("Bad type to indexed value", type);
		  case TYPE_ANY:
		    yyval.lrvalue.type = TYPE_ANY;
		    break;
		  case TYPE_STRING:
		    yyval.lrvalue.type = TYPE_NUMBER;
		    break;
		}
		if (!BASIC_TYPE(yyvsp[-1].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad type of index", yyvsp[-1].lrvalue.type);
	    }
	}
break;
case 193:
#line 4000 "lang.y"
{
#line 3875 "prolang.y"
	    yyval.lrvalue.start = yyvsp[-5].lrvalue.start;
	    yyval.lrvalue.end = CURRENT_PROGRAM_SIZE;
	    yyval.lrvalue.code = F_PUSH_INDEXED_MAP_LVALUE - F_OFFSET;
	    yyval.lrvalue.type = TYPE_ANY;
	    ins_f_byte(F_MAP_INDEX);
	    if (yyvsp[-3].lrvalue.type & TYPE_MOD_REFERENCE)
		yyerror("Reference used as index");
	    if (exact_types) {
		int type;

		type = yyvsp[-5].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_MAPPING)
		{
		    type_error("Bad type to indexed value", type);
		}
		type = yyvsp[-1].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 194:
#line 4025 "lang.y"
{ yyval.number = F_PRE_INC-F_OFFSET; }
break;
case 195:
#line 4026 "lang.y"
{ yyval.number = F_PRE_DEC-F_OFFSET; }
break;
case 196:
#line 4030 "lang.y"
{
	    if (exact_types &&
		!BASIC_TYPE(exact_types & TYPE_MOD_MASK, TYPE_VOID))
		type_error("Must return a value for a function declared",
			   exact_types);
	    ins_f_byte(F_RETURN0);
	}
break;
case 197:
#line 4038 "lang.y"
{
#line 3913 "prolang.y"
	    if (exact_types) {
		if (!MASKED_TYPE(yyvsp[0].lrvalue.type, exact_types & TYPE_MOD_MASK))
		    type_error("Return type not matching", exact_types);
		/* if (exact_types & ~TYPE_MOD_POINTER) == TYPE_ANY ,
		 * a reference in $2.type remains undetected.
		 */
	    }
	    if (yyvsp[0].lrvalue.type & TYPE_MOD_REFERENCE) {
		yyerror("May not return a reference");
	    }
	    if (last_expression == CURRENT_PROGRAM_SIZE - 1 &&
		mem_block[A_PROGRAM].block[last_expression] ==
		    F_CONST0 - F_OFFSET )
	    {
		mem_block[A_PROGRAM].block[last_expression] =
		      F_RETURN0 - F_OFFSET;
		last_expression = -1;
	    } else ins_f_byte(F_RETURN);
	}
break;
case 198:
#line 4060 "lang.y"
{ yyval.number = 0; }
break;
case 199:
#line 4061 "lang.y"
{ yyval.number = yyvsp[0].number; }
break;
case 200:
#line 4062 "lang.y"
{ yyval.number = yyvsp[-1].number; }
break;
case 201:
#line 4064 "lang.y"
{ yyval.number = 1; add_arg_type(yyvsp[0].lrvalue.type); }
break;
case 202:
#line 4065 "lang.y"
{ yyval.number = yyvsp[-2].number + 1; add_arg_type(yyvsp[0].lrvalue.type); }
break;
case 203:
#line 4067 "lang.y"
{ yyval.number = 0; }
break;
case 204:
#line 4068 "lang.y"
{ yyval.number = 1; add_arg_type(yyvsp[0].lrvalue.type); }
break;
case 205:
#line 4069 "lang.y"
{ yyval.number = yyvsp[-2].number + 1; add_arg_type(yyvsp[0].lrvalue.type); }
break;
case 206:
#line 4072 "lang.y"
{ yyval.numbers[0] = 0; yyval.numbers[1]= 1; }
break;
case 209:
#line 4075 "lang.y"
{ yyval.numbers[0] = yyvsp[0].number; yyval.numbers[1] = 0; }
break;
case 210:
#line 4076 "lang.y"
{ yyval.numbers[0] = yyvsp[-1].number; yyval.numbers[1] = 0; }
break;
case 211:
#line 4079 "lang.y"
{
	    yyval.numbers[0] = 1 + yyvsp[0].number;
	    yyval.numbers[1] = yyvsp[0].number;
	    add_arg_type(yyvsp[-1].lrvalue.type); /* order doesn't matter */
	}
break;
case 212:
#line 4085 "lang.y"
{
	    if (yyvsp[-3].numbers[1] != yyvsp[0].number) {
		yyerror("Inconsistent number of values in mapping literal");
	    }
	    yyval.numbers[0] = yyvsp[-3].numbers[0] + 1 + yyvsp[0].number;
	    yyval.numbers[1] = yyvsp[-3].numbers[1];
	    add_arg_type(yyvsp[-1].lrvalue.type);
	}
break;
case 213:
#line 4093 "lang.y"
{ yyval.number = 1; add_arg_type(yyvsp[0].lrvalue.type); }
break;
case 214:
#line 4094 "lang.y"
{ yyval.number = yyvsp[-2].number + 1; add_arg_type(yyvsp[0].lrvalue.type); }
break;
case 215:
#line 4328 "lang.y"
{
	    yyval.number = CURRENT_PROGRAM_SIZE;
	    ins_f_byte(F_CATCH);
	    ins_byte(0);
	}
break;
case 216:
#line 4334 "lang.y"
{
#line 4209 "prolang.y"
	    int start, offset;
#if 1
	    ins_f_code(F_END_CATCH);
#else
	    ins_f_byte(F_POP_VALUE);
	    ins_f_byte(F_RETURN);
#endif
	    start = yyvsp[-3].number;
	    offset = CURRENT_PROGRAM_SIZE - (start + 2);
	    if (offset >= 0x100) {
		/* should happen seldom, but better handle it without error...
		 */
		int i;
		char *p;

		if (CURRENT_PROGRAM_SIZE + 5 > mem_block[A_PROGRAM].max_size)
		    realloc_a_program();
		CURRENT_PROGRAM_SIZE += 5;
		p = mem_block[A_PROGRAM].block + CURRENT_PROGRAM_SIZE - 1;
		for( i = offset; --i >= 0; --p ) *p = p[-5];
		p[-5] = 2;
		p[-4] = F_BRANCH  - F_OFFSET;
		p[-3] = 3;
		p[-2] = F_LBRANCH - F_OFFSET;
		upd_short(start + 5, offset+2);
		if (offset > 0x7ffd)
		    yyerror("offset overflow");
	    } else {
		mem_block[A_PROGRAM].block[start+1] = offset;
	    }
	    yyval.lrvalue.start = start;
	    yyval.lrvalue.type  = TYPE_ANY;
	    yyval.lrvalue.code = -1;
	}
break;
case 217:
#line 4372 "lang.y"
{
	    ins_f_byte(F_SSCANF);
	    ins_byte(yyvsp[-1].number + 2);
	    yyval.lrvalue.start = yyvsp[-6].lrvalue.start;
	    yyval.lrvalue.type = TYPE_NUMBER;
	    yyval.lrvalue.code = -1;
	}
break;
case 218:
#line 4392 "lang.y"
{ yyval.number = 0; }
break;
case 219:
#line 4394 "lang.y"
{
	    int i;
#line 4270 "prolang.y"
	    yyval.number = 1 + yyvsp[-2].number;
	    i = verify_declared(yyvsp[0].ident);
	    if (i & VIRTUAL_VAR_TAG) {
		ins_f_byte(F_PUSH_VIRTUAL_VARIABLE_LVALUE);
		ins_byte(i);
	    } else {
		if (i + num_virtual_variables & ~0xff) {
		    ins_f_byte(F_PUSH_IDENTIFIER16_LVALUE);
		    ins_short(i + num_virtual_variables);
		} else {
		    ins_f_byte(F_PUSH_IDENTIFIER_LVALUE);
		    ins_byte(i + num_virtual_variables);
		}
	    }
	}
break;
case 220:
#line 4413 "lang.y"
{
#line 4288 "prolang.y"
	    yyval.number = 1 + yyvsp[-2].number;
	    ins_f_byte(F_PUSH_LOCAL_VARIABLE_LVALUE);
	    ins_byte(yyvsp[0].number);
	}
break;
case 221:
#line 4420 "lang.y"
{
#line 4295 "prolang.y"
	    yyval.number = 1 + yyvsp[-5].number;
	    arrange_protected_lvalue(yyvsp[-3].lrvalue.start, yyvsp[-3].lrvalue.code, yyvsp[-3].lrvalue.end,
	      F_PROTECTED_INDEX_LVALUE - F_OFFSET
	    );
	    if (yyvsp[-1].lrvalue.type & TYPE_MOD_REFERENCE)
		yyerror("Reference used as index");
	    if (exact_types) {
		int type;

		type = yyvsp[-3].lrvalue.type;
		if ( !(type & TYPE_MOD_POINTER) )
		 switch (type) {
		  default:
		    type_error("Bad type to indexed lvalue", type);
		  case TYPE_ANY:
#ifdef MAPPINGS
		  case TYPE_MAPPING:
		    yyvsp[-1].lrvalue.type = TYPE_ANY;
#endif
		    break;
		  case TYPE_STRING:
		    break;
		}
		if (!BASIC_TYPE(yyvsp[-1].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad type of index", yyvsp[-1].lrvalue.type);
	    }
	}
break;
case 222:
#line 4450 "lang.y"
{
#line 4325 "prolang.y"
	    yyval.number = 1 + yyvsp[-6].number;
	    arrange_protected_lvalue(yyvsp[-4].lrvalue.start, yyvsp[-4].lrvalue.code, yyvsp[-4].lrvalue.end,
	      F_PROTECTED_RINDEX_LVALUE - F_OFFSET
	    );
	    if (exact_types) {
		int type;

		type = yyvsp[-4].lrvalue.type;
		if ( !(type & TYPE_MOD_POINTER) )
		 switch (type) {
		  default:
		    type_error("Bad type to indexed lvalue", type);
		  case TYPE_ANY:
		    break;
		  case TYPE_STRING:
		    break;
		}
		if (!BASIC_TYPE(yyvsp[-1].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad type of index", yyvsp[-1].lrvalue.type);
	    }
	}
break;
case 223:
#line 4475 "lang.y"
{
#line 4350 "prolang.y"
	    yyval.number = 1 + yyvsp[-7].number;
	    ins_f_code(F_PUSH_PROTECTED_INDEXED_MAP_LVALUE);
	    if (yyvsp[-3].lrvalue.type & TYPE_MOD_REFERENCE)
		yyerror("Reference used as index");
	    if (exact_types) {
		int type;

		type = yyvsp[-5].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_MAPPING)
		{
		    type_error("Bad type to indexed value", type);
		}
		type = yyvsp[-1].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 224:
#line 4496 "lang.y"
{
#line 4371 "prolang.y"
	    yyval.number = 1 + yyvsp[-6].number;
	    arrange_protected_lvalue(yyvsp[-4].lrvalue.start, yyvsp[-4].lrvalue.code, yyvsp[-4].lrvalue.end,
	      F_PROTECTED_EXTRACT_LVALUE - F_OFFSET
	    );
	    if (exact_types) {
		int type;

		type = yyvsp[-4].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		}
		type = yyvsp[-2].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 225:
#line 4517 "lang.y"
{
#line 4392 "prolang.y"
	    yyval.number = 1 + yyvsp[-7].number;
	    arrange_protected_lvalue(yyvsp[-5].lrvalue.start, yyvsp[-5].lrvalue.code, yyvsp[-5].lrvalue.end,
	      F_PROTECTED_RANGE_LVALUE - F_OFFSET
	    );
	    if (exact_types) {
		int type;

		type = yyvsp[-5].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		}
		type = yyvsp[-3].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = yyvsp[-1].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 226:
#line 4541 "lang.y"
{
#line 4416 "prolang.y"
	    yyval.number = 1 + yyvsp[-8].number;
	    arrange_protected_lvalue(yyvsp[-6].lrvalue.start, yyvsp[-6].lrvalue.code, yyvsp[-6].lrvalue.end,
	      F_PROTECTED_NR_RANGE_LVALUE - F_OFFSET
	    );
	    if (exact_types) {
		int type;

		type = yyvsp[-6].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		}
		type = yyvsp[-4].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = yyvsp[-1].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 227:
#line 4565 "lang.y"
{
#line 4440 "prolang.y"
	    yyval.number = 1 + yyvsp[-8].number;
	    arrange_protected_lvalue(yyvsp[-6].lrvalue.start, yyvsp[-6].lrvalue.code, yyvsp[-6].lrvalue.end,
	      F_PROTECTED_RN_RANGE_LVALUE - F_OFFSET
	    );
	    if (exact_types) {
		int type;

		type = yyvsp[-6].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		}
		type = yyvsp[-3].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = yyvsp[-1].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 228:
#line 4589 "lang.y"
{
#line 4464 "prolang.y"
	    yyval.number = 1 + yyvsp[-9].number;
	    arrange_protected_lvalue(yyvsp[-7].lrvalue.start, yyvsp[-7].lrvalue.code, yyvsp[-7].lrvalue.end,
	      F_PROTECTED_RR_RANGE_LVALUE - F_OFFSET
	    );
	    if (exact_types) {
		int type;

		type = yyvsp[-7].lrvalue.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		}
		type = yyvsp[-4].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = yyvsp[-1].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 229:
#line 4614 "lang.y"
{
	    int i;
#line 4490 "prolang.y"
	    yyval.lvalue.length = 0;
	    i = verify_declared(yyvsp[0].ident);
	    if (i & VIRTUAL_VAR_TAG) {
		yyval.lvalue.u.simple[0] = F_PUSH_VIRTUAL_VARIABLE_LVALUE - F_OFFSET;
		yyval.lvalue.u.simple[1] = i;
		yyval.lvalue.type = V_VARIABLE(i)->flags & TYPE_MOD_MASK;
		if (i == -1)
		    yyval.lvalue.type = TYPE_ANY;
	    } else {
		if (i + num_virtual_variables & ~0xff) {
		    char *q;
		    short var_index[2];

		    q = yalloc(4); /* assign uses an extra byte */
		    yyval.lvalue.length = 3;
		    yyval.lvalue.u.p = q;
		    q[0] = F_PUSH_IDENTIFIER16_LVALUE - F_OFFSET;
		    var_index[0] = i + num_virtual_variables;
		    memcpy(q+1, (char*)var_index, 2);
		    yyval.lvalue.type = NV_VARIABLE(i)->flags & TYPE_MOD_MASK;
		} else {
		    yyval.lvalue.u.simple[0] = F_PUSH_IDENTIFIER_LVALUE - F_OFFSET;
		    yyval.lvalue.u.simple[1] = i + num_virtual_variables;
		}
		yyval.lvalue.type = NV_VARIABLE(i)->flags & TYPE_MOD_MASK;
	    }
	}
break;
case 230:
#line 4645 "lang.y"
{
#line 4520 "prolang.y"
	    yyval.lvalue.u.simple[0] = F_PUSH_LOCAL_VARIABLE_LVALUE - F_OFFSET;
	    yyval.lvalue.u.simple[1] = yyvsp[0].number;
	    yyval.lvalue.length = 0;
	    yyval.lvalue.type = type_of_locals[yyvsp[0].number];
	}
break;
case 231:
#line 4653 "lang.y"
{
	    char *p, *q;
	    int start, current;
#line 4532 "prolang.y"
	    start = yyvsp[-3].lrvalue.start;
	    current = CURRENT_PROGRAM_SIZE;
	    p = mem_block[A_PROGRAM].block;
	    q = yalloc(current-start+2); /* assign uses an extra byte */
	    if (yyvsp[-3].lrvalue.code >= 0) {
		int end, start2;

		if (end = yyvsp[-3].lrvalue.end) {
		    start2 = end+1;
		    if (yyvsp[-3].lrvalue.code == F_PUSH_IDENTIFIER16_LVALUE - F_OFFSET)
			p[start] = yyvsp[-3].lrvalue.code;
		    else
			p[end] = yyvsp[-3].lrvalue.code;
		    memcpy(q, p + start2, current - start2);
		    memcpy(q + current - start2, p + start, start2 - start);
		    q[current - start] = F_INDEX_LVALUE - F_OFFSET;
		} else {
		    char c;

		    start2 = start + 2;
		    c = p[start+1];
		    memcpy(q, p + start2, current - start2);
		    p = q + current - start2;
		    *p++ = yyvsp[-3].lrvalue.code;
		    *p++ = c;
		    *p = F_INDEX_LVALUE - F_OFFSET;
		}
	    } else {
		memcpy(q, p + start, current - start);
		q[current - start] = F_PUSH_INDEXED_LVALUE - F_OFFSET;
	    }
	    yyval.lvalue.length = current + 1 - start;
	    yyval.lvalue.u.p = q;
	    CURRENT_PROGRAM_SIZE = start;
	    last_expression = -1;
	    if (yyvsp[-1].lrvalue.type & TYPE_MOD_REFERENCE)
		yyerror("Reference used as index");
	    if (exact_types) {
		int type;

		type = yyvsp[-3].lrvalue.type;
		if (type & TYPE_MOD_POINTER) {
		    yyval.lvalue.type = type & ~TYPE_MOD_POINTER;
		} else switch (type) {
		  default:
		    type_error("Bad type to indexed lvalue", type);
#ifdef MAPPINGS
		  case TYPE_MAPPING:
#endif
		  case TYPE_ANY:
		    yyvsp[-1].lrvalue.type = TYPE_ANY;
		    yyval.lvalue.type = TYPE_ANY;
		    break;
		  case TYPE_STRING:
		    yyval.lvalue.type = TYPE_NUMBER;
		    break;
		}
		if (!BASIC_TYPE(yyvsp[-1].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad type of index", yyvsp[-1].lrvalue.type);
	    } else {
		yyval.lvalue.type = TYPE_ANY;
	    }
	}
break;
case 232:
#line 4721 "lang.y"
{
	    char *p, *q;
	    int start, current;
#line 4600 "prolang.y"
	    start = yyvsp[-4].lrvalue.start;
	    current = CURRENT_PROGRAM_SIZE;
	    p = mem_block[A_PROGRAM].block;
	    q = yalloc(current-start+2); /* assign uses an extra byte */
	    if (yyvsp[-4].lrvalue.code >= 0) {
		int end, start2;

		if (end = yyvsp[-4].lrvalue.end) {
		    start2 = end+1;
		    if (yyvsp[-4].lrvalue.code == F_PUSH_IDENTIFIER16_LVALUE - F_OFFSET)
			p[start] = yyvsp[-4].lrvalue.code;
		    else
			p[end] = yyvsp[-4].lrvalue.code;
		    memcpy(q, p + start2, current - start2);
		    memcpy(q + current - start2, p + start, start2 - start);
		    q[current - start] = F_RINDEX_LVALUE - F_OFFSET;
		} else {
		    char c;

		    start2 = start + 2;
		    c = p[start+1];
		    memcpy(q, p + start2, current - start2);
		    p = q + current - start2;
		    *p++ = yyvsp[-4].lrvalue.code;
		    *p++ = c;
		    *p = F_RINDEX_LVALUE - F_OFFSET;
		}
	    } else {
		memcpy(q, p + start, current - start);
		q[current - start] = F_PUSH_RINDEXED_LVALUE - F_OFFSET;
	    }
	    yyval.lvalue.length = current + 1 - start;
	    yyval.lvalue.u.p = q;
	    CURRENT_PROGRAM_SIZE = start;
	    last_expression = -1;
	    if (exact_types) {
		int type;

		type = yyvsp[-4].lrvalue.type;
		if (type & TYPE_MOD_POINTER) {
		    yyval.lvalue.type = type & ~TYPE_MOD_POINTER;
		} else switch (type) {
		  default:
		    type_error("Bad type to indexed lvalue", type);
		  case TYPE_ANY:
		    yyval.lvalue.type = TYPE_ANY;
		    break;
		  case TYPE_STRING:
		    yyval.lvalue.type = TYPE_NUMBER;
		    break;
		}
		if (!BASIC_TYPE(yyvsp[-1].lrvalue.type, TYPE_NUMBER))
		    type_error("Bad type of index", yyvsp[-1].lrvalue.type);
	    } else {
		yyval.lvalue.type = TYPE_ANY;
	    }
	}
break;
case 233:
#line 4784 "lang.y"
{
	    char *p, *q;
	    int start, current;
#line 4663 "prolang.y"
	    start = yyvsp[-5].lrvalue.start;
	    current = CURRENT_PROGRAM_SIZE;
	    p = mem_block[A_PROGRAM].block;
	    q = yalloc(current-start+2); /* assign uses an extra byte */
	    memcpy(q, p + start, current - start);
	    q[current - start] = F_PUSH_INDEXED_MAP_LVALUE - F_OFFSET;
	    yyval.lvalue.length = current + 1 - start;
	    yyval.lvalue.u.p = q;
	    yyval.lvalue.type = TYPE_ANY;
	    CURRENT_PROGRAM_SIZE = start;
	    last_expression = -1;

	    if (yyvsp[-3].lrvalue.type & TYPE_MOD_REFERENCE)
		yyerror("Reference used as index");
	    if (exact_types) {
		int type;

		type = yyvsp[-5].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_MAPPING)
		{
		    type_error("Bad type to indexed value", type);
		}
		type = yyvsp[-1].lrvalue.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 234:
#line 4817 "lang.y"
{
#line 4694 "prolang.y"
	    indexing_argument = yyvsp[-4].lrvalue;
	    indexing_index1 = yyvsp[-2].lrvalue;
	    indexing_index2.type = TYPE_ANY;
	    indexing_code = F_EXTRACT_LVALUE - F_OFFSET;
	    goto range_lvalue_indexing;
	}
break;
case 235:
#line 4826 "lang.y"
{
#line 4703 "prolang.y"
	    if (CURRENT_PROGRAM_SIZE - last_expression == 2 &&
		mem_block[A_PROGRAM].block[last_expression] ==
		  F_CLIT - F_OFFSET )
	    {
		mem_block[A_PROGRAM].block[last_expression] =
		  F_NCLIT - F_OFFSET;
	    } else {
		ins_f_byte(F_NEGATE);
	    }
	    indexing_argument = yyvsp[-5].lrvalue;
	    indexing_index1 = yyvsp[-2].lrvalue;
	    indexing_index2.type = TYPE_ANY;
	    indexing_code = F_EXTRACT_LVALUE - F_OFFSET;
	    goto range_lvalue_indexing;
	}
break;
case 236:
#line 4844 "lang.y"
{
	    char *p, *q;
	    int start, current;
#line 4723 "prolang.y"
	    indexing_argument = yyvsp[-5].lrvalue;
	    indexing_index1 = yyvsp[-3].lrvalue;
	    indexing_index2 = yyvsp[-1].lrvalue;
	    indexing_code = F_RANGE_LVALUE - F_OFFSET;
range_lvalue_indexing:
	    start = indexing_argument.start;
	    current = CURRENT_PROGRAM_SIZE;
	    p = mem_block[A_PROGRAM].block;
	    q = yalloc(current-start+3);
	    if (indexing_argument.code < 0) {
		yyerror("Need lvalue for range lvalue.");
	    } else {
		int end, start2;

		if (end = indexing_argument.end) {
		    start2 = end+1;
		    if (indexing_argument.code ==
			F_PUSH_IDENTIFIER16_LVALUE - F_OFFSET)
		    {
			p[start] = indexing_argument.code;
		    } else {
			p[end] = indexing_argument.code;
		    }
		} else {
		    start2 = start+2;
		    p[start] = indexing_argument.code;
		}
		memcpy(q, p + start2, current - start2);
		memcpy(q + current - start2, p + start, start2 - start);
		current -= start;
		if (indexing_code > 0xff) {
		    q[current++] = indexing_code >> F_ESCAPE_BITS;
		}
		q[current] = indexing_code;
	    }
	    yyval.lvalue.length = current + 1;
	    yyval.lvalue.u.p = q;
	    CURRENT_PROGRAM_SIZE = start;
	    last_expression = -1;
	    if (exact_types) {
		int type;

		yyval.lvalue.type = type = indexing_argument.type;
		if ((type & TYPE_MOD_POINTER) == 0 &&
		    type != TYPE_ANY && type != TYPE_STRING)
		{
		    type_error("Bad type of argument used for range", type);
		    yyval.lvalue.type = TYPE_ANY;
		}
		type = indexing_index1.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
		type = indexing_index2.type;
		if (type != TYPE_ANY && type != TYPE_NUMBER)
		    type_error("Bad type of index", type);
	    }
	}
break;
case 237:
#line 4906 "lang.y"
{
#line 4783 "prolang.y"
	    indexing_argument = yyvsp[-6].lrvalue;
	    indexing_index1 = yyvsp[-4].lrvalue;
	    indexing_index2 = yyvsp[-1].lrvalue;
	    indexing_code = F_NR_RANGE_LVALUE - F_OFFSET;
	    goto range_lvalue_indexing;
	}
break;
case 238:
#line 4915 "lang.y"
{
#line 4792 "prolang.y"
	    indexing_argument = yyvsp[-6].lrvalue;
	    indexing_index1 = yyvsp[-3].lrvalue;
	    indexing_index2 = yyvsp[-1].lrvalue;
	    indexing_code = F_RN_RANGE_LVALUE - F_OFFSET;
	    goto range_lvalue_indexing;
	}
break;
case 239:
#line 4924 "lang.y"
{
#line 4801 "prolang.y"
	    indexing_argument = yyvsp[-7].lrvalue;
	    indexing_index1 = yyvsp[-4].lrvalue;
	    indexing_index2 = yyvsp[-1].lrvalue;
	    indexing_code = F_RR_RANGE_LVALUE - F_OFFSET;
	    goto range_lvalue_indexing;
	}
break;
case 240:
#line 4934 "lang.y"
{
	    last_string_constant = last_lex_string;
	    last_lex_string = 0;
	}
break;
case 241:
#line 4939 "lang.y"
{
	    add_string_constant();
	}
break;
case 242:
#line 4943 "lang.y"
{ fatal("presence of rule should prevent its reduction"); }
break;
case 243:
#line 4945 "lang.y"
{ fatal("presence of rule should prevent its reduction"); }
break;
case 245:
#line 4949 "lang.y"
{
#line 4826 "prolang.y"
	/* This seems to be an ordinary function call. But, if the function
	 * is not defined, then it might be a call to a simul_efun.
	 * If it is, then we make it a call_other(), which requires the
	 * function name as argument.
	 * We have to remember until after parsing the arguments if it was
	 * a simulated efun or not, which means that the pointer has to be
	 * pushed on a stack. Use the internal yacc stack for this purpose.
	 */
 	struct ident *real_name;

 	yyval.function_call_head.start = CURRENT_PROGRAM_SIZE;
 	yyval.function_call_head.simul_efun = -1;
 	real_name = yyvsp[0].function_name.real;
	/* we rely on the fact that $1.real->type is either
	 * I_TYPE_UNKNOWN or I_TYPE_GLOBAL here. All others are filtered
	 * by the lexical analysis.
	 */
	if (real_name->type == I_TYPE_UNKNOWN) {
	    /* prevent freeing by exotic name clashes */
	    /* also makes life easyer below */
	    real_name->type = I_TYPE_GLOBAL;
	    real_name->u.global.function = -1;
	    real_name->u.global.variable = -1;
	    real_name->u.global.efun     = -1;
	    real_name->u.global.sim_efun = -1;
	    real_name->next_all = all_globals;
	    all_globals = real_name;
	} else  if (!yyvsp[0].function_name.super && real_name->u.global.function < 0 &&
	  real_name->u.global.sim_efun >= 0) {
	    if ( (yyval.function_call_head.simul_efun =
				real_name->u.global.sim_efun) & ~0xff)
	    {
		PREPARE_S_INSERT(6)
		char *p = real_name->name;
		increment_string_ref(p);
		add_f_byte(F_STRING);
		add_short(store_prog_string(
		  make_shared_string(query_simul_efun_file_name())));
		add_f_byte(F_STRING);
		add_short(store_prog_string(p));
		CURRENT_PROGRAM_SIZE += 6;
	    }
	}
    }
break;
case 246:
#line 4996 "lang.y"
{ 
#line 4873 "prolang.y"
	PREPARE_S_INSERT(6)
	int f;
	unsigned short *arg_types;
	int first_arg;
	int efun_override = yyvsp[-4].function_name.super && strcmp(yyvsp[-4].function_name.super, "efun") == 0;
	int simul_efun;

	yyval.lrvalue.start = yyvsp[-3].function_call_head.start;
	yyval.lrvalue.code = -1;
	if ( (simul_efun = yyvsp[-3].function_call_head.simul_efun) >= 0) {
	    extern struct function *simul_efunp;
	    struct function *funp;

	    funp = &simul_efunp[simul_efun];
	    if (yyvsp[-1].number > funp->num_arg)
		yyerrorf("Too many arguments to simul_efun %s", funp->name);
	    if (simul_efun & ~0xff) {
		add_f_byte(F_CALL_OTHER);
		add_byte(yyvsp[-1].number + 2);
		CURRENT_PROGRAM_SIZE += 2;
	    } else {
		if (funp->num_arg != 0xff) {
		    int i;

		    i = funp->num_arg - yyvsp[-1].number;
		    if (i > 4 && CURRENT_PROGRAM_SIZE + i + 2 >
				mem_block[A_PROGRAM].max_size)
		    {
			realloc_a_program();
		    }
		    CURRENT_PROGRAM_SIZE += i;
		    while ( --i >= 0 ) {
			add_f_byte(F_CONST0);
		    }
		}
		add_f_byte(F_SIMUL_EFUN);
		add_byte(simul_efun);
		if (funp->num_arg == 0xff) {
		    add_byte(yyvsp[-1].number);
		    CURRENT_PROGRAM_SIZE += 3;
		} else
		    CURRENT_PROGRAM_SIZE += 2;
	    }
	    yyval.lrvalue.type = funp->type & TYPE_MOD_MASK;
	} else if (yyvsp[-4].function_name.super?!efun_override:(f=defined_function(yyvsp[-4].function_name.real)) >= 0)
	{
	    struct function *funp;
	    if (yyvsp[-4].function_name.super) {
	        struct program *super_prog;
		int ix;
		static struct function dummy;

	        ix = insert_inherited(
	          yyvsp[-4].function_name.super, yyvsp[-4].function_name.real->name,
		  &super_prog, &dummy, yyvsp[-1].number, __PREPARE_INSERT__p
	        );
	        if (yyvsp[-4].function_name.real->type == I_TYPE_UNKNOWN) {
	            free_shared_identifier(yyvsp[-4].function_name.real);
	        }
		if (ix < 0) {
		    yyerror("function not defined by inheritance as specified");
		    yyval.lrvalue.type = TYPE_ANY;
		    if (yyvsp[-4].function_name.super) yfree(yyvsp[-4].function_name.super);
		    pop_arg_stack(yyvsp[-1].number);	/* Argument types not needed more */
		    break;
		}
		if (super_prog && (arg_types = super_prog->argument_types)) {
		    first_arg = super_prog->type_start[ix];
		} else {
		    first_arg = INDEX_START_NONE;
		}
		funp = &dummy;
	    } else {
	        add_f_byte(F_CALL_FUNCTION_BY_ADDRESS); add_short(f);
	        funp = FUNCTION(f);
		arg_types = (unsigned short *)
		    mem_block[A_ARGUMENT_TYPES].block;
		first_arg = 
		  ((unsigned short *)mem_block[A_ARGUMENT_INDEX].block)[f];
	        add_byte(yyvsp[-1].number);	/* Actual number of arguments */
	        CURRENT_PROGRAM_SIZE += 4;
	    }
	    /*
	     * Verify that the function has been defined already.
	     */
	    if (funp->flags & (NAME_UNDEFINED|NAME_HIDDEN)) {
		if ( !(funp->flags & (NAME_PROTOTYPE|NAME_INHERITED)) &&
		     exact_types )
		{
		    yyerrorf("Function %.50s undefined", funp->name);
		} else if ((funp->flags &
			    (NAME_UNDEFINED|NAME_PROTOTYPE|NAME_HIDDEN))
			   == NAME_HIDDEN)
		{
		    yyerrorf("Function %.50s is private", funp->name);
		}
	    }
	    yyval.lrvalue.type = funp->type & TYPE_MOD_MASK;
	    /*
	     * Check number of arguments.
	     */
	    if (funp->num_arg != yyvsp[-1].number && !(funp->flags & TYPE_MOD_VARARGS) &&
		(first_arg != INDEX_START_NONE) && exact_types)
	    {
	        if (funp->num_arg-1 > yyvsp[-1].number || !(funp->flags & TYPE_MOD_XVARARGS))
		  yyerrorf("Wrong number of arguments to %.60s", yyvsp[-4].function_name.real->name);
	    }
	    /*
	     * Check the argument types.
	     */
	    if (exact_types && first_arg != INDEX_START_NONE)
	    {
		int i;
		unsigned short *argp;
		int num_arg, anum_arg;
		
		if (num_arg = funp->num_arg) {
		    if (funp->flags & TYPE_MOD_XVARARGS)
		      num_arg--; /* last argument is checked separately */
		    if (num_arg > (anum_arg = yyvsp[-1].number) )
			num_arg = anum_arg;
		    arg_types += first_arg;
		    argp = get_argument_types_start(anum_arg);
		    for (i = num_arg; --i >= 0; ) {
			int tmp1, tmp2;

			tmp1 = *argp++ & TYPE_MOD_RMASK;
			tmp2 = *arg_types++ & TYPE_MOD_MASK;
			if (!MASKED_TYPE(tmp1, tmp2)) {
			    yyerrorf("Bad type for argument %d of %s %s",
				anum_arg - i,
				funp->name,
				get_two_types(tmp2, tmp1));
			}
		    }
		    if (funp->flags & TYPE_MOD_XVARARGS)
		    {
			int tmp1, tmp2;
			/* varargs argument is either a pointer type or mixed */
			tmp2 = *arg_types & TYPE_MOD_MASK;
			tmp2 &= ~TYPE_MOD_POINTER;
			for (i = anum_arg - num_arg; --i >=0; )
			{
			    tmp1 = *argp++ & TYPE_MOD_RMASK;
			    if (!MASKED_TYPE(tmp1,tmp2)) {
				yyerrorf("Bad type for argument %d of %s %s",
				    anum_arg - i,
				    funp->name,
				    get_two_types(tmp2, tmp1));
			    }
			}
		    }
		}
	    }
	} else if ( (f = lookup_predef(yyvsp[-4].function_name.real)) != -1 ) {
	    int min, max, def, *argp, num_arg;
	    extern int efun_arg_types[];

	    {
		int f2;

		min = instrs[f].min_arg;
		max = instrs[f].max_arg;
		def = instrs[f].Default;
		yyval.lrvalue.type = instrs[f].ret_type;
		argp = &efun_arg_types[instrs[f].arg_index];
		num_arg = yyvsp[-1].number;
		if (def && num_arg == min-1) {
		    add_f_byte(def);
		    CURRENT_PROGRAM_SIZE++;
		    max--;
		    min--;
		} else if (num_arg < min &&
		    ( (f2 = proxy_efun(f, num_arg)) < 0 || (f = f2,MY_FALSE) )  )
		{
		    yyerrorf("Too few arguments to %s",
			    instrs[f].name);
		} else if (num_arg > max && max != -1) {
		    yyerrorf("Too many arguments to %s",
			    instrs[f].name);
		    num_arg = max;
		}
		if (max != -1 && exact_types && num_arg) {
		    /*
		     * Now check all types of the arguments to efuns.
		     */
		    int argn;
		    unsigned short *aargp;

		    aargp = get_argument_types_start(num_arg);
		    for (argn=0; argn < num_arg; argn++) {
			int tmp1, tmp2;
			tmp1 = *aargp++ & TYPE_MOD_MASK;
			for (;;) {
			    if ( !(tmp2 = *argp) ) {
				yyerrorf("Bad argument %d type to efun %s()",
				    argn+1, instrs[f].name);
				break;
			    }
			    argp++;
			    /* break if types are compatible; take care to
			     * handle references correctly
			     */
			    if (tmp1 == tmp2)
				break;
			    if ((tmp1 &
				 ~(TYPE_MOD_POINTER|TYPE_MOD_REFERENCE)) ==
				TYPE_ANY)
			    {
				if (tmp1 & TYPE_MOD_POINTER & ~tmp2) {
				    if ((tmp2 & ~TYPE_MOD_REFERENCE) !=
					TYPE_ANY)
				    {
					continue;
				    }
				}
				if ( !( (tmp1 ^ tmp2) & TYPE_MOD_REFERENCE) )
				    break;
			    } else if (
				(tmp2 &
				 ~(TYPE_MOD_POINTER|TYPE_MOD_REFERENCE)) ==
				TYPE_ANY)
			    {
				if (tmp2 & TYPE_MOD_POINTER & ~tmp1)
				    continue;
				if ( !( (tmp1 ^ tmp2) & TYPE_MOD_REFERENCE) )
				    break;
			    }
			} /* end for */
			while(*argp++);
		    }
		}
		if (f > 255) {
		    switch(0) { default:
			if (f > LAST_INSTRUCTION_CODE) {
			    f = efun_aliases[f-LAST_INSTRUCTION_CODE-1];
			    if (f <= 255)
				break;
			}
			add_byte(f >> F_ESCAPE_BITS);
			CURRENT_PROGRAM_SIZE++;
		    }
		}
		add_byte(f);
		CURRENT_PROGRAM_SIZE++;
		/* Only store number of arguments for instructions
		 * that allowed a variable number.
		 */
		if (max != min) {
		    add_byte(yyvsp[-1].number);/* Number of actual arguments */
		    CURRENT_PROGRAM_SIZE++;
		}
		if ( instrs[f].ret_type == TYPE_VOID ) {
		    last_expression = mem_block[A_PROGRAM].current_size;
		    add_f_byte(F_CONST0);
		    CURRENT_PROGRAM_SIZE++;
		}
	    }
	} else if (efun_override) {
	    yyerrorf("Unknown efun: %s", yyvsp[-4].function_name.real->name);
	    yyval.lrvalue.type = TYPE_ANY;
	} else {
	    struct function *funp;

	    f = define_new_function(
		yyvsp[-4].function_name.real, 0, 0, 0, NAME_UNDEFINED, TYPE_UNKNOWN
	    );
	    add_f_byte(F_CALL_FUNCTION_BY_ADDRESS);
	    add_short(f);
	    add_byte(yyvsp[-1].number);	/* Number of actual arguments */
	    CURRENT_PROGRAM_SIZE += 4;
	    funp = FUNCTION(f);
	    if (exact_types) {
		yyerrorf("Undefined function %.50s", yyvsp[-4].function_name.real->name);
	    }
	    yyval.lrvalue.type = TYPE_ANY;	/* Just a guess */
	}
	if (yyvsp[-4].function_name.super) yfree(yyvsp[-4].function_name.super);
	pop_arg_stack(yyvsp[-1].number);	/* Argument types not needed more */
    }
break;
case 247:
#line 5279 "lang.y"
{
#line 5156 "prolang.y"
	int string_number;
	char *p = yyvsp[0].function_name.real->name;
	increment_string_ref(p);
	if (yyvsp[0].function_name.real->type == I_TYPE_UNKNOWN)
	    free_shared_identifier(yyvsp[0].function_name.real);
	if (yyvsp[0].function_name.super) {
	    yfree(yyvsp[0].function_name.super);
	    yyerror("inherited function may not be called by call_other");
	}
        string_number = store_prog_string(p);
        if        ( string_number <= 0x0ff ) {
            ins_f_byte(F_CSTRING0);
            ins_byte(string_number);
        } else if ( string_number <= 0x1ff ) {
            ins_f_byte(F_CSTRING1);
            ins_byte(string_number);
        } else if ( string_number <= 0x2ff ) {
            ins_f_byte(F_CSTRING2);
            ins_byte(string_number);
        } else if ( string_number <= 0x3ff ) {
            ins_f_byte(F_CSTRING3);
            ins_byte(string_number);
        } else {
            ins_f_byte(F_STRING);
            ins_short(string_number);
        }
    }
break;
case 248:
#line 5309 "lang.y"
{
	ins_f_byte(F_CALL_OTHER);
	ins_byte(yyvsp[-1].number + 2);
	yyval.lrvalue.type = instrs[F_CALL_OTHER-F_OFFSET].ret_type;
	yyval.lrvalue.code = -1;
	yyval.lrvalue.start = yyvsp[-6].lrvalue.start;
	pop_arg_stack(yyvsp[-1].number);	/* No good need of these arguments */
    }
break;
case 249:
#line 5319 "lang.y"
{
	    yyval.string = ystring_copy(yyvsp[0].ident->name);
	    if (yyvsp[0].ident->type == I_TYPE_UNKNOWN)
		free_shared_identifier(yyvsp[0].ident);
	}
break;
case 250:
#line 5325 "lang.y"
{ fatal("presence of rule should prevent its reduction"); }
break;
case 251:
#line 5327 "lang.y"
{
	    yyval.string = ystring_copy(last_lex_string);
	    free_string(last_lex_string);
	    last_lex_string = 0;
	}
break;
case 252:
#line 5334 "lang.y"
{
		    yyval.function_name.super = 0;
		    yyval.function_name.real  = yyvsp[0].ident;
		}
break;
case 253:
#line 5339 "lang.y"
{
		    *(yyval.function_name.super = yalloc(1)) = '\0';
		    yyval.function_name.real  = yyvsp[0].ident;
		}
break;
case 254:
#line 5344 "lang.y"
{
		    extern struct function *simul_efunp;
		    extern struct object *master_ob;
#line 5223 "prolang.y"
		    if ( !strcmp(yyvsp[-2].string, "efun") &&
		      yyvsp[0].ident->type == I_TYPE_GLOBAL &&
		      yyvsp[0].ident->u.global.sim_efun >= 0 &&
		      simul_efunp[yyvsp[0].ident->u.global.sim_efun].flags &
			TYPE_MOD_NO_MASK &&
		      master_ob
		    )
		    {
			struct svalue *res;

			push_constant_string("nomask simul_efun");
			push_volatile_string(current_file);
			push_volatile_string(yyvsp[0].ident->name);
			res = apply_master_ob("privilege_violation", 3);
			if (!res || res->type != T_NUMBER || res->u.number < 0)
			{
			    yyerrorf(
			      "Privilege violation: nomask simul_efun %s",
			      yyvsp[0].ident->name
			    );
			    yfree(yyvsp[-2].string);
			    yyval.function_name.super = 0;
			} else if (!res->u.number) {
			    yfree(yyvsp[-2].string);
			    yyval.function_name.super = 0;
			} else {
			    yyval.function_name.super = yyvsp[-2].string;
			}
		    } else yyval.function_name.super = yyvsp[-2].string;
		    yyval.function_name.real  = yyvsp[0].ident;
		}
break;
case 255:
#line 5381 "lang.y"
{
	    int current;
	    char *current_code;

	    yyval.numbers[0] = current_break_address;
	    /* don't place labels to enter the inner part of an if */
	    current_break_address &= ~CASE_LABELS_ENABLED;
	    current = CURRENT_PROGRAM_SIZE;
	    if (current + 2 > mem_block[A_PROGRAM].max_size)
		realloc_a_program();
	    current_code = mem_block[A_PROGRAM].block + current;
	    if (last_expression == current - 1 &&
		current_code[-1] == F_NOT - F_OFFSET)
	    {
		current_code[-1] = F_BRANCH_WHEN_NON_ZERO - F_OFFSET;
	    } else {
		*current_code = F_BRANCH_WHEN_ZERO - F_OFFSET;
		current++;
	    }
	    yyval.numbers[1] = current;
	    CURRENT_PROGRAM_SIZE = current + 1;
	}
break;
case 256:
#line 5407 "lang.y"
{
	    int destination, location, offset;

	    destination = yyvsp[0].number;
	    location = yyvsp[-2].numbers[1];
	    if ( (offset = destination - location) > 0x100) {
		FIX_BRANCH(
		  mem_block[A_PROGRAM].block[location-1] ==
		   F_BRANCH_WHEN_ZERO - F_OFFSET ?
		    F_LBRANCH_WHEN_ZERO :
		    F_LBRANCH_WHEN_NON_ZERO
		  ,
		  destination, location
		);
	    } else {
		mem_block[A_PROGRAM].block[location] = offset - 1;
	    }
	    current_break_address |= yyvsp[-2].numbers[0] & CASE_LABELS_ENABLED;
	}
break;
case 257:
#line 5427 "lang.y"
{
	yyval.number=mem_block[A_PROGRAM].current_size;
    }
break;
case 258:
#line 5431 "lang.y"
{
	ins_f_byte(F_BRANCH);
	yyval.number = CURRENT_PROGRAM_SIZE;
	ins_byte(0);
    }
break;
case 259:
#line 5437 "lang.y"
{
	yyval.number = FIX_BRANCH( F_LBRANCH, CURRENT_PROGRAM_SIZE, yyvsp[-1].number);
	yyval.number += yyvsp[-1].number + 1;
    }
break;
#line 8964 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
