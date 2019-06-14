#!/usr/bin/python3
# -*- coding:UTF-8 -*-

import requests
url = 'https://jsonplaceholder.typicode.com/albums'
with requests.Session() as s:
#   r = s.get('http://httpbin.org/get')
#   print(r.status_code)
#   print(r.ok) #r.ok는 서버가 열려있는지 확인!
  r = s.get(url)
  print(r.json()[0].keys()) #json keys
  print(r.json()[0].values()) #json values
  print(r.encoding) #encoding
#   print(r.content) #binary -> multi-media data
  print(r.raw)
  