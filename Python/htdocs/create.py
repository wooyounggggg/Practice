#!/usr/bin/python3
print("Content-Type: text/html\n")

import cgi, os
from view import getList

form = cgi.FieldStorage()
files = os.listdir('data')

if 'id' in form:
  pageId = form.getvalue('id')
  description = open('data/'+pageId,'r').read()
else:
  pageId = 'Welcome'
  description = 'Hello Web'

print('''<!doctype html>
<html>
<head>
  <title>WEB1 - Welcome</title>
  <meta charset="utf-8">
</head>
<body>
  <h1><a href="index.py">WEB</a></h1>
  <ol>
    {listStr}
  </ol>
  <form action="process_create.py" method="post">
    <a href="create.py">create</a>
      <p><input type="text" placeholder="title" name="title"></p>
      <p><textarea rows="20" placeholder="description" name="description" style="width:50%;"></textarea></p>
      <p><input type="submit"></p>
  </form>
</body>
</html>
'''.format(title=pageId, description=description, listStr=getList()))

#URL Query String