// These are a number of Timewarp's simul efuns.
// They are used throught the bim distribution, so I thought
// I'd better put them in the distribution too.
// Either use bim.h to inherit them into the files, or add
// them as simul_efuns.  Your choice.
// Bubbs.  24/Sep/94.

#include <bim.h>

#if defined(SML_SET_OF)
mixed *set_of(mixed *arr) { return m_indices(mkmapping(arr)); }
#endif


#if defined(SML_SUBST)
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
#endif


#if defined(SML_SUM_ARRAY)
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
#endif


#if defined(SML_TRIMSTR)
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
#endif
