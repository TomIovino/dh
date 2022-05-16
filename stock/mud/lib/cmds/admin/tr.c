status main(string dir) {
     string *files;
     object ob, *loaded;
     int i;
     if(!dir) dir = this_player()->make_path("");
printf("dir = %O\n",dir);
     loaded = ({ });
     files = get_dir(dir+"/*.c");
printf("files = %-=65O\n",files);
     i = sizeof(files);
     while(i--)
        if(ob = find_object(dir+"/"+files[i])) {
           printf("%O\n",ob);
           loaded += ({ ob });
        }
     i = sizeof(loaded);
     while(i--) call_out("Time",i*2,loaded[i]);
      return 1;
}
 
#define TIMER "/cmds/wiz/_Time"
void Time(object o) {
    printf("\n============================================================\n");
    printf("\tTimeing   %O->reset(1);\n",o);
    TIMER->timer_on();
    o->reset(1);
    TIMER->timer_off(0);
    TIMER->timer_results();
}
