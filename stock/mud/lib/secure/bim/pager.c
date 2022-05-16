// Pager for BIM.
// Bubbs.   4/June/95.
// v1.0.1

// To use this pager daemon as part of something else, for example,
// a bulletin board, all you need is one function call.
// However, several arguments must be passed to that function.
// Call start_pager() with a mapping argument, see that
// function for more details.

#include <bim.h>
#include <ansi.h>

#if defined(SML_TRIMSTR)
inherit BIM_DIR"simuls";
#endif


// Version of this editor.
#define PAGER_VERSION  "1.0.1"

void print_prompt (mapping data);
void print_page   (mapping data);
void process_input(string input, mapping data);
void end_pager    (mapping data, status eof);


varargs void start_pager(mapping data) {
  if (!data) data = ([ ]);
  if (!data[PG_BUFF])   data[PG_BUFF]   = ({ });
  if (!data[PG_CURR])   data[PG_CURR]   = 1;
  if (!data[PG_LENG])   data[PG_LENG]   = DEF_PAGE_LENG;
  if (!data[PG_MAILR])  data[PG_MAILR]  = previous_object();
  if (!data[PG_Q_FUNC]) data[PG_Q_FUNC] = "end_pager";
  data[PG_SIZE] = sizeof(data[PG_BUFF]);
  print_page(data);
  return;
  }

void print_prompt(mapping data) {
  printf(NORM B_GREEN_F "More-%d/%d" B_RED_F " (%2d%%) " NORM, data[PG_CURR],data[PG_SIZE],100*data[PG_CURR] / data[PG_SIZE]);
  input_to("process_input", 0, data);
  return;
  }


void print_page(mapping data) {
  int start, index, end;
  status eof;
  if (data[PG_LENG] < 0)
   data[PG_LENG] = 0;
  start = data[PG_CURR];
  end = start + data[PG_LENG];
  if (end >= data[PG_SIZE])
   end = data[PG_SIZE], eof = 1;
  for(index = start; index < end; index++)
   printf("%s\n", data[PG_BUFF][index]);
  data[PG_CURR] = end;
  printf(NORM);
  if (eof) end_pager(data, 1);
  else print_prompt(data);
  return;
  }


void process_input(string input, mapping data) {
  string rest, *txt;
  int index;
  rest = trimstr(input[1..]);
  switch(input[0]) {
    case  0 :  print_page(data);    break;
    case 'b':
      data[PG_CURR] = data[PG_SIZE] - data[PG_LENG];
      print_page(data);
    break;
    case 'd':
      data[PG_CURR] += data[PG_LENG];
      print_page(data);
    break;
    case 'g':
      if (sscanf(rest, "%d", index) == 0)
       index = 0;
      data[PG_CURR] = (index * data[PG_SIZE]) / 100;
      print_page(data);
      break;
    case 'h':
      write("BIM default pager:   Version "PAGER_VERSION".\n"
            "<return>             See next page of text.\n"
            "b                    Goto the bottom of text.\n"
            "d                    Go down one page.\n"
            "g<perc>              Goto <perc> percent way through text.\n"
            "h                    This help.\n"
            "l                    Reprint current page.\n"
            "q                    Quit pager.\n"
            "t                    Goto the top of the text.\n"
            "u                    Go up one page.\n"
            "/<patt>              Search (forwards) for <patt>.\n"
            "?<patt>              Search (backwards) for <patt>.\n");
      print_prompt(data);
    break;
    case 'l':
      data[PG_CURR] -= data[PG_LENG];
      print_page(data);
    break;
    case 'q':  end_pager(data, 0);  break;
    case 't':
      data[PG_CURR] = 0;
      print_page(data);
    break;
    case 'u':
      data[PG_CURR] -= 2*data[PG_LENG];
      if(data[PG_CURR] < 0) 
          data[PG_CURR] = 0;
      print_page(data);
    break;
    case '/':
    case '?':
      if (rest == "")
       rest = data[PG_PATT];
      if (!rest) {
        write("No pattern in search buffer.\n");
        print_prompt(data);
        }
      else {
        txt = (input[0] == '/' ? data[PG_BUFF][data[PG_CURR]..] :
                                 data[PG_BUFF][0..data[PG_CURR]-1]);
        txt = regexp(txt, rest);
        data[PG_PATT] = rest;
        if (!sizeof(txt)) {
          write("Match not found.\n");
          print_prompt(data);
          }
        else {
          data[PG_CURR] = member_array(txt[0], data[PG_BUFF]);
          print_page(data);
          }
        }
    break;
    default :  print_prompt(data);  break;
    }
  return;
  }

void end_pager(mapping data, status eof) {
  if (!eof) write("Terminated.\n");
  if (closurep(data[PG_Q_FUNC]))
   funcall(data[PG_Q_FUNC], data);
  else
   (void)call_other(data[PG_MAILR], data[PG_Q_FUNC], data);
  return;
  }
