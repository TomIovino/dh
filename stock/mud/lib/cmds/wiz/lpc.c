// By Bubbs.
// A checkered history, passing through
// several muds, including LPCC, Dune and Timewarp.
// Current version dated 6/Aug/94.

#define TMP_FILE\
 sprintf("/w/%s/tmp_code", \
  (string)this_player()->query_real_name())

inherit "secure/bim/simuls";

mixed do_lpc(string file, int cost) {
  mixed result;
  int pre, post;
  pre = get_eval_cost();
  if(this_player()->query_security() < 30){ return 0; } // Ash
  result = (mixed)file->test();
  post = get_eval_cost();
  cost = pre - post;
  return result;
  }

status main(string str) {
  mixed result;
  object ob;
  string file, error;
  int tmp, cost;
  status silent;
  file = TMP_FILE;
  if (str == "-clean") {
    while(file_size(error = sprintf("%s%d.c", file, tmp)) != -1)
     rm(error), tmp++;
    return 1;
    }
  if (str && str[0..2] == "-s ")
   silent = 1, str = str[2..];
  str = (str ? trimstr(str, ';', 2) : "0");
  while(file_size(sprintf("%s%d.c", file, tmp)) != -1) tmp++;
  file = sprintf("%s%d.c", file, tmp);
  if (ob = find_object(file)) destruct(ob);
  if (member_array(';', str) == -1)
   str = sprintf("return (%s)", str);
  write_file(file,
   sprintf("#include <mudlib.h>\n"
            "#include <ansi.h>\n"
           "\n"
           "#define me this_player()\n"
           "#define here MYENV(me)\n"
           "#define FO(o) find_object(\"o\")\n"
           "#define FP(p) find_player(\"p\")\n"
           "#define FL(l) find_living(\"l\")\n"
           "#define MYENV(e) environment(e)\n"
           "#define PRES(s, a) present(s, a)\n"
           "#define QUERY(N) FO(%s/**/N)\n"
           "#define DUMP(V) sprintf(\"%%O\", V)\n"
           "\n"
           "closure cl, cl2, cl3;\n"
           "mapping map = ([ ]), map2 = ([ ]), map3 = ([ ]);\n"
           "object ob, ob2, ob3, *obs = ({ });\n"
           "string str, str2, str3, *strs = ({ });\n"
           "float fl, fl2, fl3, *fls = ({ });\n"
           "int num, num2, num3, *nums = ({ }), a, b, c;\n"
           "\n"
           "test(arg, arg2, arg3) {\n"
           "  %s;\n"
           "  }\n",
    TMP_FILE,
    str));
  error = catch(result = do_lpc(file, &cost));
  if (error) write((error[0] == '*' ? error[1..] : error));
  else {
  rm(file);
    if (ob = find_object(file)) destruct(ob);
    if (!silent)
     printf("Result (%d): %O\n", cost, result);
    }
  return 1;
  }

void help() {
  write("Usage: lpc [-s] <lpc code>\n"
        "This command allows testing of efuns and lfuns\n"
        "as if you had written a test-file.  It does this\n"
        "by automatically writing a test-file for you.\n"
        "The \"-s\" flag is to supress the return value report.\n"
        "\n"
        "There are many variables and macros provided for use.\n"
        "(the best way to look at them is to examine a test\n"
        "file yourself....)\n"
        "\n"
        "Examples: lpc explode(\"wibble\", \"b\")\n"
        "          lpc ob = FL(guard); return creator(ob);\n"
        "          lpc ({ sscanf(\"test\", \"t%st\", str), str })\n");
  return;
  }
