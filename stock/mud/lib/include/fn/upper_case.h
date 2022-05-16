// upper_case() : Opposite of lower_case()
// Bubbs.  4/Jul/94.

#if !defined(UPPER_CASE)

#define UPPER_CASE

string upper_case(string str) {
  return to_string(map_array(to_array(str), lambda(({ 'char }),
   ({ #'?,
     ({ #'&&,
       ({ #'>=, 'char, 'a' }),
       ({ #'<=, 'char, 'z' }),
     }),
     ({ #'-, 'char, 'a' - 'A' }),
     'char
   }))));
  }

#endif    // UPPER_CASE
