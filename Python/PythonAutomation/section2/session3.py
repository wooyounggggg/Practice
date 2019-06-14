#!/usr/bin/python3
# -*- coding:UTF-8 -*-

import requests, json
url = ''
with requests.Session() as s:
  r = s.get('http://httpbin.org/stream/10', stream=True) #데이터가 stream 형태로 오는 경우에,
#                                                          stream=True로 명시해주어야 함.
#   print(r.text)
#   print(r.encoding)
  if r.encoding is None :
    r.encoding = 'utf-8'
  for line in r.iter_lines() : #decode_unicode 설정해주어야 함.
    print(line)
#     b = json.loads(line) #dict
#     print(b)
#     for e in b.keys():
#       print('key :',e,',value :',b[e])