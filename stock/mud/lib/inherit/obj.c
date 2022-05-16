// UBER OBJ 
// ASH 2001 

#include <ansi.h>

        mapping data                    = ([ ]);
static  mapping tmp_data                = ([ ]);

mapping query_all_data(){ 
        return copy_mapping(data);      
}

void    reset_all_data(){
        data = ([]);                    
}

int     query_total_data(){  
        return sizeof(m_indices(data)); 
}

varargs mixed query_data(string var){ 
        return data[var]; 
}

status  set_data(string var, mixed setting) {
        data[var] = setting;
        return 1;
}

status  adj_data(string var, mixed setting) {
        data[var] += setting;
        return 1;
}


status  unset_data(string var){
        data = m_delete(data,var);
        return 1;
}


