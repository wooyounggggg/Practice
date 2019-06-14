#!/usr/bin/python3
# -*-coding:UTF-8 -*-

from bs4 import BeautifulSoup as bs
import requests

url = 'https://help.coinfinit.com/hc/ko/sections/360002871954-공지사항'
req = requests.get(url)
bs_obj = bs(req.content,'html.parser')
articleList = bs_obj.find('ul',{'class':'article-list'})
