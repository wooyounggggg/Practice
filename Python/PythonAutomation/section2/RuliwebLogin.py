#!/usr/bin/python3
# -*- coding:UTF-8 -*-

import requests
from bs4 import BeautifulSoup as bs

#Login User Info.
LOGIN_INFO = {
  'user_id':'souljit1',
  'user_pw':'thsutleo1!'
}

#세션 생성, with문 안에서 유지
with requests.Session() as s:
  login_req = s.post('https://user.ruliweb.com/member/login_proc',data=LOGIN_INFO)
  #html 소스 확인
#   print('login_req',login_req.text)
  #header 확인
#   print('headers',login_req.headers)
  if login_req.status_code == 200 and login_req.ok :
    post_one = s.get('http://market.ruliweb.com/read.htm?table=market_ps&page=1&num=4648238&find=&ftext=')
    post_one.raise_for_status()
    soup = bs(post_one.text,'html.parser')
    print(soup.findAll('td',{'class':'con'}))