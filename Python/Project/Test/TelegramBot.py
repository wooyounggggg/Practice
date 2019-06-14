#!/usr/bin/python3
# -*- coding:UTF-8 -*-

from moduleCollection import *
#base info.
headers = {
  'Accept': 'application/json, text/javascript, */*; q=0.01',
  'Accept-Encoding': 'gzip, deflate, br',
  'Accept-Language': 'ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7',
  'Connection': 'keep-alive',
  'Cookie': '817eebc5b49c13f8e6a0a7d159a49c09=WTI5cGJtSnBkQzVqYnk1cmNn; 4994a8ffeba4ac3140beb89e8d41f174=a28%3D; Language=a28%3D',
  'Host': 'www.coinbit.co.kr',
  'Referer': 'https://www.coinbit.co.kr/customer/main',
  'User-Agent': 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36 OPR/58.0.3135.127',
  'X-Requested-With': 'XMLHttpRequest'
}
url = 'https://www.coinbit.co.kr/webbbsmain/noticelists/chno-100/&page=1/&subject='
telegram_token = '826396772:AAEY3s9YEN5yTl5qE_H6AMP_rkbwolDsdkY'
site = 'coinbit'
BASE_DIR = os.path.dirname(os.path.abspath(__file__)) #~/Project/TelegramBot

dataFileDirectory = BASE_DIR+'/data'
dataFileName = site+'_data.txt'
dataFile = dataFileDirectory+'/'+dataFileName

logFileDirectory = BASE_DIR+'/log'
logFileName = site+'_log.txt'
logFile = logFileDirectory+'/'+logFileName

#Crawling
result = requests.get(url, headers=headers)
json_obj=result.json()

# Title, Article, time Load
title = json_obj[0]['subject']
article = json_obj[0]['content_kr']
time = json_obj[0]['reg_dt']

message = makeMsg(site, title, article)
fileCheck(telegram_token, time, message, dataFileDirectory, dataFile, dataFileName)
writeLog(logFileDirectory, logFile, logFileName, title)