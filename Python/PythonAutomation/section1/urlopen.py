#!/usr/bin/python3
# -*- coding:UTF-8 -*-
import urllib.request as request

imgURL='http://imgnews.naver.net/image/002/2017/03/13/0002026985_001_20170313153101670.jpg'
htmlURL='https://google.com'
savePath = '/home/cabox/workspace/Python/PythonAutomation/data/test1.jpg'
savePath2 = '/home/cabox/workspace/Python/PythonAutomation/data/index.html'

f = request.urlopen(imgURL).read()
f2 = request.urlopen(htmlURL).read()

with open(savePath2,'wb') as saveFile2: #with문을 벗어날 때에 saveFile2가 자동으로 close 처리됨.
  saveFile2.write(f2) 
# saveFile1 = open(savePath,'wb') #w:write, r:read, a:add b:binary
# saveFile1.write(f)
# saveFile1.close()

print('다운로드 완료!')

# URL open vs URL retrieve
# URL open : 변수할당 -> 파싱 -> 저장(db,...) : 파싱 필요한 data에 사용하면 좋음
# URL retieve : 저장 -> open(r) -> 변수할당 -> 파싱 -> 저장 : 파싱 필요없는 data에 사용하면 좋음