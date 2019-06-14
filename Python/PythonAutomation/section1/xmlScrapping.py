#!/usr/bin/python3
# -*- coding:UTF-8 -*-
import urllib.request as req
from urllib.parse import urlencode

API = 'http://www.mois.go.kr/gpms/view/jsp/rss/rss.jsp'

values = {
  'ctxcd' : '1012'
}

params = urlencode(values)
url = API+'?'+params
reqData = req.urlopen(url).read().decode('utf-8')
print(reqData)

#콜백함수 살펴보기! https://opentutorials.org/course/2136/11861
#과제 : 네이버 홈페이지 상단/우측 광고 저장하기