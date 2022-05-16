string www_main(string *args)
{
    string data, *info;

    info="/cmds/std/_who"->get_who_string();
    data=sprintf(
"Content-type:  text/html\n\n"
"<TITLE>DragonHeart Who List</TITLE>\n"
"<BODY>\n"
"<H1 align=center>DragonHeart Who List</H1>\n"
"\n"
"There are %d people currently logged in.\n"
"<PRE><HR>\n"
"%s\n"
"<HR></PRE>\n"
"</BODY>\n",
    sizeof(users()), info);
    return (string) "/secure/ColorD/color_d"->color(data,0);    
}
    
status clean_up(status arg) { destruct(this_object()); }

