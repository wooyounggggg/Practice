#!/usr/bin/python3
# -*-coding:UTF-8 -*-

import requests
from fake_useragent import UserAgent

url = 'https://www.wishket.com/accounts/login/'
#Fake User Agent
ua = UserAgent()
#LOGIN Info.
LOGIN_INFO = {
  'identification':'souljit1'
  ,'password':'thsutleo1'
}
headers = {
  #header의 User-Agent를 확인해서 버전에 맞게 렌더링해서 사용자에게 보내줌.
  'User-Agent': str(ua.chrome)
  ,'Referer': 'https://www.wishket.com/accounts/login/'
}
#wishket은 csrf_token을 발급하기 때문에 데이터에 csrf 토큰값을 넣어주어야 함

with requests.Session() as s:
  #post를 요청하기 전에 cookie를 받아온다.
  s.get(url)
  LOGIN_INFO['csrfmiddlewaretoken']=s.cookies['csrftoken']
  
  req = s.post(url,data=LOGIN_INFO,headers=headers)
  print(req)