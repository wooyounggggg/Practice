#!/usr/bin/python3
# -*- coding:UTF-8 -*-

import requests
from bs4 import BeautifulSoup as bs
url = 'https://portal.konkuk.ac.kr/Login.aspx'
url2 = 'https://report.konkuk.ac.kr/acweb/readNx.jsp'#'https://kupis.konkuk.ac.kr/NxKonkuk/tk-auth-chk.jsp'
LOGIN_INFO = {
  'TextUserID' : 'souljit1',
  'TextPassword' : 'thsutleo1!'
}
headers = {
  'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8'
  ,'Accept-Encoding': 'gzip, deflate, br'
  ,'Accept-Language': 'ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7'
  ,'Cache-Control': 'max-age=0'
  ,'Connection': 'keep-alive'
  ,'Content-Length': '2098'
  ,'Content-Type': 'application/x-www-form-urlencoded'
  ,'Cookie': 'ASP.NET_SessionId=ihzbiqczcuholyeajazyfw1z; Login=; UTF8_Option=0; AcSe=0; serverURL=http#3a#2f#2freport#3a8000; skin=classic; ac_userid=schaff; ac_pas_locale_timezone=ko#5fKR#7cKorea#20Standard#20Time; UI=; APRUI=; KMS=; LoginCookie='
  ,'Host': 'portal.konkuk.ac.kr'
  ,'Origin': 'https://portal.konkuk.ac.kr'
  ,'Referer': 'https://portal.konkuk.ac.kr/Login.aspx'
  ,'Upgrade-Insecure-Requests': '1'
  ,'User-Agent': 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.86 Safari/537.36 OPR/60.0.3255.27'
}
with requests.Session() as s:
  req = s.post(url,data=LOGIN_INFO, headers=headers)
  req_2 = requests.get(url2)
  bsObj = bs(req_2.text,'html.parser')
#   if req.status_code==200 and req.ok :
#     print('ok')