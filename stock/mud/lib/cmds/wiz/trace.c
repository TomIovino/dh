int tracemask;
 
void reset() { tracemask = 0; }
 
int main(string arg) {
  int m;
 
  if(!arg) {
    write("Trace all function calls to lfuns?  Y/N  ");
    input_to("trace_one");
    return 1;
  }
  sscanf(arg,"%d",m);
  trace(m);
  return 1;
}
 
trace_one(string ans){
  if(ans=="") { write("Aborting....\n"); return 1; }
  ans = lower_case(ans);
  if(ans[0] == 'y') { tracemask = 1; } else { tracemask = 0; }
  write("Trace all calls to 'call_other'?  Y/N  ");
  input_to("trace_two");
  return 1;
}
 
trace_two(string ans){
  if(ans=="") { write("Aborting....\n"); return 1; }
  ans = lower_case(ans);
  if(ans[0] == 'y') { tracemask += 2; }
  write("Trace all function returns?  Y/N  ");
  input_to("trace_three");
  return 1;
}
 
trace_three(string ans){
  if(ans=="") { write("Aborting....\n"); return 1; }
  ans = lower_case(ans);
  if(ans[0] == 'y') { tracemask += 4; }
  write("Print arguments at function calls and return values?  Y/N  ");
  input_to("trace_four");
  return 1;
}
 
trace_four(string ans){
  if(ans=="") { write("Aborting....\n"); return 1; }
  ans = lower_case(ans);
  if(ans[0] == 'y') { tracemask += 8; }
  write("Print all executed stack machine instructions?\n(This produces a lot of output!)  Y/N  ");
  input_to("trace_five");
  return 1;
}
 
trace_five(string ans){
  if(ans=="") { write("Aborting....\n"); return 1; }
  ans = lower_case(ans);
  if(ans[0] == 'y') { tracemask += 16; }
  write("Enable trace in heart beat functions?  Y/N  ");
  input_to("trace_six");
  return 1;
}
 
trace_six(string ans){
  if(ans=="") { write("Aborting....\n"); return 1; }
  ans = lower_case(ans);
  if(ans[0] == 'y') { tracemask += 32; }
  write("Trace calls to apply?  Y/N  ");
  input_to("trace_seven");
  return 1;
}
 
trace_seven(string ans){
  if(ans=="") { write("Aborting....\n"); return 1; }
  ans = lower_case(ans);
  if(ans[0] == 'y') { tracemask += 64; }
  write("Show object name in tracing?  Y/N  ");
  input_to("trace_eight");
  return 1;
}
 
trace_eight(string ans){
  if(ans=="") { write("Aborting....\n"); return 1; }
  ans = lower_case(ans);
  if(ans[0] == 'y') { tracemask += 128; }
  write("Setting trace mask to "+tracemask+"\n");
  trace(tracemask);
  tracemask = 0;
  return 1;
}
