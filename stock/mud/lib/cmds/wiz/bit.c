// USED in sould.c 
#define FLAG(B,FLAGS) ((FLAGS) & (1<<B))
 
 
string dump_bits(int num) {
    int i;
    string ret;
    ret = "";
    i = 32;
    while(i--) {
        ret += sprintf("%d%c",FLAG(i,num) ? 1 : 0, i%8==0 ? ' ' : 0);
    }
    return ret;
}
 
status main(string arg) {
    int num, i;
    if(!arg || arg == "" || sscanf(arg,"%d",num) != 1) {
        return notify_fail("bit <number>\n");
    }
    printf("%d ==> %s\n",num,dump_bits(num));
    return 1;
}
 
void reset(int arg) { if(!arg) printf("Yes! " __FILE__ " loaded.\n"); }
void notify_destruct() { printf("Aieeeeee! " __FILE__ " killed.\n"); }
 
