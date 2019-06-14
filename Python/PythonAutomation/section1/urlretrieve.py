#!/usr/bin/python3
# -*- coding:UTF-8 -*-
import urllib.request as request

imgURL='http://imgnews.naver.net/image/002/2017/03/13/0002026985_001_20170313153101670.jpg'
htmlURL='https://google.com'
savePath = '/home/cabox/workspace/Python/PythonAutomation/data/test1.jpg'
savePath2 = '/home/cabox/workspace/Python/PythonAutomation/data/index.html'
request.urlretrieve(htmlURL,savePath2)
print('다운로드 완료')