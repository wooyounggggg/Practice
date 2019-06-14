#!/usr/bin/python3
# -*- coding:UTF-8 -*-

import telegram
import requests
import re
import os
import datetime

def cleanhtml(raw_html):
  raw_html = raw_html.replace('<br />','\n').replace('<a href=\\"','').replace('\\"','')
  raw_html = raw_html.replace('&gt;','>').replace('&lt','<')
  cleanr = re.compile('<.*?>')
  cleantext = re.sub(cleanr, '', raw_html)
  return cleantext

def sendMsg(token, message, chat_id):
  bot = telegram.Bot(token=token)
  bot.sendMessage(chat_id=chat_id,text=message)

def makeMsg(site, title, article):
  msg = '<'+site+'>\n\n'+cleanhtml(title)+'\n\n'+cleanhtml(article)
  return msg

def fileCheck(token, time, message, dataFileDirectory, dataFile, dataFileName):
  if os.path.exists(dataFileDirectory):
    if os.path.exists(dataFile):
      with open(os.path.join(dataFileDirectory, dataFileName), 'r+') as f_read:
        before = f_read.readline()
        if before != time:
          f_read.close()
          sendMsg(token, message,'@crawlingtest')
          with open(os.path.join(dataFileDirectory, dataFileName), 'w+') as f_write:
            f_write.write(time)
            f_write.close()        
        f_read.close()
    else :
      sendMsg(token, message,'@crawlingtest')
      with open(os.path.join(dataFileDirectory, dataFileName), 'w+') as f_write:
            f_write.write(time)
            f_write.close()

  else:
    os.mkdir(dataFileDirectory)
    with open(os.path.join(dataFileDirectory, dataFileName), 'w+') as f_write:
      f_write.write(time)
      sendMsg(token, message,'@crawlingtest')
      f_write.close()

def writeLog(logFileDirectory, logFile, logFileName, title):
  fileMode = 'w+'
  if os.path.exists(logFileDirectory):
    if os.path.exists(logFile):
      fileMode = 'a'
      if os.path.getsize(logFile) > 20000:
        os.remove(logFile)
        fileMode = 'w+'
  else: os.mkdir(logFileDirectory)
  with open(os.path.join(logFileDirectory, logFileName),fileMode) as f_write:
    time_gap = datetime.timedelta(hours=9)
    now=datetime.datetime.utcnow()+time_gap
    log = 'Telegram Log : {}/{}/{} {}:{}:{} <{}>\n'.format(
      str(now.year),str(now.month),str(now.day),
      str(now.hour),str(now.minute),str(now.second),
      cleanhtml(title)
    )
    f_write.write(log)
    f_write.close()