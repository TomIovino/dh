private static closure match;
string subst(string str, string patt, string repl);

string *match_wildcard(string pattern, string *input) {
  int s;
  if (!stringp(pattern))
   raise_error("match_wildcard(): Bad type to argument 1.\n");
  if (!pointerp(input))
   raise_error("match_wildcard(): Bad type to argument 2.\n");
  if (!match)
   match = lambda(({ 'input, 'pattern, 's }),
    ({ #'?,
      ({ #'stringp, 'input }),
      ({ #'==,
        ({ #'sscanf, 'input, 'pattern }),
        's
      })
    }));
  s = sizeof(regexplode(pattern, "\\*|\\?")) / 2;
  if (!s)
   return filter_array(input, #'==, pattern);
  else
   return filter_array(input, match,
    subst(subst(pattern, "*", "%~s"), "?", "%~1s"), s);
  }
