#!/usr/bin/python3
# -*- coding:UTF-8 -*-

import requests
from bs4 import BeautifulSoup as bs

#Login User Info.
LOGIN_INFO = {
  'jazoest': '2743'
  ,'lsd': 'AVrOWlci'
  ,'email': 'souljit2@gmail.com'
  ,'pass': 'airforce20170918!'
  ,'timezone': '-540'
  ,'lgndim': 'eyJ3IjoxMjgwLCJoIjoxMDI0LCJhdyI6MTI4MCwiYWgiOjk4NCwiYyI6MjR9'
  ,'lgnrnd': '040234_d-8L'
  ,'lgnjs': '1555066934'
  ,'ab_test_data': 'AAVqqV/VVqVAAAAAAAAAAAAVAAAVAAAAAAAAAAAARc/iRFAFAAIFAB'
  ,'locale': 'ko_KR'
  ,'next': 'https://www.facebook.com/'
  ,'login_source': 'login_bluebar'
  ,'prefill_contact_point': 'souljit2@gmail.com'
  ,'prefill_source': 'last_login'
  ,'prefill_type': 'contact_point'
  ,'skstamp': 'eyJoYXNoIjoiNzBkZTNkYmFhNjNhNGEzZTIzNTAxNWY0ZThjMzdlYTgiLCJoYXNoMiI6ImNmMGI2YzVmMjBmNTc5NTQzZjM4YzQ4ZDYzYjhkZTZlIiwicm91bmRzIjo1LCJzZWVkIjoiMThlYTM5Nzk5YzMxNTkxYjZkNDYxYmYzZjkxOWMzNGMiLCJzZWVkMiI6IjRiYjVlOTU3ZjAxMGM0ODViNzgxYjUyMmE2M2EyNmQ4IiwidGltZV90YWtlbiI6MzA3MTY1LCJzdXJmYWNlIjoibG9naW4ifQ=='
}

headers={
  'authority': 'www.facebook.com'
  ,'method': 'POST'
  ,'path': '/login/device-based/regular/login/?login_attempt=1&lwv=111'
  ,'scheme': 'https'
  ,'accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8'
  ,'accept-encoding': 'gzip, deflate, br'
  ,'accept-language': 'ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7'
  ,'cache-control': 'max-age=0'
  ,'content-length': '751'
  ,'content-type': 'application/x-www-form-urlencoded'
  ,'cookie': 'sb=Y12wXPwT1x_giXpZVX_RLVJN; datr=Y12wXLp6gotivnBYQVPZhHi_; locale=ko_KR; fr=0NNMj4I4ki7SoZJbG.AWV1nTKHBglK9VRl_5fVsx5YcsA.BcsFtn.Dt.Fyw.0.0.BcsHBK.AWXzcgF5; wd=857x882'
  ,'origin': 'https://www.facebook.com'
  ,'referer': 'https://www.facebook.com/?stype=lo&jlou=Afce05cuTCmluVB_vd8Uqc7sDiKCn50YoeQjy8c8z3mulj5LZUZS9doKYa-mx3oTcamak8c8rInXaVGVb48xh02VsmP4py3bigxFDANzgI4qrQ&smuh=20643&lh=Ac9gkAOgNvyEYCgF'
  ,'upgrade-insecure-requests': '1'
  ,'user-agent': 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.86 Safari/537.36 OPR/60.0.3255.27'
}
#세션 생성, with문 안에서 유지
with requests.Session() as s:
  req = s.post('https://www.facebook.com/login/device-based/regular/login/?login_attempt=1&lwv=111',headers=headers,data=LOGIN_INFO)
  bsObj = bs(req.content,'html.parser')
  print(bsObj)