#!/usr/bin/python3
# -*-coding:UTF-8 -*-

import requests, json

# req = requests.get('https://api.github.com/events')
# print(req.text)
# req.raise_for_status()

#jar
jar = requests.cookies.RequestsCookieJar()
jar.set('name','kim',domain='httpbin.org',path='/cookies') #cookie setting
# req = requests.get('http://httpbin.org/cookies',cookies=jar)
# req.raise_for_status()
# print(req.text)

#timeout
# req = requests.get('https://github.com',timeout=5)
# print(req.text)

#Post Fake Rest
# req = requests.post('http://httpbin.org/post',data={'name':'kim'},cookies=jar)
# print(req.text)

#payload
# payload1 = {'key1':'value1','key2':'value2'} #dict
# payload2 = (('key1','value1'),('key2','value2')) #tuple
# payload3 = {'some':'nice'}

# r = requests.post('http://httpbin.org/post',data=payload1) #form data
# print(r.text)

# r = requests.post('http://httpbin.org/post',data=payload2) #form data
# print(r.text)

# r = requests.post('http://httpbin.org/post',data=json.dumps(payload3)) #json data
# print(r.text)

