DEBUG = 0

# basic modules
import sys

# standard unix functions
import posix, posixpath, stat, time

# socket I/O
import socket, SOCKET, fcntl, FCNTL, struct, select, ERRNO, stat

# string manipulation
import string, regex, glob

#local configuration
import local

MAX_BUFFER_SIZE = 32768

transfer_type = 'A'
pending_input = ['']
input_closed = 0
current_path = '/'
last_command = 0

def main():
  global incoming
  try:
    sys.stdout = sys.stdin = open("ERRORS."+socket.gethostname(),"a")
  except:
    pass
  if not DEBUG:
    posix.setsid()
    child = posix.fork()
    if child:
      sys.exit()
  local.start_up()
  incoming = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
  incoming.setsockopt(SOCKET.SOL_SOCKET,SOCKET.SO_REUSEADDR,1)
  incoming.bind('',local.PORT)
  incoming.listen(5)
  while 1:
    link, peer = incoming.accept()
    if not local.fork_session(peer[0]):
      force_close(link)
      continue
    if DEBUG:
      session(link)
      sys.exit()
    else:
      try:
	child = posix.fork()
      except:
        # Oops! Something went seriously wrong. System overload.
	time.sleep(30) # Let's calm down a bit.
        continue
      if child==0:
        # double fork to avoid zombies <sigh>
	try:
	  child = posix.fork()
	except:
	  sys.exit() # Clean exit, don't clutter stderr
        if child == 0:
          session(link)
          sys.exit()
        else:
          sys.exit() # doesn't matter if there was an error or not
      else:
        force_close(link) # close our own copy to avoid running out of fds
        if child>=0:
          posix.waitpid(child,0)

if hasattr(FCNTL,'FNDELAY'):
  def make_non_blocking(fd):
    fcntl.fcntl(fd,FCNTL.F_SETFL,FCNTL.FNDELAY)
elif hasattr(FCNTL,'O_NDELAY'):
  def make_non_blocking(fd):
    fcntl.fcntl(fd,FCNTL.F_SETFL,FCNTL.O_NDELAY)
else:
  def make_non_blocking(fd):
    fcntl.ioctl(fd,IOCTL.FIONBIO,struct.pack('i',1))

def resolve_path(path):
  if len(path) and path[0]=='~':
    try:
      i = string.index(path,'/')
    except:
      i = len(path)
    if i == 1:
      path = local.get_home(None)+path[i:]
    else:
      path = local.get_home(path[1:i])+path[i:]
  if len(path) and path[0]<>'/':
    path = current_path + path
  path = string.splitfields(path,'/')
  j = 0
  for i in range(len(path)):
    if path[i]=='' or path[i]=='.':
      if i == len(path)-1:
        path[j]=""
        j = j+1
    elif path[i]=='..':
      if j>0:
        j = j-1
    else:
      path[j] = path[i]
      j = j+1
  path = '/'+string.joinfields(path[:j],'/')
  return local.HOME+path,path

def change_dir(path):
  global current_path
  temp, temp2 = resolve_path(path)
  try:
    st = posix.stat(temp)
    if not stat.S_ISDIR(st[0]):
      return 0
  except posix.error:
    return 0
  current_path = temp2
  if current_path[-1]<>'/':
    current_path = current_path + '/'
  return 1

def getline():
  global pending_input, input_closed
  if input_closed:
    if len(pending_input):
      result = pending_input[0]
      pending_input = pending_input[1:]
      return result
    return None
  while len(pending_input)==1:
    if not select.select([control],[],[],local.MAXIDLE)[0]:
      timeout()
    try:
      input = posix.read(control.fileno(),1000)
    except posix.error:
      input = ""
    if not input:
      input_closed = 1
      return getline()
    input = string.splitfields(input,'\n')
    last = len(pending_input)-1
    pending_input[last] = pending_input[last]+input[0]
    pending_input = pending_input + input[1:]
  result = pending_input[0]
  pending_input = pending_input[1:]
  if len(result) and result[-1]=='\r':
    result = result[:-1]
  return result

def log_file(message):
  f = None
  try:
    f = open(local.LOG_FILE,"a")
    f.write(user_name+': '+message+'\n')
  except:
    pass
  if f:
    try:
      f.close()
    except:
      pass

def force_close(stream):
  if type(stream)==type(0):
    try:
      if stream >= 0:
	posix.close(stream)
    except:
      pass
  else:
    try:
      stream.close()
    except:
      pass

def timeout(*die_silently):
  if not len(die_silently):
    reply(421,'Timeout: closing control connection.')
  sys.exit()

def reply(code,message):
  data = str(code)+' '+message+'\r\n'
  while data:
    if not select.select([],[control],[],local.MAXIDLE)[1]:
      timeout(1)
    try:
      n = posix.write(control.fileno(),data)
    except posix.error:
      sys.exit(1)
    data = data[n:]

def receive_file(type,name,file,append):
  global peeraddr, peerport
  if not local.valid_write(name):
    reply(553,name+': Permission denied.')
    return
  log_file('upload of '+name)
  s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
  fd = s.fileno()
  make_non_blocking(fd)
  if peerport<0:
    reply(425,'Cannot open data connection without valid PORT command.')
    return
  try:
    s.connect(string.joinfields(peeraddr,'.'),peerport)
  except socket.error,errno:
    if errno[0]<>ERRNO.EINPROGRESS:
      reply(425,'Unable to establish data connection')
      return
  if type=='A':
    type = 'ASCII'
    ascii = 1
  else:
    type = 'BINARY'
    ascii = 0
  reply(150,'Opening %s data connection for %s' % (type, name) )
  try:
    out = -1
    pending_cr = 0
    while 1:
      if not select.select([fd],[],[],local.MAXIDLE)[0]:
        raise posix.error
      packet = posix.read(fd,16384)
      if not packet:
        if pending_cr:
	  posix.write(out,"\r")
        break
      if ascii:
        if pending_cr:
	  packet = "\r" + packet
	  pending_cr = 0
	if packet[-1] == "\r":
	  packet = packet[:-1]
	  pending_cr = 1
	packet = string.joinfields(string.splitfields(packet,"\r\n"),"\n")
      if out<0:
	try:
	  mode = FCNTL.O_WRONLY+FCNTL.O_CREAT+FCNTL.O_TRUNC
	  if append:
	    mode = mode + O_APPEND
	  out = posix.open(file, mode)
	except posix.error:
	  reply(426,'Cannot open file for writing.')
	  force_close(s)
	  return
      posix.write(out,packet)
  except posix.error:
    force_close(s)
    force_close(out)
    reply(426,'Data connection broken.')
    return
  force_close(s)
  force_close(out)
  reply(226,'Transfer complete')

def open_data_conn():
  if peerport<0:
    reply(425,'Cannot open data connection without valid PORT command.')
    return None
  try:
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    fd = s.fileno()
    make_non_blocking(fd)
    s.connect(string.joinfields(peeraddr,'.'),peerport)
    return s
  except socket.error,errno:
    if errno[0]<>ERRNO.EINPROGRESS:
      force_close(s)
      reply(425,'Unable to establish data connection.')
      return None
    return s

def exists(file,dir):
  try:
    st = posix.stat(file)
  except:
    return 0
  if dir:
    if stat.S_ISDIR(st[0]):
      return 1
  else:
    if stat.S_ISREG(st[0]):
      return 1
  return 0
  
def send_file(type,name,file): 
  if exists(file,0):
    extension = None
    check = 0
  else:
    filter = None
    for (ext,filt,dir) in [
	(".Z",local.filter_Z,0),
	(".gz",local.filter_gz,0),
	(".tar",local.filter_tar,1),
	(".tar.Z",local.filter_tar_Z,1),
	(".taz",local.filter_tar_Z,1),
	(".tgz",local.filter_tar_gz,1),
	(".tar.gz",local.filter_tar_gz,1) ]:
      if file[-len(ext):] == ext and exists(file[:-len(ext)],dir):
        filter = filt
	file = file[:-len(ext)]
	extension = ext
	check = dir
	break
    if not filter:
      reply(550, name+": no such file or directory.")
      return
  if check:
    if not local.valid_write(name[:-len(ext)]+"/"):
      reply(550, name+": Permission denied.")
      return
  else:
    if not local.valid_read(name):
      reply(550, name+": Permission denied.")
      return
  if extension:
    if type == 'A':
      reply(550, "Can't transfer binary data using ASCII mode.")
      return
    read, write = posix.pipe()
    try:
      pid = posix.fork()
    except posix.error:
      reply(425,'Cannot fork.')
      return
  else:
    try:
      read = -1
      read = posix.open(file, FCNTL.O_RDONLY)
      size = posix.fstat(read)[6]
    except posix.error:
      reply(550, name + ": File inaccessible.")
      force_close(read)
      return
    pid = 0
  if type=='A':
    type = 'ASCII'
  else:
    type = 'BINARY'
  if extension and not pid:
    try:
      force_close(control)
      posix.dup2(write,1)
      posix.close(write)
      posix.close(read)
      try:
	error = posix.open("/dev/null", FCNTL.O_RDWR)
	posix.dup2(error,2)
	posix.close(error)
      except:
        force_close(2)
      dir, file = posixpath.split(file)
      posix.chdir(dir)
      filter = filter + [ file ]
      posix.execv(filter[0], filter)
    except:
      sys.exit(1) # Usually a failed execv()
  else:
    log_file("download of "+name)
    if extension:
      force_close(write)
      reply(150,'Opening %s data connection for %s[%s]' %
	(type, name[:-len(extension)], extension) )
    else:
      reply(150,'Opening %s data connection for %s (%d bytes)' % (type,
                name, size) )
    try:
      sock = open_data_conn()
      if not sock:
	raise posix.error
      write = sock.fileno()
      make_non_blocking(read)
      buffer = ""
      while buffer or read >= 0:
	if read >= 0 and len(buffer) < MAX_BUFFER_SIZE:
	  inp, out, exc = select.select([read],[write],[],local.MAXIDLE)
	else:
	  inp, out, exc = select.select([],[write],[],local.MAXIDLE)
        if inp:
	  packet = posix.read(read,MAX_BUFFER_SIZE)
	  if not packet:
	    posix.close(read)
	    read = -1
	  if type == "ASCII":
	    packet = string.joinfields(string.splitfields(packet,"\n"),"\r\n")
	  buffer = buffer + packet
	elif not out:
	  timeout()
	if out and buffer:
	  written = posix.write(write,buffer)
	  buffer = buffer[written:]
      sock.close()
      reply(226,'Transfer complete')
    except:
      reply(426,'Data connection broken.')
      # clean up socket and pipe, dispose of child process
      force_close(sock)
      try:
	if pid:
	  posix.kill(pid,9) # Need to wait for it later, so it should go away
      except posix.error:
        pass
      if read >= 0:
	force_close(read)
    # don't create zombies
    try:
      if pid:
	posix.waitpid(pid,0)
    except posix.error:
      pass

def send_data(type,name,data):
  s = open_data_conn()
  if not s:
    return
  fd = s.fileno()
  if type=='A':
    data = string.joinfields(string.splitfields(data,'\n'),'\r\n')
    type = 'ASCII'
  else:
    type = 'BINARY'
  reply(150,'Opening %s data connection for %s' % (type, name) )
  try:
    while len(data):
      if not select.select([],[fd],[],local.MAXIDLE)[1]:
        raise posix.error
      try:
	n = posix.write(fd,data)
      except posix.error,errno:
	if errno[0] <> ERRNO.EAGAIN:
	  raise posix.error
	else:
	  n = 0
      data = data[n:]
  except posix.error:
    force_close(s)
    reply(426,'Data connection broken.')
    return
  force_close(s)
  reply(226,'Transfer complete')

def session(link):
  global control,peeraddr,peerport,commands
  force_close(incoming)
  make_non_blocking(link.fileno())
  control = link
  # We need the other side's port, and peerport-1 is the default destination
  # address if there's no PORT command. However, make sure we're not going
  # to access any of the privileged ports.
  peeraddr, peerport = link.getpeername()
  if peerport < 1024:
    peerport = -1
  peeraddr = string.splitfields(peeraddr,'.')
  commands = login_commands
  reply(220,'LPmud FTP server ('+local.MUDNAME+') ready. Login with your wizard name.')
  while 1:
    input = getline()
    if not input:
      parse_input('quit')
    parse_input(input)

def bad_command(cmd):
  reply(500, '\''+cmd+'\' command not understood.')

def parse_input(line):
  global last_command
  try:
    n = string.index(line,' ')
  except ValueError:
    cmd = line
    args = None
  else:
    cmd = line[:n]
    args = line[n+1:]
  cmd = string.upper(cmd)
  if commands.has_key(cmd):
    commands[cmd](args)
    last_command = cmd
  else:
    if full_commands.has_key(cmd):
      reply(530,'Please login with USER and PASS.')
    else:
      bad_command(line)

def cmd_quit(args):
  if args==None:
    reply(221,'Goodbye.')
    force_close(control)
    sys.exit()
  else:
    bad_command('QUIT '+args)

def cmd_user(args):
  global user_name, commands
  if args==None:
    bad_command('USER')
    return
  if not local.set_user(args):
    reply(530,'User '+args+' unknown.')
    return
  if local.is_guest():
    reply(331,'Guest login ok, send e-mail address as password.')
  else:
    reply(331,'Password required for '+args+'.')
  user_name = args
  commands = login_commands

def cmd_pass(args):
  global peeraddr, commands
  hostname = string.joinfields(peeraddr,'.')
  if args == None or not local.auth_user(args):
    log_file('Failed login for '+user_name+' from '+hostname+'.')
    reply(530,'Login incorrect')
    return
  commands = full_commands
  if local.is_guest():
    reply(230,'Guest login ok, access restrictions apply.')
    try:
      string.index(args,'@')
    except ValueError:
      log_file('Guest login from '+hostname+' without a valid e-mail address.')
    else:
      log_file('Guest login from '+hostname+' as '+args+'.')
  else:
    log_file('User '+user_name+' logged in from '+hostname)
    reply(230,'User '+user_name+' logged in.')
  change_dir('~')

def cmd_port(args):
  global peeraddr,peerport
  if args == None:
    bad_command('PORT')
    return
  fields = string.splitfields(args,',')
  if len(fields)<>6:
    bad_command('PORT '+args)
    return
  for f in fields:
    if len(f)>3 or regex.match('^[0-9][0-9]*$',f)<0:
      bad_command('PORT '+args)
      return
  for i in range(0,4):
    if string.atoi(peeraddr[i]) <> string.atoi(fields[i]):
      bad_command('PORT '+args)
      return
  port = string.atoi(fields[4])*256+string.atoi(fields[5])
  if port > 65535 or port < 1024:
    reply(500,'Port #'+str(port)+' is not a valid port number.')
    return
  peerport = port
  reply(200,'PORT command successful.')

def cmd_type(args):
  global transfer_type
  if args == None:
    bad_command('TYPE')
    return
  args = string.upper(args)
  if args=='I' or args == 'A':
    transfer_type = args
    reply(200,'Type set to '+args+'.')
  else:
    bad_command('TYPE '+args)

def cmd_abor(args):
  if args <> None:
    bad_command('ABOR '+args)
  else:
    reply(225,'ABOR command successful.')

def cmd_noop(args):
  reply(200,'NOOP command sucessful.')

def cmd_retr(args):
  if args == None:
    bad_command('RETR '+args)
    return
  file, path = resolve_path(args)
  if not local.valid_read(path):
    reply(550,path+': Permission denied.')
    return
  send_file(transfer_type,path,file)
 
def directory(args):
  if args==None:
    args='.'
  file, path = resolve_path(args)
  if path[-1]<>'/':
    path = path + '/'
  if not local.valid_read(path):
    reply(550,path+': Permission denied.')
    return None
  list = glob.glob(file)
  if len(list)==1:
    if list[0]== file:
      try:
        list = posix.listdir(file)
        list.sort()
        if file[-1]<>'/':
          file = file + '/'
        for i in range(len(list)):
          list[i] = file + list[i]
      except posix.error:
        list = [ file ]
  return list

def ls_bits_output(st):
  if stat.S_ISDIR(st[0]): h = 'd'
  elif stat.S_ISCHR(st[0]): h = 'c'
  elif stat.S_ISBLK(st[0]): h = 'b'
  elif stat.S_ISFIFO(st[0]): h = 'p'
  elif stat.S_ISSOCK(st[0]): h = 's'
  elif stat.S_ISREG(st[0]): h = '-'
  else: h = '?'
  if st[0] & stat.S_IRUSR: h = h + 'r'
  else: h = h + '-'
  if st[0] & stat.S_IWUSR: h = h + 'w'
  else: h = h + '-'
  if st[0] & stat.S_IXUSR: h = h + 'x'
  else: h = h + '-'
  if st[0] & stat.S_IRGRP: h = h + 'r'
  else: h = h + '-'
  if st[0] & stat.S_IWGRP: h = h + 'w'
  else: h = h + '-'
  if st[0] & stat.S_IXGRP: h = h + 'x'
  else: h = h + '-'
  if st[0] & stat.S_IROTH: h = h + 'r'
  else: h = h + '-'
  if st[0] & stat.S_IWOTH: h = h + 'w'
  else: h = h + '-'
  if st[0] & stat.S_IXOTH: h = h + 'x'
  else: h = h + '-'
  if st[0] & stat.S_ISUID:
    if h[3]=='-': h = h[:3] + 'S' + h[4:]
    else: h = h[:3] + 's' + h[4:]
  if st[0] & stat.S_ISGID:
    if h[6]=='-': h = h[:6] + 'S' + h[7:]
    else: h = h[:6] + 's' + h[7:]
  if st[0] & stat.S_ISVTX:
    if h[9]=='-': h = h[:9] + 'T'
    else: h = h[:9] + 't'
  return h + ' '

def ls_link_number(st):
  return '%2d ' % st[3]

def ls_user_name(st):
  return 'dh       '

def ls_group_name(st):
  return 'dh       '

def ls_file_size(st):
  return '%8d ' % st[6]

def ls_file_date(st):
  h = time.ctime(st[8])
  if (st[8] < time.time()-15552000):    # approximately half a year
    return h[4:11] + h[19:] + ' '
  else:
    return h[4:16] + ' '

def ls_file_name(file):
  return posixpath.basename(file)

def cmd_nlst(args):
  files = directory(args)
  if files == None:
    return
  if len(files)==0:
    output=''
  else:
    files = map(posixpath.basename,files)
    output = string.joinfields(files,'\n')+'\n'
  send_data('A','NLST',output)
  return

def cmd_list(args):
  files = directory(args)
  if files == None:
    return
  if len(files)==0:
      output=''
  else:
    for i in range(len(files)):
      st = posix.stat(files[i])
      files[i] = (ls_bits_output(st) + ls_link_number(st) + ls_user_name(st) +
                  ls_group_name(st) + ls_file_size(st) + ls_file_date(st) +
                  ls_file_name(files[i]))
      output = string.joinfields(files,'\n')+'\n'
  send_data('A','LIST',output)
  return

def cmd_dele(args):
  if args==None:
    bad_command('DELE')
    return
  file, path = resolve_path(args)
  if not local.valid_write(path):
    reply(550,path+': Permission denied.')
    return
  try:
    posix.unlink(file)
  except posix.error:
    reply(550,path+': No such file or directory.')
    return
  reply(250,'DELE command successful.')
  return

def cmd_mkd(args):
  if args==None:
    bad_command('MKD')
    return
  file, path = resolve_path(args)
  if not local.valid_write(path):
    reply(550,path+': Permission denied.')
    return
  try:
    posix.mkdir(file,0777)
  except posix.error:
    reply(550,path+': No such file or directory.')
    return
  reply(250,'MKD command successful.')
  return

def cmd_rmd(args):
  if args==None:
    bad_command('RMD')
    return
  file, path = resolve_path(args)
  if not local.valid_write(path):
    reply(550,path+': Permission denied.')
    return
  try:
    posix.rmdir(file)
  except posix.error:
    reply(550,path+': No such file or directory.')
    return
  reply(250,'RMD command successful.')
  return
  pass

def cmd_stor(args):
  if not args:
    bad_command('STOR')
    return
  file, path = resolve_path(args)
  receive_file(transfer_type, path, file, 0)

def cmd_appe(args):
  if not args:
    bad_command('APPE')
    return
  file, path = resolve_path(args)
  receive_file(transfer_type, path, file, 1)

def cmd_rnfr(args):
  global from_file,from_path
  from_file = 0
  if not args:
    bad_command('RNFR')
    return
  file, path = resolve_path(args)
  if not local.valid_write(path):
    reply(550,path + ': Permission denied.')
    return
  try:
    posix.stat(file)
  except posix.error:
    reply(550,path+': No such file or directory.')
    return
  from_file = file
  from_path = path
  reply(350,'File exists, ready for destination name.')

def cmd_rnto(args):
  if last_command <> 'RNFR' or from_file==0:
    reply(503,'Bad sequence of commands')
    return
  if not args:
    bad_command('RNTO')
    return
  file, path = resolve_path(args)
  if not local.valid_write(args):
    reply(550,path+': Permission denied.')
    return
  if not local.valid_write(from_path):
    reply(550,from_path+': Permission denied.')
    return
  try:
    posix.rename(from_file,file)
  except posix.error:
    reply(550,from_path+': Unable to rename.')
  else:
    reply(250,'RNTO command successful.')

def cmd_cwd(args):
  if args==None:
    args = '~'
  if (change_dir(args)):
    reply(250,'CWD command successful.')
  else:
    reply(550,args+': Not a directory.')

def cmd_cdup(args):
  cmd_cwd('..')

def cmd_pwd(args):
  pwd = current_path
  if len(pwd)>1:
    pwd = pwd[:-1]
  reply(257,'"'+pwd+'" is current directory.')

login_commands = { 'QUIT' : cmd_quit, 'USER' : cmd_user, 'PASS': cmd_pass }
full_commands = { 'QUIT' : cmd_quit, 'USER' : cmd_user, 'PASS': cmd_pass, \
  'PORT' : cmd_port, 'RETR' : cmd_retr, 'NOOP' : cmd_noop, 'ABOR' : cmd_abor, \
  'STOR' : cmd_stor, 'APPE' : cmd_appe, 'MKD' : cmd_mkd, 'RMD' : cmd_rmd, \
  'PWD' : cmd_pwd, 'CDUP' : cmd_cdup, 'CWD' : cmd_cwd , 'DELE' : cmd_dele, \
  'LIST' : cmd_list, 'NLST' : cmd_nlst, 'RNFR': cmd_rnfr, 'RNTO' : cmd_rnto, \
  'TYPE' : cmd_type }

if __name__ == "__main__":
  main()
