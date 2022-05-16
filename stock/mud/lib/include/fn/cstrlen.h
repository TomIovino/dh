 
int cstrlen(string msg) {
  int len;
  string color, r, t, e;

  color = sprintf("%%s%c[%%sm%%s",27);
  while(sscanf(msg, color, msg, t, e)) msg += e;
  len = strlen(msg);
  return len;
}
