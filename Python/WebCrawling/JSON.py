#!/usr/bin/python3

from urllib.parse import urlparse
import requests

def get_api_result(keyword, display, start):
  url = "https://openapi.naver.com/v1/search/blog?query="+keyword+"&display=" +str(display)+str(start)
  result = requests.get(url,headers = {
                         "X-Naver-Client-Id" : "n2MUPDk42zXl_tTFtbbO",
                         "X-Naver-Client-Secret" : "1HWN5PpKFt"
                       })
  return result.json()

keyword = "강남역"
json_obj = get_api_result(keyword,100)
print(json_obj['display'])