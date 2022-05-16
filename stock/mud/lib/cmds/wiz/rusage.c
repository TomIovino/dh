status main() {
   int *ru;
   ru = rusage();
   printf("%10d    utime (user time)\n",ru[0]);
   printf("%10d    stime (system time)\n",ru[1]);
   printf("%10d  inblock (block input)\n",ru[9]);
   printf("%10d outblock (block output)\n",ru[10]);
   printf("%10d   msgsnd (messages sent)\n",ru[11]);
   printf("%10d   msgrcv (messages received)\n",ru[12]);
   printf("%10d nsignals (signals received)\n",ru[13]);
   printf("%10d    nvcsw (voluntary context switches)\n",ru[14]);
   printf("%10d   nivcsw (involuntary context switches)\n",ru[15]);
   return 1;
}
