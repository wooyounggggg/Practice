#!/usr/bin/python3
# -*-coding:UTF-8 -*-

import requests
from bs4 import BeautifulSoup as bs

url = 'https://okky.kr/j_spring_security_check'

LOGIN_DATA = {
  'j_username' : 'souljit1',
  'j_password' : 'thsutleo1!'
}

req = requests.post(url,data=LOGIN_DATA)
bsObj = bs(req.content,'html.parser')
print(bsObj)