#pragma strict_types
#pragma save_types
#pragma combine_strings
#pragma verbose_errors

// Environment Variables Player Inherit File -- v1.00e
// Geewhiz@DoD, @Dreamtime, @DragonHeart (96-01-10)
//
 
#define wizardp(WHO)                ( ( (WHO) -> query_security() ) >= 10 )
 
        mapping environment_variables = ([ ]);
        mapping perm_attrib = ([ ]);
static  mapping tmp_attrib = ([ ]);     // Temporary ! Not saved ! 
 
        mapping query_env();
varargs mixed   query_wiz_env_var(string var);
        status  set_wiz_env_var(string var, mixed setting);
        status  unset_wiz_env_var(string var);
varargs mixed   query_env_var(string var);
        status  set_env_var(string var, mixed setting);
        status  unset_env_var(string var);
        status  defined_env_var(string var);
        status  defined_wiz_env_var(string var);
        status  set_tmp_attrib(string var, mixed setting);
        status  unset_tmp_attrib(string var);
        status  set_attrib(string var, mixed setting);
        status  unset_attrib(string var);
varargs mixed   query_attrib(string var);
 
mapping query_env() { return copy_mapping(environment_variables); }
 
varargs mixed query_wiz_env_var(string var) { 
    if(wizardp(this_object()))
        return query_env_var(var);
    return 0;
}
 
status set_wiz_env_var(string var, mixed set) { return set_env_var(var,set); }
 
status unset_wiz_env_var(string var) { return unset_env_var(var); }
 
varargs mixed query_env_var(string var) {
    if(!var) return copy_mapping(environment_variables);
    return environment_variables[var];
}
 
status set_env_var(string var, mixed setting) {
    environment_variables[var] = setting;
    return 1;
}
 
status unset_env_var(string var) {
    environment_variables = m_delete(environment_variables,var);
    return 1;
}
 
status defined_env_var(string var) {
    return member(environment_variables,var);
}
 
status defined_wiz_env_var(string var) { return defined_env_var(var); }
 
varargs status set_tmp_attrib(string var, mixed setting) {
    if(!setting) setting = 1;
    tmp_attrib[var] = setting;
    return 1;
}
 
status unset_tmp_attrib(string var) {
    tmp_attrib = m_delete(tmp_attrib,var);
    return 1;
}
 
status set_attrib(string var, mixed setting) {
    perm_attrib[var] = setting;
    return 1;
}
 
status unset_attrib(string var) {
    perm_attrib = m_delete(perm_attrib,var);
    return 1;
}
 
status clear_attrib(string var) {
    if(!member(tmp_attrib,var))
        unset_attrib(var);
    else
        unset_tmp_attrib(var);
    return 1;
}
 
varargs mixed query_attrib(string var) {
    mixed ret;
    if(!var) return copy_mapping(perm_attrib) + copy_mapping(tmp_attrib);
    ret = tmp_attrib[var];
    if(!ret) ret = perm_attrib[var];
    return ret;
}
