import lpc, marshal, string, crypt, sys, time, stat, posix
import WAIT

PORT = 8911
HOME = '/home/dh/mud/lib'
MUDNAME = 'circular.org'
MAXTRANSFER = 2500000
MAXIDLE = 1800
ALLOW_GUEST = 0

LOG_FILE = '/home/dh/mud/lib/log/sys/FTPD'

filter_Z =      [ "/bin/compress", "-c" ]
filter_gz =     [ "/bin/gzip", "-c" ]
filter_tar =    [ "/bin/tar", "-cf", "-" ]
filter_tar_Z =  [ "/bin/tar", "-cZf", "-" ]
filter_tar_gz = [ "/bin/tar", "-czf", "-" ]

def start_up():
  pass

def fork_session(host):
  return 1

def set_user(name):
  global user, userdata, security, level
  user = string.lower(name)
  try:
    userdata = lpc.read_data_file(HOME+'/usr/creators/'+name+'.o')
  except:
    if not is_guest():
      return 0
  level = 0
  if is_guest():
    return 1
  if userdata.has_key("security_level"):
    level = userdata["security_level"]
    if level <= 10:
      return 0
  else:
    return 0
  return 1

def auth_user(passwd):
  if is_guest():
    return 1
  if not userdata.has_key('password'):
    return 0
  pwd = userdata['password']
  if crypt.crypt(passwd,pwd)<>pwd:
    return 0
  return 1

def is_guest():
  global user
  if not ALLOW_GUEST:
    return 0
  return user=='guest' or user=='ftp' or user=='anonymous'

def get_home(name):
  if not name:
    if is_guest():
      return "/ftp"
    name = user
  return "/w/"+name

def valid_write(file):
  if is_guest():
    return 0
  if level>=90:
    return 1
  if file[:6] == "/open/":
    return 1
  home = get_home(user)+"/"
  if file[:len(home)] == home:
    return 1
  return 0
  

def valid_read(file):
  if level>=90:
    return 1
  home = get_home(user)+"/"
  if file[:len(home)] == home:
    return 1
  if file[:5] == "/doc/":
    return 1
  if file[:6] == "/open/":
    return 1
  return 0

