#!/usr/bin/python3
# -*-coding:UTF-8 -*-

from urllib.parse import urljoin
from bs4 import BeautifulSoup as bs

# baseURL = 'http://test.com/html/a.html'
# print('>>',urljoin(baseURL,'sub/b.html')) #urljoin : 절대경로를 제외하고 경로를 치환해서 자동 적용
html = '''
<html>
  <body>
    <h1>파이썬 BeautifulSoup Study</h1>
    <p>태그 선택자</p>
    <p>CSS 선택자</p>
  </body>
</html>
'''

soup = bs(html,'html.parser')
# print('prettify',soup.prettify())
h1 = soup.html.body.h1
print('h1',h1)
p1 = soup.html.body.p
p2 = p1.next_sibling.next_sibling
print('p1 :',p1,'p2 :',p2)
p3 = p1.previous_sibling.previous_sibling
print('p3 :',p3)

print('h1 >>',h1,'\np1 >>',p1,'\np2 >>',p2,'\np3 >>',p3)