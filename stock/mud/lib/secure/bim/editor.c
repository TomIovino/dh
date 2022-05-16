// Editor for BIM.
// Bubbs.  28/Sep/94.
// v1.3.1

// To use this editor daemon as part of something else, for example,
// a bulletin board, all you need is one function call.
// However, several arguments must be passed to that function.
// Call start() with a mapping argument, see that function
// for more details.

#include <bim.h>

#if defined(SML_SUBST) || defined(SML_TRIMSTR)
inherit BIM_DIR"simuls";
#endif


// Version of this editor.
#define ED_VERSION    "1.3.1"


void print_prompt (mapping data);
void process_input(string input, mapping data);
void end_editing  (mapping data);
void get_range    (string desc, mapping data, int a, int b);
int  get_index    (string desc, mapping data);


varargs void start_editing(mapping data) {
  if (!data) data = ([ ]);
  if (!data[ED_BUFF])   data[ED_BUFF]   = ({ });
  if (!data[ED_CURR])   data[ED_CURR]   = 0;
  if (!data[ED_MAILR])  data[ED_MAILR]  = previous_object();
  if (!data[ED_Q_FUNC]) data[ED_Q_FUNC] = "end_editing";
write("                                                        End Lines Here --v\n");
write("Enter text, type '**' to quit, '~q' to abort, and '~?' for help.         *\n");
  print_prompt(data);
  return;
  }

void print_prompt(mapping data) {
  printf("%2d] ", data[ED_CURR] + 1);
  input_to("process_input", 0, data);
  return;
  }

void process_input(string input, mapping data) {
  string rest, *buff;
  int a, b, index;
  rest = trimstr(input[2..]);
  switch(input[0..1]) {
    case "**":     // Save and quit.
    case "~x":
      end_editing(data);
      return;
    break;
    case "~q":     // Abort and quit.
    case "~Q":
      data[ED_BUFF] = 0;
      end_editing(data);
      return;
    break;
    case "~r":    // Read/print lines.
    case "~p":
    case "~l":
      if (rest == "") rest = (input[0..1] == "~p" ? "." : "1,$");
      get_range(rest, data, &a, &b);
      if (sizeof(data[ED_BUFF]))
       for(index = a; index <= b; index++)
        printf("%2d] %s\n", index+1, data[ED_BUFF][index]);
    break;
    case "~d":    // Delete lines.
      if (rest == "") rest = ".";
      get_range(rest, data, &a, &b);
      data[ED_BUFF] = data[ED_BUFF][0..a-1] + data[ED_BUFF][b+1..];
      index = sizeof(data[ED_BUFF]);
      if (data[ED_CURR] > index)
       data[ED_CURR] = index;
    break;
    case "~g":    // Goto line.
      if (rest == "") rest = ".";
      index = get_index(rest, data);
      if (index < 0)
       write("No such line.\n");
      else {
        data[ED_CURR] = index + 1;
        printf("%2d] %s\n", index+1, data[ED_BUFF][index]);
        }
    break;
    case "~s": {  // Substitute.
      mixed *bits;
      get_range(&rest, data, &a, &b);
      if (!rest) {
        a = b = data[ED_CURR] - 1;
        rest = input[2..];
        }
      bits = explode(rest[1..], rest[0..0]);
      switch(sizeof(bits)) {
        case 0 :
        case 1 :  bits  = 0;          break;    // Boggle.
        case 2 :  bits += ({ "1" });  break;    // Do 1st occurance.
        case 3 :                      break;
        default:  bits = bits[0..2];  break;    // Ignore extra args.
        }
      if (bits && sizeof(data[ED_BUFF])) {
        int s, e;
        if (bits[2] == "g") bits[2] = -1;
        else if (sscanf(bits[2], "%d", bits[2]))
         bits[2] = 2 * bits[2] - 1;
        else bits[2] = 1;
        for(index = a; index <= b; index++) {
          buff = regexplode(data[ED_BUFF][index], bits[0]);
          e = sizeof(buff);
          for(s = 1; s < e; s += 2)
           if (bits[2] == -1 || bits[2] == s) {
             buff[s] = subst(bits[1], "&", buff[s]);         
//           buff[s] = bits[1];
             buff[s] = subst(buff[s], "\\n", "\n");
             }
          data[ED_BUFF][index] = implode(buff, "");
          printf("%2d] %s\n", index+1, data[ED_BUFF][index]);
          }
        data[ED_CURR] = b+1;
        }
      }
    break;
    case "~?":    // Help.
    case "~h":
      printf("BIM default editor:  Version "ED_VERSION".\n"
             "**          ~x       Save and quit.\n"
             "~q          ~Q       Abort and quit.\n"
             "~d<lines>            Delete lines.\n"
             "~g<line>             Goto line.\n"
             "~h          ~?       This help page.\n"
             "~p<lines>   ~r       Print lines.\n"
             "~s<range>/patt/txt/[g|<num>]\n"
             "                     Substitute 'patt' for 'txt' (<num>th occurance).\n"
             "~R<num>              Read in numbered message.\n"
             "%s"
             "~~<text>             Write \"~<text>\" into buffer.\n"
             "<text>               Write \"<text>\" into buffer.\n",
       ALLOW_R_W_FILE(this_player()) ?
             "~R<file>             Read in contents of file.\n"
             "~W<file>             Write buffer to file.\n" :
             "");
    break;
    case "~R":    // Read into buffer.
      if (sscanf(rest, "%d", index) == 1 || sscanf(rest, "#%d", index) == 1)
       buff = (string *)(data[ED_MAILR])->get_message_text(index);
      else if (ALLOW_R_W_FILE(this_player())) {
#if defined(EVAL_PATH)
        rest = EVAL_PATH(rest);
#endif   // EVAL_PATH
        b = file_size(rest);
        if (b < 0)
         printf("Could not read: %s.\n", rest);
        else if (b > FILE_TOO_BIG)
         printf("File too big: %s.\n", rest);
        else {
          string text;
          text = read_file(rest);
          if (text) {
            if (text[<1] == '\n')
             text = text[0..<2];
            buff = explode(text, "\n");
            printf("Read from file: %s.\n", rest);
            }
          else
           printf("Failed to read from file: %s.\n", rest);
          }
        }
      if (b = sizeof(buff)) {
        for(a = 0; a < b; a++)
         buff[a] = sprintf("> %s", buff[a]);
        index = data[ED_CURR];
        data[ED_BUFF] = data[ED_BUFF][0..index-1] + buff + data[ED_BUFF][index..];
        data[ED_CURR] += b;
        }
    break;
    case "~W":
      if (ALLOW_R_W_FILE(this_player())) {
#if defined(EVAL_PATH)
        rest = EVAL_PATH(rest);
#endif   // EVAL_PATH
        b = file_size(rest);
        if (b == -2)
         printf("Cannot write to directory: %s.\n", rest);
        else if (b != -1)
         printf("File already exists: %s.\n", rest);
        else {
          printf("Written buffer to file: %s.\n", rest);
          write_file(rest,
           sprintf("%s\n", implode(data[ED_BUFF], "\n")));
          }
        }
    break;
    default:
      if (input[0] == '~' && input[1] != '~') {
        index = get_index(input[1..], data);
        if (index >= 0) {
          data[ED_CURR] = index + 1;
          printf("%2d] %s\n", index+1, data[ED_BUFF][index]);
          }
        else if (index == -1)
         write("No such line.\n");
        else
         printf("Unrecognised command: %O.\n", input);
        }
      else {
        if (input[0] == '~') input = input[1..];
        index = data[ED_CURR];
        data[ED_BUFF] = data[ED_BUFF][0..index-1] +
         ({ input }) + data[ED_BUFF][index..];
        data[ED_CURR]++;
        }
    break;
    }
  print_prompt(data);
  return;
  }

void end_editing(mapping data) {
  if (closurep(data[ED_Q_FUNC]))
   funcall(data[ED_Q_FUNC], data);
  else
   (void)call_other(data[ED_MAILR], data[ED_Q_FUNC], data);
  return;
  }

void get_range(string desc, mapping data, int a, int b) {
  string A, B;
  if (sscanf(desc, "%s,%s", A, B) != 2) {
    a = b = get_index(&desc, data);
    }
  else {
    a = get_index(A, data);
    b = get_index(&B, data);
    desc = B;
    }
  return;
  }

int get_index(string desc, mapping data) {
  string base;
  int num, mod, max;
  status add;
  max = sizeof(data[ED_BUFF]);
  if ((sscanf(desc, "%s+%d%s", base, mod, desc) == 3 && add = 1) ||
       sscanf(desc, "%s-%d%s", base, mod, desc) == 3) {
    switch(base) {
      case "" :
      case ".":  num = data[ED_CURR];  break;
      case "$":  num = max;            break;
      default:
        if (sscanf(base, "%d", num) == 0)
         return -2;
      break;
      }
    num += (add ? mod : -mod);
    }
  else if (sscanf(desc, ".%s", desc) == 1)
   num = data[ED_CURR];
  else if (sscanf(desc, "$%s", desc) == 1)
   num = max;
  else if (sscanf(desc, "%d%s", num, desc) == 2);
  else
   return -2;
  return (num < 1 | num > max ? -1 : num - 1);
  }
