

int main(string arg) {
    object *u;
    mapping ss;
    int s, i;
    i = sizeof(u = users());
    while(i--) {
         command("mortal ;"+arg+" "+u[i]->query_name(),this_player());
        }
    return 1;
}

