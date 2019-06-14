#!/usr/bin/python3
# -*- coding:UTF-8 -*-

import requests
from selenium import webdriver

#15분 49초
options = webdriver.ChromeOptions()
options.add_argument('headless')
options.add_argument('window-size=1920x1080')
options.add_argument("disable-gpu")
driver = webdriver.Chrome('/home/cabox/workspace/Python/PythonAutomation/section2/webDriver/chromedriver')
