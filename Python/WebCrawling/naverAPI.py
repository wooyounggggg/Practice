#!/usr/bin/python3

import requests
from urllib.parse import urlparse
client_id = "n2MUPDk42zXl_tTFtbbO"
client_secret = "1HWN5PpKFt"

def searchText():
  keyword = "강남역"
  url = "https://openapi.naver.com/v1/search/blog?query=" + keyword
  result = requests.get(urlparse(url).geturl(),
                      headers = {"X-Naver-Client-Id" : "n2MUPDk42zXl_tTFtbbO",
                                "X-Naver-Client-Secret" : "1HWN5PpKFt"})
  
  print(result.json())
  
searchText()