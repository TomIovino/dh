  ////////////////////////////////
//////  Debug Utilities v1.0  ////
//////////////////////////////////
//////     By  :  Geewhiz     ////
//////////////////////////////////
////// atgraham@ptdprolog.net ////
//////////////////////////////////
////////////////////////////////

/** 
 ** What is it?
 **    Provides two macro defines:
 **        PRINTD() and ASSERT()
 **    PRINTD(variable)  will print out
 **    the variables name and it's value.
 **    very useful for debugging code.
 **
 **    ASSERT(statement or variable)
 **    will abort program execution if
 **    the given variable or statement is
 **    not true (i.e. it is false). Very
 **    useful for verifying that varibles
 **    are not changed by some rogue code
 **    I.e. if you know that variable foo
 **    was not 0 before you can use
 **    ASSERT(foo != 0);
 **    before the place where it can't be
 **    0.  Thus telling you that it was
 **    changed before that point if the
 **    assertion failed.
 **
 ** To Use:
 **    #include <debug_utils.h>
 **
 ** In Code:
 **    ...
 **    string name;
 **    
 **    name = this_player()->query_real_name();
 **    PRINTD(name);
 **    ...
 ** Would print (to wizard who wrote file)
 **    ========================================
 **    /w/name/filename (linenumber) w/name/filename#xxx
 **    name =
 **    "geewhiz"
 **    ========================================
 **
 **/


#ifndef PRINTDH
#define PRINTDH
 
#ifdef NO_PRINTD
#  undef  PRINTD
#  define PRINTD(X)
#else
#  undef  PRINTD
#  define PRINTD(X)  ___printd(__FILE__,__LINE__,"X",(X))
#endif

#ifdef NO_ASSERT
#  undef ASSERT
#  define ASSERT(X)
#else
#  undef ASSERT
#  define ASSERT(X)  ___assert(__FILE__,__LINE__,"X",(X))
#endif

varargs
private static void ___printd(string file,int line,string var, mixed val, int assrt, object me) {
#ifdef DEBUG_WIZARD_NAME
    if(!me)
        me = find_player(DEBUG_WIZARD_NAME);
#else
    if(!me)
        if(!me = find_player((string)__MASTER_OBJECT__->get_wiz_name(file[1..])))
            me = this_player();
#endif
    if(!me) return;
    tell_object(me,sprintf("\n========================================\n"));
    if(assrt) {
        tell_object(me,sprintf("\t*** ASSERTION FAILED! ***\n"));
        tell_object(me,sprintf("========================================\n"));
    }
    tell_object(me,sprintf("%s (%d) %-=*O\n",file,line,70 - strlen(file),this_object()));
    tell_object(me,sprintf("%s = %-=*O\n",var,75 - strlen(var),val));
    tell_object(me,sprintf("========================================\n\n"));
}

private static void ___assert(string file,int line,string var, mixed val) {
    object who;
    if(!val) {
        ___printd(file,line,var,val,1,&who);
        if(who && this_player() && who != this_player())
            ___printd(file,line,var,val,1,this_player());
        raise_error("Assertion failed! in " + file + " (Line: " + line + ")\n");
    }
}


#endif

