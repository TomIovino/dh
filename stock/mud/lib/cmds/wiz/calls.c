#pragma strict_types

string fmt_args(mixed a, mixed b, mixed c, mixed d, mixed e, mixed f, 
                mixed g, mixed h, mixed i, mixed j, mixed k, mixed l) {
    return sprintf("%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s",
            to_string(a),b ? ' ' : 0,
            b ? to_string(b) : "",c ? ' ' : 0,
            c ? to_string(c) : "",d ? ' ' : 0,
            d ? to_string(d) : "",e ? ' ' : 0,
            e ? to_string(e) : "",f ? ' ' : 0,
            f ? to_string(f) : "",g ? ' ' : 0,
            g ? to_string(g) : "",h ? ' ' : 0,
            h ? to_string(h) : "",i ? ' ' : 0,
            i ? to_string(i) : "",j ? ' ' : 0,
            j ? to_string(j) : "",k ? ' ' : 0,
            k ? to_string(k) : "",l ? ' ' : 0,
            l ? to_string(l) : "");
}            


int main()
{
        int i, s, a;
        mixed *info;
        string ob;
        mixed *call_inf;
        string func;

        info = call_out_info();
        s = sizeof(info);
        i = s;

        printf("%-'-'30s%-'-'20s%-'-'5s | %-'-'20s\n", "Object", "Function",
       "Delay", "Arguments");

        while(i--) {
          call_inf = info[i];
          a = sizeof(call_inf);
          
          if(closurep(call_inf[0]))
            ob = sprintf("%O",call_inf[0]);
          else if(!objectp(call_inf[0]))
             ob = sprintf("(unknown) %O",call_inf[0]);
           else
             ob= (string) call_inf[0];
             
          if(closurep(call_inf[1]))
            func = sprintf("%O",call_inf[1]);
          else 
            func = (string) call_inf[1];

          catch(printf("%-30s%-20s%5d | %-=20s\n", (string)ob, (string)func, (int)call_inf[2],
              a > 3 ? (string)apply(#'fmt_args,call_inf[3..]) : "--"));
        }

   printf("%-'-'76s\n","");
        printf("\t\t\t%s: %d\n", "Total call_outs", s);

        return 1;
}




