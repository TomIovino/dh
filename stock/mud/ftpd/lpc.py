import regex
import string

_regex_varname = regex.symcomp('\(<var>[a-zA-Z_][a-zA-Z0-9_]*\) ')
_regex_string = regex.symcomp('"\(<str>\([^"\\]\|\\\\.\)*\)"')
_regex_xref = regex.compile('<[0-9]+>=')
_regex_float = regex.compile('\(-?[0-9]+\.[0-9]+\(e[+-]?[0-9]+\)?\)\(=[0-9a-f]+:[0-9a-f]+\)?')

quotechars = { 't':'\t','n':'\n' }

error = 'lpc parse error'

linepos = 0
line = 0

def get_svalue():
  global linepos,line,error,quotechars,_regex_string,_regex_xref,_regex_float
  pos = linepos

  xref = 0
  ch = line[pos]

  # first check for crossreferences before the svalue itself

  if ch=='<' and _regex_xref.match(line,pos)>=0:
    newpos = string.index(line,'=',pos)
    xref = line[pos:newpos]
    pos = newpos+1
    if line[pos] <> '(':
      raise error, "crossreferencing scalar or string value"

  # Now parse the svalue

  if ch=='(':
    ch = line[pos+1]
    if ch=='{':
      array = []
      if xref:
        crossrefs[xref] = array
      pos = pos + 2
      while line[pos] <> '}':
        linepos = pos
	if line[pos] != ',': # Sigh. MudOS sometimes produces ({,})
          array = array + [ get_svalue() ]
        pos = linepos
        if line[pos]<>',':
          raise error,'garbage in svalue'
        pos = pos + 1
      if line[pos]<>'}' or line[pos+1]<>')':
        raise error,'garbage in svalue'
      linepos = pos+2
      return array
    elif ch=='[':
      mapping = {}
      if xref:
        crossrefs[xref] = mapping
      linepos = pos+2
      if line[linepos] == ':':
	linepos = linepos + 1
	while line[linepos]>='0' and line[linepos]<='9':
	  linepos = linepos + 1
	if line[linepos] <> ']':
	  raise error,'garbage in svalue'
      while line[linepos] <> ']':
        key = get_svalue();
        ch = line[linepos]
        if ch == ':':
          linepos = linepos+1
          values = get_svalue()
          if line[linepos] == ';':
            values = values,
            while line[linepos] == ';':
              linepos = linepos + 1
              values = values + (get_svalue(),)
          mapping[key] = values
          ch = line[linepos]
        else:
          mapping[key] = ()
        if ch <> ',':
          raise error, 'garbage in svalue'
        linepos = linepos + 1
      if line[linepos+1] <> ')':
        raise error,"garbage in svalue"
      linepos = linepos + 2
      return mapping
    else:
      raise error,"garbage in svalue"
  elif ch=='"':
    mlen = _regex_string.match(line,pos)
    if mlen<0:
      raise error,"garbage in svalue"
    linepos = pos+mlen
    str = _regex_string.group('str')
    str = string.splitfields(str,'\\')
    i = 1
    while i<len(str):
      if len(str[i])==0:
	str[i]='\\'
      else:
        ch = str[i][0]
        if quotechars.has_key(ch):
          str[i] = quotechars[ch]+str[i][1:]
      i = i+1
    return string.joinfields(str,'')
  elif (ch>='0' and ch<='9') or ch == '-':
    mlen = _regex_float.match(line,pos)
    if mlen < 0:
      newpos = pos
      if ch == '-':
        newpos = pos+1
        ch = line[newpos]
      while ch >= '0' and ch <= '9':
        newpos = newpos +1
        ch = line[newpos]
      linepos = newpos
      return string.atoi(line[pos:newpos])
    else:
      try:
        val = string.atof(_regex_float.group(1))
      except:
	val = 0.0
      linepos = pos + mlen
      return val
  elif ch=='<': # a crossref
    newpos = string.index(line,'>',pos)
    if newpos<0:
      raise error,"unterminated cross reference"
  else:
    raise error,"garbage in svalue"

def read_data_file(filename):
  global line,linepos,_regex_varname
  vars = { }
  crossrefs = { }
  lines = []
  file = None
  try:
    file = open(filename,'r')
    while 1:
      line = file.readline()
      if not line:
        break
      lines = lines + [ line ]
  finally:
    if file:
      file.close()
  try:
    for line in lines:
      if line[0] <> '#':
        pos = _regex_varname.match(line)
        if pos<0:
          raise error,'line not starting with a variable name'
        varname = _regex_varname.group('var')
        line = line[pos:]
        linepos = 0
        temp = get_svalue();
        vars[varname] = temp
        if line[linepos] <> '\n':
          raise error,"garbage at end of line: "+line[linepos:-2]
  except ValueError:
    raise error,"file corrupted"
  return vars
