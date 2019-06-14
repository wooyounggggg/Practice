#!/usr/bin/python3
print('content-type:text/html;charset=UTF-8\n')
import urllib.request
import bs4

url = "https://www.naver.com"
html = urllib.request.urlopen(url)
print(html)
bs_obj = bs4.BeautifulSoup(html,"html.parser")
naver_menu = bs_obj.find('ul',{'class':'an_l'})
# print(naver_menu)
span_txts = naver_menu.findAll('span',{'class':'an_txt'})

# for span_txt in span_txts :
#   print(span_txt.text)