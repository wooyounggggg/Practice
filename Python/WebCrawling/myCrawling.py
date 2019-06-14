#!/usr/bin/python3
import requests
from bs4 import BeautifulSoup as bs

LOGIN_INFO = {
    'TextUserID': 'souljit1',
    'TextPassword': 'thsutleo1!'
}

with requests.Session() as s:
    first_page = s.get('https://www.inflearn.com')
    html = first_page.content
    html_obj = bs(html,'html.parser')
    print(html_obj)
#     login_req = s.post('https://portal.konkuk.ac.kr/Login.aspx', data=LOGIN_INFO) 
#     print(first_page.status_code)
#     print(login_req.status_code)
#     if login_req.status_code != 200:
#         raise Exception('로그인이 되지 않았어요! 아이디와 비밀번호를 다시한번 확인해 주세요.')