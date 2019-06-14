#!/usr/bin/python3
# -*- coding:UTF-8 -*-

import requests, json
with requests.Session() as s:
  r = s.get('https://nid.naver.com/nidlogin.login?mode=form&url=https%3A%2F%2Fwww.naver.com')
  print(r)