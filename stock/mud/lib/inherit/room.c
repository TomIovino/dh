////////////////////////////////////////////////////////////
// ROOM.C            // CRIMSONESTI // Ash.oct31.98       //
////////////////////////////////////////////////////////////

#include <mudlib.h>
#include <ansi.h>

inherit BASE;

#define TP this_player()


#undef FLUSH_OUT_ADD_OBJECT_PROBLEMS


varargs mixed set_weather(mixed a,mixed b,mixed c,mixed d) {}
varargs void update_daynight_objects(int remove_only);

void env_move(object who,string str);

#define item_wrap(X) desc_wrap(X)
string desc_wrap(string item_desc);

string fix_path(string file) {
    if(!file || !stringp(file))
        raise_error("fix_path():  Bad argument 1!\n");
    if(file[0..7] == "/players") {
        file = "/w" + file[8..];
        log_file("FIXME", sprintf("%s room w/ \"/players\" %O\n",
                     timestamp(), this_object()));
    }
   return file;

}

#include <room.spc>
#include <vowel.h>
#include <exit_con.h>
#include <i_to_str.h>
#include <newline.h>
#include <weather.h>

#define BLIND       (string)this_player()->query_var("blind")
#define TPQN        (string)this_player()->query_name()
#define CAP         (string)this_player()->query_cap_name()
#define REAL        (string)this_player()->query_name(1)

static string door_path;
static string door_name;
static int    door_open;

string *dest_dir;             /* list of exit room|directions              */
static string *extra_objects; /* list of objects that can be taken  */
string *items;                /* list of items that can be looked at       */
static string *night_items;   /* list of items in night_desc        */
static string *day_items;     /* list of items in day_desc          */
static string day_desc;       /* extra description of long in daylight*/
static string night_desc;     /* extra description of long at night   */
static string enter_msg;      /* string written after entering a room */
static string exit_msg;       /* string written before leaving room   */
string type;                  /* element attunement of room */
int no_track;                 /* This prevents location   */
int no_magic;                 /* This will prevent magic  */
int no_fight;                 /* This will prevent fighting */ 
int no_steal;                 /* why do i document this crap */
int no_summon;                /* summoning from a particular room; */
static string custom_exits;   /* "" == no exit list in long */
static int setup;             /* used for init(); */
static int track_time;
static string track_type;    
static string track_dir;
int pk_only;

/***********************************************************************/

void reset(status arg) {
    object player;
    player = this_player();
 
    if(arg) return;
}

string query_object_type() { return "room"; }

string set_type(string s){ type = s; return type; }
string query_type(){ return type; }


int query_track_time(){ return track_time; }
string query_track_type(){ return track_type; }
string query_track_dir(){ return track_dir; }

int query_pk_only(){ return pk_only; }
int set_pk_only(int x){ pk_only = x; }

void set_track_info(string ttype,string tdir,int ttime){
       track_time = ttime;
       track_dir = tdir;
       track_type = ttype; 
} 

void init() {  
    int i, size;  
    string act, tmp;
    ::init();  
    fix_day();        
    update_daynight_objects(1);
  if(pk_only)
     if(this_player()->query_wp() < 1){ this_player()->church(); }
  if(this_player()->query_var("slow")) this_player()->adj_mp(-5);
  if(this_player()->query_var("wings"))
    this_player()->adj_mp(-1);
  else  
    this_player()->adj_mp(-2);
  if(!setup){
    if(door_path)call_other(door_path,"???",0);
    if(!smell_msg)     set_smell("You smell nothing special.\n");  
    if(!listen_msg)    set_listen("You hear nothing special.\n");  
    if(!dest_dir)      dest_dir = ({});
    if(!extra_objects) extra_objects = ({});
    if(!items)         items = ({});
    if(!night_items)   night_items = ({});
    if(!day_items)     day_items = ({});
   }
   if(no_steal)
    add_action("steal","steal");

   if(door_path) {    
        add_action("open_door", "open");
        add_action("close_door","close");
   }

    add_action("scan","scan");

}



int steal(){ write("You may not steal here.\n"); return 1; }
int open_door(string str){
     object other_door;
     if(door_path == 0)      return 0;
     other_door = find_object(door_path);
     if(door_open && str == door_name){
     write(GREY_F+"Its already open.\n"+NORM);
     return 1;
     }
     if(str == door_name){
     write(GREY_F+"You open the "+door_name+".\n"+NORM);      
     add_exit(door_path,door_name);
     other_door->add_exit(
     other_door->query_door_path(),other_door->query_door_name()),
     other_door->set_door_open(1);
     door_open = 1;
     return 1;
     }
     return 0;     
}
int close_door(string str){
     object other_door;
     if(door_path == 0)      return 0;
     other_door = find_object(door_path);
     if(!door_open && str == door_name){
     write(GREY_F+"Its already closed.\n"+NORM);
     return 1;
     }
     if(str == door_name){
     write(GREY_F+"You close the "+door_name+".\n"+NORM);
     remove_exit(door_name);
     other_door->remove_exit(other_door->query_door_name());
     other_door->set_door_open(0);
     door_open = 0;
     return 1;
     }
     return 0;
}

        

void long(status wiz) {  
    int i, day;  
    object obj;  

    if(wiz) {
        write(BOLD+"File name: "+file_name(this_object()) +NORM+"\n");
    }
    ::long(wiz);
    if(query_day()) {  
        if(day_desc) write( (NORM)+process_msg(day_desc));
    }  
    else {  
        if(night_desc) {
            write( (NORM)+process_msg(night_desc));
        }

    }  
}  


/* ids of items in room */

status id(string str) {
    int i, size;

    if(get_id_index(str,items) != -1) {
        return 1;
    }
    else if(get_id_index(str,extra_objects) != -1) {
        return 1;
    }
    else if(query_day() && get_id_index(str,day_items) != -1) {
        return 1;
    }
    else if(!query_day() && get_id_index(str,night_items) != -1) {
        return 1;
    }
    else {
    }
    return 0;  /* nothing in 'room' matching id matching 'str' */
}



/*********************************************************************/
string query_door_name()              { return door_name; }
string query_door_path()              { return door_path; }

string set_door_path(string str)      { return door_path = str; }
string set_door_name(string str)      { return door_name = str; }
int    set_door_open(int x)           { return door_open = x; }
string set_day_desc(string str)       { return day_desc = str; }  
string set_night_desc(string str)     { return night_desc = str; }  
string set_enter_msg(string s)        { return enter_msg = s; }
string set_exit_msg(string s)         { return exit_msg = s; }

string *set_exits(string *arr) {
    int i, j;
    dest_dir = (arr) ? arr : ({});
    if(j = sizeof(dest_dir))
        for(i = 0; i < j; i++)
            if(i % 2 == 0) dest_dir[i] = fix_path(dest_dir[i]);
    update_actions();
    return dest_dir;
}
string *set_extra_objects(string *arr) {
    int i, j;
    extra_objects = arr;
    if(extra_objects && j = sizeof(dest_dir))
        for(i = 0; i < j; i++)
            if(i % 2 == 0) extra_objects[i] = fix_path(extra_objects[i]);
    update_actions();
    return extra_objects;
}

string *set_items(string *arr)        { return items = arr; }
string *set_day_items(string *arr)    { return day_items = arr; }
string *set_night_items(string *arr)  { return night_items = arr; }


int set_no_fight(int i)          { return no_fight = i; }
int set_no_track(int i)          { return no_track = i; }
int set_no_summon(int i)         { return no_summon = i; }
int set_no_magic(int i)          { return no_magic = i;  }
int set_no_steal(int i)          { return no_steal = i; }

/**********************************************************************/
/* queries */


string query_day_desc()       { return day_desc;   }  
string query_night_desc()     { return night_desc; }  
string query_enter_msg()      { return enter_msg; }
string query_exit_msg()       { return exit_msg; }
string *query_dest_dir()      { return dest_dir; }  
string *query_extra_objects() { return extra_objects; }

string *query_items()         { return items; }
string *query_day_items()     { return day_items; }
string *query_night_items()   { return night_items; }


string query_exit_filename(string str) { 
    int i;

    if(!(sizeof(dest_dir) || str)) return 0;  
    if((i = member_array(str,dest_dir)) > -1) return dest_dir[i-1];  
    return 0;  
}  

string query_file_exitname(string file) {  
    int i;  

    if(!file) return 0;
    sscanf(file,"/%",file);
    if((i = member_array(file, dest_dir)) > -1) return dest_dir[i+1];  
    return 0;
}  



string *query_all_exits()    { return dest_dir ; }
string *query_open_exits()   { return dest_dir ; } 
string *query_exits()        { return dest_dir;  }
int query_no_fight()         { return no_fight;  }
int query_no_track()         { return no_track;  }
int query_no_summon()        { return no_summon; }
int query_no_steal()         { return no_steal;  }
int query_no_magic()         { if(present("holy ground")){ return 1; }
                               return no_magic;  }


/************************************************************************/

void exits(status brief) {  
    string str, name, pad;
    int i, size;
    int count;  
    if(custom_exits) {
        write(custom_exits);
        return;
    }


      brief = (status)this_player()->query_env_var("BRIEF");
//    count += 1;
    if(count) write(str +".\n");
    size = sizeof(dest_dir);
    if(!(count || size)) {
        write("\n"); // no obvious exits
        return;    }
    if(!size) return;
   str = "There "+((size == 2) ? "is " : "are ")+int_to_str(size/2)+" obvious";
   str += " exit"+ ((size == 2) ? ": " : "s: ");
   str += BOLD;
    pad = "                                                               ";
    pad = extract(pad,0,strlen(str)-1);   

    for(i = 1; i < size; i += 2) {
        if(str == "") {
            write("\n"+NORM);
            str = pad;
        }
        str += " "+ ((brief) ? convert_to_brief(dest_dir[i]) : dest_dir[i]);
        str += (i == size - 1) ? "."+NORM : ((i == size - 2) ? " and" : ",");
        if(strlen(str) > 70) {
            write(str);
            str = ""+NORM;
        } 
    }
    printf("%s\n",str);
} 


/**************************************************************************/
/* get ready to move */


status ready_to_move(object who,string str) {   
    string direction, func;   
    int i;   

//    write("requested\n");
    direction = str;
    direction = convert_exit_name(direction);   
    if((i = member_array(direction,dest_dir)) == -1) return 0;
    if(sscanf(dest_dir[i-1],"@%s",func) == 1) 
     { /* requested by tamsyn */
        return (status)call_other(this_object(),func,direction);
     }
    str = direction +"#"+ dest_dir[i-1];

    remove_call_out("env_move");
    if(who->query_position() != "standing" &&
       who->query_position() != "sneaking" &&
       who->query_position() != "flying"){
        write("Maybe you should stand first.\n");
        return 1;
    }
    if(who->query_visibility() == "hidden"){
       who->set_visibility("visibile");
    }
    who->set_hiding(0);
    if(who->query_sneaking()){   
        write("You sneak "+ direction +"...\n");   
    }   
    if(who->query_position() == "flying"){   
        write("You fly "+ direction +"...\n");   
    } 
    set_track_info(who->query_race(),direction,time());
    if(who->query_primary_attack()){
        write("You attempt to leave combat "+ direction +"...\n");   
        env_move(who,str);
        return 1;
    } 
   // write("room_move");  
    env_move(who,str);   
    return 1;   
} 


/****************************************************************************/
/* move player */

void env_move(object who,string str){ room_move(str); }

void room_move(string str) {
    string tmp_msg;

     if(this_player()->query_dead()){
        this_player()->church();
        return;
     }
    if(this_player()->query_dead()) return;
    if(this_player()->query_var("stun")){
        return;
    }
    if(this_player()->query_mp() < 1){
        write("You are EXAUSTED!\n");
        return;
    }
    if(!this_player()->move_player(str)) return; /* failed to move */
                   
    if(exit_msg) write(process_msg(exit_msg));
    if((tmp_msg = (string)environment(this_player())->query_enter_msg()))
     {
     write(process_msg(tmp_msg));
     }
}


/***************************************************************************/
/* look direction */

status look_dir(string dir) {
    int i;
    status wiz;
    object it;

    if(!dir) return 0;
    dir = convert_exit_name(dir);
    if((i = member_array(dir, dest_dir)) == -1) return 0; /* failure */
    wiz = (this_player()->query_security_level()) ? 1 : 0;
    call_other(dest_dir[i-1],"long",wiz);
    return 1;
}
status scan(string dir) {
    int i;
    status wiz;
    object it;

    if(!dir){ write("Cant see anything that direction.\n"); return 1; }
    dir = convert_exit_name(dir);
    if((i = member_array(dir, dest_dir)) == -1)
      { write("Cant see anything that direction.\n"); return 1; }
//    wiz = (this_player()->query_security_level()) ? 1 : 0;
    it = clone_object("inherit/objs/scan");
  if(this_player()->test_dark()){ return 1; }
    it->scan(this_player());
    move_object(it,dest_dir[i-1]);
    it->new_room("look");
    return 1;
}


/************************************************************************/
/* dynamic exits */   


status add_exit(string where, string dest) {  
    int i;  
    if(!dest_dir) dest_dir = ({});
    if((i = member_array(dest, dest_dir)) > -1) return 0; /* failure */
    dest_dir += ({ where, dest });  
    update_actions();
    return 1;  
}  


status remove_exit(string dest) {  
    int i, size;  

    if((i=member_array(dest,dest_dir)) < 0) return 0; /* failure */
    if((size = sizeof(dest_dir)) == 2)
        dest_dir = ({});
    else
        dest_dir = dest_dir[0..i-2] + dest_dir[i+1..size];  
    update_actions();
    return 1;
}


status change_exit_room(string old_dest,string new_dest) {  
    int i;  

    if((i=member_array(old_dest,dest_dir)) == -1) return 0;
///////// !! Compatibility Mode !! //////////
          fix_path(&new_dest);
///////// !! Compatibility Mode !! //////////
    dest_dir[i] = new_dest;  
    update_actions();
    return 1;  
}  


/**********************************************************************/
/* dynamic items */


varargs status add_item(string id_item, string item_str, mixed type) {
    int i;
    string str;
    str = type +"";
    switch(str) {
    case "1": case "day":
        if(!day_items) day_items = ({});
        if((i = member_array(id_item, day_items)) != -1) return 0;
        day_items += ({ id_item, item_str, });
        break;

    case "2": case "night":
        if(!night_items) night_items = ({});
        if((i = member_array(id_item, night_items)) != -1) return 0;
        night_items += ({ id_item, item_str, });
        break;

    default:
        if(!items) items = ({});
        if((i = member_array(id_item, items)) != -1) return 0;
        items += ({ id_item, item_str, });
        break;
    }
    return 1;
}


varargs status remove_item(string id_item, mixed type) {
    int i, size;  
    string str;

    str = type +"";
    switch(str) {
    case "1": case "day":
        if(!day_items) day_items = ({});
        if((i=get_id_index(id_item, day_items)) == -1) return 0;
        if((size = sizeof(day_items)) == 2)
            day_items = ({});
        else
            day_items = day_items[0..i-1]+ day_items[i+2..size];  
        break;

    case "2": case "night":
        if(!night_items) night_items = ({});
        if((i=get_id_index(id_item, night_items)) == -1) return 0;
        if((size = sizeof(night_items)) == 2)
            night_items = ({});
        else
            night_items = night_items[0..i-1]+ night_items[i+2..size];  
        break;

    default:
        if(!items) items = ({});
        if((i=get_id_index(id_item, items)) == -1) return 0;
        if((size = sizeof(items)) == 2)
            items = ({});
        else
            items = items[0..i-1]+ items[i+2..size];  
        break;
    }
    return 1;
}


varargs status change_item(string id_item, string item_str, mixed type) {
    int i;
    string str;

    str = type +"";
    switch(str) {
    case "1": case "day":
        if(!day_items) day_items = ({});
        if((i = get_id_index(id_item, day_items)) == -1) return 0;
        day_items[i+1] = item_str;
        break;

    case "2": case "night":
        if(!night_items) night_items = ({});
        if((i = get_id_index(id_item, night_items)) == -1) return 0;
        night_items[i+1] = item_str;
        break;

    default:
        if(!items) items = ({});
        if((i = get_id_index(id_item, items)) == -1) return 0;
        items[i+1] = item_str;
        break;
    }
    return 1;
}


/**********************************************************************/
/* extra objects */


status add_extra_object(string id_extra_object, string file_extra_object) {
    int i;

    if(!extra_objects) extra_objects = ({});
///////// !! Compatibility Mode !! //////////
          fix_path(&file_extra_object);
///////// !! Compatibility Mode !! //////////
    if((i = member_array(id_extra_object,extra_objects)) != -1) return 0;
    extra_objects += ({ id_extra_object, file_extra_object, });
    return 1;
}


status remove_extra_object(string id_extra_object) {
    int i, size;  

    if((i=member_array(id_extra_object,extra_objects)) == -1) return 0;
    if((size = sizeof(extra_objects)) == 2)
        extra_objects = ({});
    else
        extra_objects = extra_objects[0..i-1]+extra_objects[i+2..size];  
    return 1;
}


status get_extra_object(string id_extra_object) {
    int i;
    object ob;  

    if(!id_extra_object) {
        write("Usage: get <item>\n");
        return 1;
    }
    if(present(id_extra_object,this_object())) return 0;
    if((i = get_id_index(id_extra_object,extra_objects)) == -1) return 0;
    ob = clone_object(extra_objects[i+1]);
    if(!this_player()->add_weight((int)ob->query_weight())) {
        write(capitalize(id_extra_object)+" is too heavy for you.\n");
        destruct(ob);
        return 1;
    }
#ifdef NATIVE_MODE
    ob->move(this_player());
#else
    move_object(ob, this_player());
#endif /* NATIVE_MODE */
    write("You get the "+capitalize(id_extra_object)+".\n");
    say(this_player()->query_name()+" gets a "+id_extra_object+".\n");
    return 1;
}


/****************************************************************************/
/* item_look */

status look_at(string str) {
    int i, size;
    string lock;

    if(!str) return 0;

    /* look thru items list */

    if((i = get_id_index(str,items)) != -1) {
        items[i+1] = item_wrap(items[i+1]);
        write(process_msg(add_newline(items[i+1])));
        return 1;
    }

    /* look thru day items list */

    if(query_day() && (i = get_id_index(str,day_items)) != -1) {
        items[i+1] = item_wrap(items[i+1]);
        write(process_msg(add_newline(day_items[i+1])));
        return 1;
    }

    /* look thru night items list */

    if(!query_day() && (i = get_id_index(str,night_items)) != -1) {
        items[i+1] = item_wrap(items[i+1]);
        write(process_msg(add_newline(night_items[i+1])));
        return 1;
    }

    /* look thru extra objects */

    else if((i = get_id_index(str,extra_objects)) != -1) {
        call_other(extra_objects[i+1],"long");
        return 1;
    }
    /* look in a direction */

    else if(look_dir(str)) {
        return 1;
    }

    return 0;  /* nothing in 'room' matching id matching 'str' */
}


/***************************************************************************/
/* here */

status ob_here(string str) {
    if(!str) return 0;
    return (present(str) || present(str,this_player())) ? 1 : 0;
}

/* experiment code for clean up */

// int clean_up( int arg)
// {
//         if (!first_inventory(this_object()) && arg < 2){
//                 log_file("clean_up",file_name(this_object())+"\n");
//                if(find_player("cyberspam"))
//                 tell_object(find_living("cyberspam"),
//                         "Clean_up: " + file_name(this_object()) + "\n");
//                 destruct(this_object());
//         }
// }

static int Unique = 0;
int set_no_clean(int x) { if(!Unique && x) Unique = 1; }

int clean_up(int refcount) {
    object *tmp;
    int i;

    if(Unique)
        return 0;

    if(refcount > 1)
        return 0;
    tmp = filter_array(deep_inventory(this_object()),#'interactive);
//    log_file("CLEANUP",sprintf("%s %O %d %d\n",timestamp(),this_object(),refcount,sizeof(tmp)));
    if(sizeof(tmp)) {
        return 0;
    }
    i = sizeof(tmp = deep_inventory(this_object()));
    while(i--)
        destruct(tmp[i]);
    
//    log_file("CLEANUP","\tAll inventory destructed.\n");
    i = 0;
    catch(i = this_object()->is_storeroom());
    if(!i) {
//        log_file("CLEANUP","\tRoom destructed.\n");
        destruct(this_object());
    }
    return 1;
}


  void do_clean_up(){
    if(!present(this_player())){
        rto();
    }
}
/***************************************************************************/
/* get index */

int get_id_index(string str,string *arr) {
    int i;
    string *ids;

    if(!arr) return -1;
    for(i = 0; i < sizeof(arr); i += 2) {
        if(!arr[i]) continue;
#ifdef OLD_EXPLODE
        ids = explode(arr[i]+"#", "#");
#else
        ids = explode(arr[i], "#");
#endif
        if(member_array(str,ids) != -1) return i;
    }
    return -1;
}     

/****************************************************************************/
/* set_sign */

varargs void set_sign_message(string str, string lang) {
    object obj;

    if(!str) return;
    obj = clone_object("inherit/sign");
    obj->set_message(str);
    if(lang) obj->set_language(lang);
#ifdef NATIVE_MODE
    obj->move(this_object());
#else
    move_object(obj, this_object());
#endif /* NATIVE_MODE */
}
string query_name() { return file_name(this_object()); }


/* Original add_object functions written by Ash@dragonheart
 * Rewritten entirely by Geewhiz@DragonHeart
 */

private static mapping AddObjs = ([ ]);
private static mapping DayAddObjs = ([ ]);
private static mapping NightAddObjs = ([ ]);

mapping  query_AO() { return copy_mapping(AddObjs); }
mapping query_DAO() { return copy_mapping(DayAddObjs); }
mapping query_NAO() { return copy_mapping(NightAddObjs); }

string objKey(string name, string path) { return name; }

private object objExists(string key) { return AddObjs[key]; }


varargs
void __add_object(string path, string key, int unique
#ifdef FLUSH_OUT_ADD_OBJECT_PROBLEMS 
                                                     , int do_it
#endif
                                                                ) {
    object obj;

#ifdef FLUSH_OUT_ADD_OBJECT_PROBLEMS
    if(!do_it) {
        call_out("__add_object",0,path,key,unique,1);
        return;
    } else {
#endif
        fix_path(&path);
#ifdef FLUSH_OUT_ADD_OBJECT_PROBLEMS
        obj = efun::clone_object(path);
#else
        obj = clone_object(path);
#endif
        move_object(obj,this_object());
        AddObjs[key] = unique ? 1 : obj;

#ifdef FLUSH_OUT_ADD_OBJECT_PROBLEMS
    }
#endif
}


varargs void add_object(string name, string path, int unique, int guard){
    object obj;
    string key, fileID;

    key = objKey(name,path);
    if(unique)
        Unique = 1;

    if(!objExists(key))
        __add_object(path,key,unique);
}

varargs void update_daynight_objects(int remove_only) {
    string *a_keys, *r_keys;
    mapping add;
    int i, ret;
    object ob;
    if((mud_daynight() - 2) < 0) { // Day
        a_keys = m_indices(add = DayAddObjs);
        r_keys = m_indices(NightAddObjs);
    } else { // Night
        a_keys = m_indices(add = NightAddObjs);
        r_keys = m_indices(DayAddObjs);
    }
    
    if(i = sizeof(r_keys)) {
        while(i--) {
            if(mapping_contains(&ob,AddObjs,r_keys[i])) {
                if(intp(ob))
                    continue;
                if(catch(ret = ob->allow_daynight_change()) 
                || ret)
                    continue;
                move_object(ob,"/secure/void"); // void will dest mob and eq
                AddObjs = m_delete(AddObjs,r_keys[i]);
            }
        }
    }

    if(remove_only)
        return;
        
    if(i = sizeof(a_keys))
        while(i--)
            add_object(a_keys[i],add[a_keys[i],0],add[a_keys[i],1]);
}
    
varargs void add_day_object(string name, string path, int unique) {
    string key;
    key = "D" + objKey(name,path);
    DayAddObjs += ([ key : path; unique ]);
    update_daynight_objects();
}

varargs void add_night_object(string name, string path, int unique) {
    string key;
    key = "N" + objKey(name,path);
    NightAddObjs += ([ key : path; unique ]);
    update_daynight_objects();
}

/***************************************************************************/
/* Added by Tasslehoff */

int notdir() { return write("You can't go that way.\n"),1; }
int no_enter() { return write("Enter what?\n"),1; }

/***************************************************************************/

void set_custom_exits(string msg) { custom_exits = msg; }

no_search(str){
    if(!str){
        write("You find nothing of interest there.\n");
        tell_room(environment(TP), TPQN+" searches around.\n",({ TP }));
        return 1;}
    write("You find nothing of interest in the "+str+".\n");
    say(TPQN+" searches the "+str+".\n", TP);
    return 1; }


varargs string format(string str, int indent, int shift, int width) {
    int extra,pre;
    if(!str || !stringp(str))
        raise_error("format(): Bad arg 1 (need string)\n");
    if(str == "")
        return str;
    while(str[<1] == '\n')
        str = str[0..<2];
    if(!indent || indent < 0)
        indent = 0;
    if(!width || width < 0)
        width = 75;
    if(shift < 0) {
        pre = -1 * shift;
        extra = 0;
    } else if(shift == 0) {
        pre = 0;
        extra = 0;
    } else { // shift > 0
        pre = 0;
        extra = shift;
    }
    if(extra) str = sprintf("%*s",extra,"") + str;
    return sprintf("%*s%-*=s\n",indent,str[0..pre-1],width-indent,str[pre..]);
}
 
varargs string wrap(string str, int width, int indent) {
    if(!width) width = 78;
    if(!indent) indent = 0;
    return format(str,indent,(-1*indent),width);
}

int desc_width = 0;
int desc_indent = 0;

void set_desc_format(int w, int i) {
    desc_width = w;
    desc_indent = i;
}

string desc_wrap(string item_desc) {
    int pos, last;
    last = strlen(item_desc) - 1;
    pos = member(item_desc,'\n');
    if(pos == last || pos == -1) {
        item_desc = wrap(item_desc,desc_width ? desc_width : 65, desc_indent);
    }
    return item_desc;    
}    



string query_rcs_id() { return "$Id: room.c,v 1.9 1998/07/21 08:08:18 atgraham Exp atgraham $"; }

