#!/usr/bin/python3
# -*-coding:UTF-8 -*-
import requests
from bs4 import BeautifulSoup as bs

LOGIN_DATA = {
  'userId': 'souljit1'
  'userPassword': 'thsutleo1!'
}
url = 'https://www.clien.net/service/login'

with requests.Session() as s:
  req = s.get(url)
  