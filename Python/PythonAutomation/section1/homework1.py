#!/usr/bin/python3
# -*- coding:UTF-8 -*-
import urllib.request as req

imgURL = 'https://ssl.pstatic.net/tveta/libs/1227/1227360/cdfabb67f450354bd932_20190308151556523.jpg'
req.urlretrieve(
  imgURL,
  '/home/cabox/workspace/Python/PythonAutomation/data/homework1-1.jpg')