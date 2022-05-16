status user_exists(string who) {
    if(!who || !stringp(who)) raise_error("user_exists: Bad Arg 1\n");
    return restore_object(sprintf("/data/players/%c/%s",who[0],who));
}
 
// break_point is a DANGEROUS efun.  Can crash the mud if
// called like:  printf(break_point());
 
nomask void break_point() {}
 
varargs string wrap(string str_to_wrap, int max_col, int indent);
 
status is_clone(mixed ob) {
    return objectp(ob) && member(file_name(ob), '#') >= 0;
}
 
/*========================================================================*/
/*=== Simul_Efuns borrowd from Dreamtime LPMud                         ===*/
/*========================================================================*/
 
// format() --  String Formatter (Based on code snippets from a
//              discussion in the rec.games.mud.admin/rec.games.mud.lp
//              on 941205 by dack@prag.darmstadt.gmd.de) This version
//              coded by Geewhiz@Dreamtime for DreamtimeLPmud.
// ----------------------------------------------------------------------
//      Usage:  format(string str [,int indent [,int shift [,int width]]]);
//              string str  -- String to format (Required)
//              int indent  -- Number of spaces to indent lines (Optional)
//                             (Defaults to 0)
//              int shift   -- Number of spaces to shift first line (Optional)
//                             negative shift is to the left
//                             postive shift is to the right
//                             zero shift is neutral
//              int width   -- Max Column                       
 
varargs string format(string str, int indent, int shift, int width) {
    int extra,pre;
    if(!str || str == "" || !stringp(str)) return "";
    if(!indent || indent < 0) indent = 0;
    if(!width || width < 0) width = 75;
    if(shift < 0) {
        pre = -1 * shift;
        extra = 0;
    }
    if(shift == 0) {
        pre = 0;
        extra = 0;
    }
    if(shift > 0) {
        pre = 0;
        extra = shift;
    }
    if(extra) str = sprintf("%*s",extra,"") + str;
    return sprintf("%*s%-*=s\n",indent,str[0..pre-1],width-indent,str[pre..]);
}
 
varargs string wrap(string str, int width, int indent) {
    if(!width) width = 75;
    if(!indent) indent = 0;
    return format(str,indent,(-1*indent),width);
}
 
// =================================================================
// These are a number of Timewarp's simul efuns.
// They are used throughout the bim distribution, so I thought
// I'd better put them in the distribution too.
// Either use bim.h to inherit them into the files, or add
// them as simul_efuns.  Your choice.
// Bubbs.  24/Sep/94.
 
string subst(string str, string patt, string repl) {
  string new;
  int index, len1, len2;
  if (str == 0) str = "";
  if (!stringp(str))
   raise_error("subst(): Bad type to argument 1.\n");
  if (!stringp(patt))
   raise_error("subst(): Bad type to argument 2.\n");
  if (!stringp(repl))
   raise_error("subst(): Bad type to argument 3.\n");
  if (strstr(str, patt) == -1)
   return str;
  return implode(explode(str, patt), repl);
  }
 
mixed sum_array(mixed *arr) {
  mixed ret;
  int index, size;
  size = sizeof(arr);
  if (!size) return 0;
  ret = arr[0];
  for(index = 1; index < size; index++)
   ret = ret + arr[index];
  return ret;
  }
 
varargs string trimstr(string str, int char, int mode) {
  string new;
  if (!str || !stringp(str)) return 0;
  if (str == "") return str;
  if (!char) char = ' ';
  if (!mode) mode = 3;
  new = str;
  if (mode & 1)
   while(new && new != "" && new[0] == char)
    new = new[1..];
  if (mode & 2)
   while(new && new != "" && new[<1] == char)
    new = new[0..<2];
  return new;
  }
 
mixed *set_of(mixed *arr) { return m_indices(mkmapping(arr)); }
 
