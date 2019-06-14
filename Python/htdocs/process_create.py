#!/usr/bin/python3

import cgi
form = cgi.FieldStorage()
title = form.getvalue('title')
description = form.getvalue('description')

opened_file = open('data/'+title,'w')
opened_file.write(description)
opened_file.close()

# redirection
print("Location: index.py?id="+title)
print()