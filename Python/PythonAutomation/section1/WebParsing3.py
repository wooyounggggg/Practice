#!/usr/bin/python3
# -*- coding:UTF-8 -*-

from urllib import request
import urllib.parse as parse
from bs4 import BeautifulSoup as bs
import os

queryName = '아이유'
base = 
quote = parse.quote_plus(queryName)
url = base+quote

response = request.urlopen(url)

try :
  if not os.path.isdir(savePath):
    os.makedirs(os.path.join(savePath))
    
except OSError as e:
  if e.errno != errno.EEXIST :
    print('failed folder making!')
    raise

soup = bs(response,'html.parser')

img_lists = soup.select('div.img_area > a.thumb._thumb>img')
for i,img_list in enumerate(img_lists,1):
  fullFileName = os.path.join(savePath,fileName+str(i)+'.jpg')
  request.urlretrieve(img_list['data-source'],fullFileName)

# opener = request.build_opener()
# opener.addheaders = [('User-agent', 'Mozilla/5.0')]
# request.install_opener(opener)