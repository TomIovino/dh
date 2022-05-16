status main(string arg) {
   string *ed_info;
   int i, j;
   ed_info = (string *) __MASTER_OBJECT__->query_current_edit();
   j = sizeof(ed_info);
   if(!j) return printf("No ed users.\n"),1;
   printf("%|79s\n","Ed Users");
   printf("%|20s : %s\n","Name","File");
   for(i = 0; i < j; i += 2) {
       printf("%-20s : %s\n",ed_info[i+1],ed_info[i]);
   }
   return 1;
}
