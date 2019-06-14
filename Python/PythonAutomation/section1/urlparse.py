#!/usr/bin/python3
# -*- coding:UTF-8 -*-
import urllib.request as req
from urllib.parse import urlparse

url = 'http://www.encar.com/index.do'
mem = req.urlopen(url)
# print('geturl :',mem.status) #200:ok 404:server not found 403:reject, 500:server error
# print('read :',mem.read().decode('euc-kr')) #read 매개변수는 불러올 byte를 지정한다.

print(urlparse(url))

#urlpase는 url을 parsing하여 직관적인 형태로 반환함.