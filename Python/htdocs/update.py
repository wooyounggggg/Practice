#!/usr/bin/python3
print("Content-Type: text/html\n")
import cgi, os
from view import getList

form = cgi.FieldStorage()
# files = os.listdir('data')
# listStr = ''
# for item in files:
#   listStr = listStr+'<li><a href="index.py?id={name}">{name}</a></li>'.format(name=item)
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
  <form action="process_update.py" method="post">
    <a href="create.py">create</a>
      <input type="hidden" name="pageId" value={form_default_title}>
      <p><input type="text" placeholder="title" value="{form_default_title}" name="title"></p>
      <p"><textarea rows="20" placeholder="description" name="description" style="width:50%;">{form_default_description}</textarea></p>
      <p><input type="submit"></p>
  </form>
</body>
</html>
'''.format(title=pageId, description=description, listStr=getList(), 
           form_default_title=pageId, form_default_description=description))

#URL Query String