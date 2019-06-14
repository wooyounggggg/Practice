#!/usr/bin/python3
print('content-type:text/html;charset=UTF-8\n')

import bs4
html_str = "<html><div>hello</div></html>"
bs_obj = bs4.BeautifulSoup(html_str, "html.parser")

print("--------------------------------------")
print("ex_1 : Finding element")
print("type(bs_obj) :",type(bs_obj))
print("bs_obj :",bs_obj)
print('bs_obj.find("div") :',bs_obj.find("div"))
print("--------------------------------------")
print("ex_2 : Finding elements list")
html_str = """
<html>
  <body>
    <ul>
      <li>hello</li>
      <li>bye</li>
      <li>welcome</li>
    </ul>
  </body>
</html>
"""

bs_obj = bs4.BeautifulSoup(html_str,"html.parser")

ul = bs_obj.find("ul")
li = ul.find("li")
print("li :",li)
print("li.text :",li.text)
liList = ul.findAll("li")
print("ul.findAll(\"li\") :",liList)
index=0
for li in liList :
  print("liList[",index,"] :",li)
  index=index+1

print("--------------------------------------")

print("ex_3 : Finding element by property")
html_str = """
<html>
  <body>
    <ul class="greet">
      <li>hello</li>
      <li>bye</li>
      <li>welcome</li>
    </ul>
    
    <ul class="reply">
      <li>ok</li>
      <li>no</li>
      <li>sure</li>
    </ul>
  </body>
</html>
"""
bs_obj = bs4.BeautifulSoup(html_str,"html.parser")

ul = bs_obj.find("ul",{"class":"reply"})
print(ul)
print(type(ul))

print("--------------------------------------")
html_str = """
<html>
  <body>
    <ul class="ko">
      <li>
        <a href="https://www.naver.com">네이버</a>
      </li>
      <li>
        <a href="https://www.daum.net">다음</a>
      </li>
    </ul>
    <ul class="sns">
      <li>
        <a href="https://www.google.com">구글</a>
      </li>
      <li>
        <a href="https://www.facebook.com">페이스북</a>
      </li>
    </ul>
  </body>
</html>
"""
bs_obj = bs4.BeautifulSoup(html_str,"html.parser")

firstURL = bs_obj.find("ul",{"class":"ko"})
naver = firstURL.findAll("a")[0]
sites=["naver","daum"]

for idx,site in enumerate(sites) :
  siteURL = {site : firstURL.findAll("a")[idx]}
print(siteURL["daum"]["href"])
