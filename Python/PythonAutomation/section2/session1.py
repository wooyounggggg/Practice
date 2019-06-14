#!/usr/bin/python3
# -*- coding:UTF-8 -*-

import requests
with requests.Session() as s:
#   r = s.get('https://www.naver.com')  #PUT(FETCH), DELETE, GET, POST
#   print('1',r.text)

#   r = s.get('http://httpbin.org/cookies',cookies={'from':'myName'})
#   print(r.text)

  url = 'https://jsonplaceholder.typicode.com/users'
  r = s.get(url)
  print(r.text)

#   r = s.get('https://www.naver.com')
#   print(r.text)