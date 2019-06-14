#!/usr/bin/python3
# -*- coding:UTF-8 -*-

from bs4 import BeautifulSoup
import re #regex
html = '''
<html>
  <body>
    <ul>
      <li><a id='naver' href='http://naver.com'>naver</li>
      <li><a href='http://daum.net'>daum</li>
      <li><a href='https://google.com'>google</li>
      <li><a href='https://tistory.com'>tistory</li>
    </ul>
  </body>
</html>
'''
soup = BeautifulSoup(html,'html.parser')

lis = soup.findAll(href=re.compile(r'da')) # 'r' means 'raw string'
for li in lis:
  print(li['href'])