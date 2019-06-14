#!/usr/bin/python3
# -*- coding:UTF-8 -*-

from bs4 import BeautifulSoup

fp = open('food-list.html',encoding='utf-8')
soup = BeautifulSoup(fp,'html.parser')
# print(soup)
print('1',soup.select_one('li:nth-of-type(5)'))
print('2',soup.select_one('#ac-list > li:nth-of-type(4)'))
print('3',soup.select('#ac-list>li[data-lo="cn"]')[0].string)
print('4',soup.select('#ac-list>li.alcohol.high'))

param = {'data-lo':'cn','class':'alcohol'}
print('5',soup.find('li',param).string)
print('6',soup.find(id='ac-list').find('li',param).string)

for ac in soup.findAll('li'):
  if ac['data-lo']=='us':
    print('data-lo == us',ac.string)