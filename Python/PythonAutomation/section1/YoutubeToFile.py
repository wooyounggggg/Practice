#!/usr/bin/python3
# -*- coding:UTF-8 -*-

import pytube
import os
import subprocess #terminal의 명령어를 코드로 입력할 수 있게 하는 library!

yt = pytube.YouTube('https://www.youtube.com/watch?v=OxgiiyLp5pk')
videos = yt.streams.all()

for i,video in enumerate(videos) :
  print(i,':',video)

userInput = int(input('input(0~21) : '))
down_dir = '/home/cabox/workspace/Python/PythonAutomation/section1/Youtube'
videos[userInput].download(down_dir)

newFileName = input('저장할 mp3 파일명: ')
oriFileName = videos[userInput].default_filename

subprocess.call([
  'ffmpeg','-i',
  os.path.join(down_dir,oriFileName),
  os.path.join(down_dir,newFileName)
])

print('complete!')