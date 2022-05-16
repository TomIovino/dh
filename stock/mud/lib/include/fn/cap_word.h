// cap_word() : Capitalize all the words in a string.
// Bubbs. 13/Aug/93.

#if !defined(CAP_WORD)

#define CAP_WORD

string cap_word(string str) {
  string *words;
  int index, size;
  if (!str || !stringp(str))
   raise_error("cap_word() : Bad type to argument 1.\n");
  words = explode(str, " ");
  size = sizeof(words);
  for(index = 0; index < size; index++)
   words[index] = capitalize(words[index]);
  return implode(words, " ");
  }

#endif    /* CAP_WORD */
