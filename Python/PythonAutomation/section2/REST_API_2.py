#!/usr/bin/python3
# -*-coding:UTF-8 -*-

import requests, json

#REST : POST,GET,PUT:UPDATE/[REPLACE](FETCH:UPDATE/[MODIFY]),DELETE

payload1 = {'key1':'value1','key2':'value2'} #dict
payload2 = (('key1','value1'),('key2','value2')) #tuple
payload3 = {'some':'nice'}

#put
# r = requests.put('http://httpbin.org/put', data=payload1)
# print(r.text)

#fake rest api
# r = requests.put('https://jsonplaceholder.typicode.com/posts/1',data=payload1)
# print(r.text)

#delete
# r = requests.delete('https://jsonplaceholder.typicode.com/posts/1')
# print(r.text)

