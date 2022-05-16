string get_col(int i) {
    string ret;
    switch(i) {
        case 30: ret = "Black"; break;
        case 31: ret = "Red"; break;
        case 32: ret = "Green"; break;
        case 33: ret = "Yellow"; break;
        case 34: ret = "Blue"; break;
        case 35: ret = "Magenta"; break;
        case 36: ret = "Cyan"; break;
        case 37: ret = "White"; break;
    }
    return ret;
}
 
status main() {
   int i;
 
   printf("\tColor Swatch Generator\n");
   printf(" Color   Ansi Code\tAnsi Code       Color\n");
   for(i=30; i < 38; i++) {
      printf("%c[0;%dm%7s  ESC[0;%dm%c[0m\t%c[0;1;%dmESC[0;1;%dm  Bold %-7s%c[0m\n",
             27,i,get_col(i),i,27,27,i,i,get_col(i),27);
   }
   printf("\n** NOTE:  An easy way to get ESC is:\n\t\t#define ESC (printf(\"%%c\",27))\n");
   return 1;
}
