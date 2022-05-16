// Purge Function, Ash@dragonheart (sometime in 1995)
// once in a while its needed to update users when they log in
// to either remove gear, or whatever. this function is called
// every log in.
#define TP this_player()
int purge;

status purge(){
              if(purge<1){
                 TP-> set_money(0);
                 TP-> set_savings(0);
                 TP-> clear_inventory();
                 TP-> clear_backup_inv();
}
