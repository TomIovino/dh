 
inherit "inherit/treasure";
 
#define TP this_player()
#define QN query_name()
 
static string *whose;
 
reset(arg){
        if(arg) return;
  
        set_name("stool");
        set_alias("stools");
        set_long(""+
"A set of wooden bar stools. They look comfortable enough to get drunk on.\n");  
        whose = ({});
        }
 
init(){
        ::init();
        removeplayer(this_player()->query_name());
        checkwhose();
        add_action("look", "look");
        add_action("look", "l");
        add_action("look", "exa");
        add_action("look", "examine");
        add_action("take", "get");
        add_action("take", "take");
        add_action("stand", "stand");
        add_action("sit", "sit");
        }
 
query_whose(){ return whose; }
 
take(str){
  object ob;    object *obs;     int i;
 
  if(str == "all"){
        obs = all_inventory(environment(this_object()));
        for(i = 0; i < sizeof(obs); i++){
                if(obs[i]->query_name() != this_object()->query_name()){
                        command("take "+obs[i]->query_name(), this_player());
                        }
                } return 1; }
 
        ob = present(lower_case((string)str), environment(this_player()));
        if(ob == this_object()){
                write("The stool is far to heavy for you to lift!\n");
                return 1; }
        return 0; }
 
inarray(str){
        int i;
        for(i = 0; i < sizeof(whose); i++){
        if(lower_case((string)str) == lower_case((string)whose[i])){ 
                return i; }
                }
        return -1; }
 
checkwhose(){
        int i;
        for(i =0; i < sizeof(whose); i++){
                if(!present(whose[i], environment(this_object()))){
                        removeplayer(whose[i]); }
                        }
        return 1; }
 
removeplayer(str){
        int i;
        if((i = inarray(str)) == -1){ 
                return -1; }
        whose = whose[0..i-1]+whose[i+1..sizeof(whose)-1];
        TP->set_alt_extra_long();
        return i;
        }
 
sit(str){
        if(str != "on stool" && str != "on stools" && str != "stool"){ 
                return 0; }
        if(inarray(TP->QN) != -1){
                write("You are already on a stool.\n");
                return 1; }
        write("You sit down on a wooden stool.\n");
        say(TP->QN+" sits on one of the wooden bar stools.\n", TP);
        whose += ({ TP->QN, });
        TP->set_alt_extra_long(TP->QN+" is sitting on a wooden bar stool.\n");
        return 1; 
        }
 
stand(str){
          if(str == "stool" || str == 0 || str == "up" || str == "stools"){
                if(inarray(TP->QN) == -1){ 
                        write("You are not on a stool.\n");
                        return 1; }
                removeplayer(TP->QN);
                write("You stand up from the bar stool.\n");
                say(TP->QN+" stands up from a wooden bar stool.\n", TP);
                return 1; }
        return 0; }
 
look(str){
        int i;
        if(str == "stool" || str == "stools"){
                checkwhose();
                write(this_object()->long());
                if(sizeof(whose) == 0){
                        write("No one is sitting on the stools.\n");
                        return 1; }
                for(i = 0; i < sizeof(whose); i++){
                        write(whose[i]);
                        write(" is sitting on a stool.\n"); 
                        }
                return 1; }
        return 0; }
