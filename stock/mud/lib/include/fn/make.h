 /* make_path - for players */

string make_path(string file) {
  string tmp1, tmp2;
  string *path_split;
  int i;

  if(sscanf(file,"%s..%s", tmp1, tmp2)) {
    while((i = member_array("..",path_split)) != -1) {
      path_split = path_split[0..i-2]+path_split[i+1..sizeof(path_split)-1];
    }
    file = "/" +implode(path_split,"/");
    while(sscanf(file, "%s//%s", tmp1, tmp2)) file = tmp1 + "/" + tmp2;
  }
  return file;
}
