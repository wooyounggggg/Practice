#!/usr/bin/python3
# -*-coding:UTF-8 -*-

from bs4 import BeautifulSoup as bs

html = '''
<html>
  <body>
    <ul>
      <li><a href='http://naver.com'>naver</li>
      <li><a href='http://daum.net'>daum</li>
      <li><a href='http://google.com'>google</li>
      <li><a href='http://tistory.com'>tistory</li>
    </ul>
  </body>
</html>
'''

soup = bs(html,'html.parser')
a_tags = soup.findAll('a')
b = soup.findAll('a',limit=3)
for a_tag in a_tags:
  print('a_tag["href"] >>',a_tag['href'])
  print('a_tag.attrs >>',a_tag.attrs)
  print('a_tag.text >>',a_tag.text)
  
# https://souljit2.tistory.com