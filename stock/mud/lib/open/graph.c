/*
  Info for a importer to another mud...
  I WANT my name in the long() !! thats the only thing i want for this
  set yourself as importer or somthing..

  reset_graph() resets the graph to zero values ..... if it bugged

  p.s. change the paths and stuff in each file...
*/

#include "/secure/security.h"
#define TIME_ZONE 19/* EET */
#define DEST "room/root/pub2"
#define Sv_Obj "data/graph"
static int users,wizes,times,users_h,times_h,last_rec;

int u_0 ,u_1 ,u_2 ,u_3 ,u_4 ,u_5 ,u_6,
  u_7 ,u_8 ,u_9 ,u_10,u_11,u_12,
  u_13,u_14,u_15,u_16,u_17,u_18,
  u_19,u_20,u_21,u_22,u_23;

int t_0 ,t_1 ,t_2 ,t_3 ,t_4 ,t_5 ,t_6,
  t_7 ,t_8 ,t_9 ,t_10,t_11,t_12,
  t_13,t_14,t_15,t_16,t_17,t_18,
  t_19,t_20,t_21,t_22,t_23;

int w_0 ,w_1 ,w_2 ,w_3 ,w_4 ,w_5 ,w_6,
  w_7 ,w_8 ,w_9 ,w_10,w_11,w_12,
  w_13,w_14,w_15,w_16,w_17,w_18,
  w_19,w_20,w_21,w_22,w_23;

id(str) {
  return str == "graph" || str == "user graph";
}

short()
{
  return"A user graph hanging on the wall";
}

long()
{
  int i,j,k,w,wiz_a,mor_a,mo,wz;
  string l;

  wiz_a=0;mor_a=0;
  l = "User graph by Draconian of Genesis. Patches by Animal & Galahad.\n"+
  "W= Wizard m= mortal. The time is now "+my_time()+"\n" +
  "Time                                  Users\n";
  l += "                                                    11111111111 Users, \
n" +
       "   000011111222223333344444555556666677777888889999900000111112 Wizards\
n" +
       "   246802468024680246802468024680246802468024680246802468024680 =======\
n";

  for (j=0;j<24;j++)
    {
      if (j/10 == 0)
        l += " ";
      l += j + ":";
      k=0; w=0;
      if(times[j])
        {
          k=((users[j]/times[j])/2);
          w=((wizes[j]/times[j])/2);
          mo=(users[j]/times[j]);
          wz=(wizes[j]/times[j]);
        }
      wiz_a+=wz;mor_a+=mo;
      for(i=0;i< w;i++)
                l += "W";
      for(i=w;i< k;i++)
                l += "m";
      for(i=0;i< (60-k);i++)
                l += " ";
      l += ":";
      if(times[j])
        {
  if(mo/10 == 0)
    l += " ";
    l += (mo)+",";
  if(wz/10 == 0)
    l += " ";
    l += (wz)+"\n";
        }
      else l += "Nada.\n";
    }
  wiz_a=wiz_a/24;mor_a=mor_a/24;
  l += "Average W:"+wiz_a+" m:"+mor_a+
        " Total:"+(wiz_a+mor_a)+" (all times in EST)\n";
  if(!this_player()->query_invis())
  say(this_player() ->query_name() + " studies the user graph\n");
  write(l);
}

this_h()
{
  return ((time()/60/60)+TIME_ZONE+1)%24;
}

my_time()
{
  string a;
  int b;
  a=this_h();
  b=(time()/60)%60;
  if (this_h()<10)
    a=" "+a;
  if(b<10)
    return a+":0"+b;
  else
    return a+":"+b;
}

reset(str)
{
  if(str)
    return;
  move_object(this_object(), DEST);
  users=allocate(24);
  times=allocate(24);
  wizes=allocate(24);
  restore_h();
  last_rec= this_h();
  users_h = users[last_rec];
  times_h = times[last_rec];
  call_out("hb",10);
  call_out("save_h",900);
}

hb()
{
  object list;
  int i;
  list = users();
  users_h += sizeof(list);
  times_h += 1;

  users[last_rec] = users_h;
  times[last_rec] = times_h;

  i=0;
  while(i<sizeof(list))
    {
      int level;
      level = list[i]->query_level();
      if (level >= EXPLORE)
        wizes[last_rec] += 1;
        i += 1;
    }

  last_rec = this_h();
  users_h = users[this_h()];
  times_h = times[this_h()];
  call_out("hb",10);
}

restore_h() {
  restore_object(Sv_Obj);
  users[0]=u_0;   users[1]=u_1;
  users[2]=u_2;   users[3]=u_3;   users[4]=u_4;   users[5]=u_5; users[6]=u_6;
  users[7]=u_7;   users[8]=u_8;   users[9]=u_9;   users[10]=u_10;
  users[11]=u_11; users[12]=u_12; users[13]=u_13; users[14]=u_14;
  users[15]=u_15; users[16]=u_16; users[17]=u_17; users[18]=u_18;
  users[19]=u_19; users[20]=u_20; users[21]=u_21; users[22]=u_22;
  users[23]=u_23; times[0]=t_0;   times[1]=t_1;   times[2]=t_2;
  times[3]=t_3;   times[4]=t_4;   times[5]=t_5;
  times[6]=t_6; times[7]=t_7;
  times[8]=t_8;   times[9]=t_9;   times[10]=t_10; times[11]=t_11;
  times[12]=t_12; times[13]=t_13; times[14]=t_14; times[15]=t_15;
  times[16]=t_16; times[17]=t_17; times[18]=t_18; times[19]=t_19;
  times[20]=t_20; times[21]=t_21; times[22]=t_22; times[23]=t_23;
  wizes[0]=w_0;   wizes[1]=w_1;   wizes[2]=w_2;   wizes[3]=w_3; wizes[4]=w_4;
  wizes[5]=w_5;   wizes[6]=w_6;   wizes[7]=w_7;   wizes[8]=w_8; wizes[9]=w_9;
  wizes[10]=w_10; wizes[11]=w_11; wizes[12]=w_12; wizes[13]=w_13;
  wizes[14]=w_14; wizes[15]=w_15; wizes[16]=w_16; wizes[17]=w_17;
  wizes[18]=w_18; wizes[19]=w_19; wizes[20]=w_20; wizes[21]=w_21;
  wizes[22]=w_22; wizes[23]=w_23;
}

save_h() {
  u_0=users[0];   u_1=users[1];   u_2=users[2];   u_3=users[3];
  u_4=users[4];   u_5=users[5];   u_6=users[6];   u_7=users[7]; u_8=users[8];
  u_9=users[9];   u_10=users[10]; u_11=users[11]; u_12=users[12];
  u_13=users[13]; u_14=users[14]; u_15=users[15]; u_16=users[16];
  u_17=users[17]; u_18=users[18]; u_19=users[19]; u_20=users[20];
  u_21=users[21]; u_22=users[22]; u_23=users[23]; t_0=times[0];
  t_1=times[1];   t_2=times[2];   t_3=times[3];   t_4=times[4]; t_5=times[5];
  t_6=times[6];   t_7=times[7];   t_8=times[8];   t_9=times[9];
  t_10=times[10]; t_11=times[11]; t_12=times[12]; t_13=times[13];
  t_14=times[14]; t_15=times[15]; t_16=times[16]; t_17=times[17];
  t_18=times[18]; t_19=times[19]; t_20=times[20]; t_21=times[21];
  t_22=times[22]; t_23=times[23]; w_0=wizes[0];   w_1=wizes[1];
  w_2=wizes[2];   w_3=wizes[3];   w_4=wizes[4];   w_5=wizes[5]; w_6=wizes[6];
Sonja: Chanes' dagger? Oh, Khayman's dagger

  w_7=wizes[7];   w_8=wizes[8];   w_9=wizes[9];   w_10=wizes[10];
  w_11=wizes[11]; w_12=wizes[12]; w_13=wizes[13]; w_14=wizes[14];
  w_15=wizes[15]; w_16=wizes[16]; w_17=wizes[17]; w_18=wizes[18];
  w_19=wizes[19]; w_20=wizes[20]; w_21=wizes[21]; w_22=wizes[22];
  w_23=wizes[23];
 save_object(Sv_Obj);call_out("save_h",900);
}

reset_graph()
{
  int i;
  if(this_player()->query_level() < 500) return 0;
  for (i=0;i<24;i++)
    {
      wizes[i] =0;
      times[i] =0;
      users[i] =0;
      users_h  =0;
      times_h  =0;
    }
}

query_level() { return 1; }
query_invis() { return 1; }

EOF
>
Sonja: Ignore me

