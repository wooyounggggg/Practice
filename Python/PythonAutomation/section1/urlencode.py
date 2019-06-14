#!/usr/bin/python3
# -*- coding:UTF-8 -*-
import urllib.request as req
from urllib.parse import urlencode

API = 'https://api.ipify.org'

values = {
  'format' : 'json'
}
print('before :',values)
params = urlencode(values)
print('after :',params)

url = API+'?'+params
print('요청 url :',url)

reqData = req.urlopen(url).read().decode('utf-8')
print(reqData)

#콜백함수 살펴보기! https://opentutorials.org/course/2136/11861