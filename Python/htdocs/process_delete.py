#!/usr/bin/python3

import cgi, os
form = cgi.FieldStorage()
pageId = form.getvalue('pageId')

os.remove('data/'+pageId)

# redirection
print("Location: index.py")
print()