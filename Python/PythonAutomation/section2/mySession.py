#!/usr/bin/python3
# -*- coding:UTF-8 -*-

import requests, json
with requests.Session() as s:
  s.auth=('a','b') #session 레벨에서 데이터를 추가할 시에는 데이터가 유지됨.
  r = s.get('https://httpbin.org/basic-auth/a/b' ,cookies={'from-my': 'browser'})
#   메서드 레벨에서 데이터를 추가하면 한번만 유효함.
  print(r.text) 
  r = s.get('https://httpbin.org/basic-auth/a/b')
  print(r.text)