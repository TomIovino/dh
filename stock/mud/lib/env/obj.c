// UBER OBJ 
// ASH 2001 

        mapping data                    = ([ ]);
static  mapping tmp_data                = ([ ]);
varargs mixed query_data(string var);
mapping query_all_data();
int     query_total_data();
status  set_data(string var, mixed setting);
status  add_data(string var, mixed setting);
status  remove_data(string var);
void    remove_all_data();
void    printdata();

void printdata(){ save_object("/log/print"); }
void reset(status arg) 
{
  if(arg) return;
  set_data("name",  "object" );
  set_data("short", "small object");
  set_data("long",  "A small object.");
  set_data("weight", 1);
  set_data("value",  1);
}     
status id(string str)
{
        if(str == query_data("name")) return 1; 
} 
varargs mixed query_data(string var)
{ 

        return data[var]; 
}
mapping query_all_data()
{ 
        return copy_mapping(data) +
               copy_mapping(tmp_data);      
}
int     query_total_data()
{  
        return sizeof(m_indices(data)); 
}
status  set_data(string var,mixed setting) 
{
        data[var] = setting;
        return 1;
}
status  add_data(string var, mixed setting) 
{
        data[var] += setting;
        return 1;
}
status  remove_data(string var)
{
        data = m_delete(data,var);
        return 1;
}
void    remove_all_data()
{
        data = ([]);                    
        tmp_data = ([]);
}


