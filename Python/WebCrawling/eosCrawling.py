#!/usr/bin/python3
print('content-type:text/html;charset=UTF-8\n')

import requests
import time
from bs4 import BeautifulSoup

result = requests.get('https://bp.eosgo.io')
bs_obj = BeautifulSoup(result.content,'html.parser')
lf_contacts = bs_obj.findAll('div',{'class':'lf-item'})
hrefs = [a_tag.find('a')['href'] for a_tag in lf_contacts]

def get_bp_info(url) :
  result = requests.get(url)
  bs_obj = BeautifulSoup(result.content, 'html.parser')

  #extract title : staked
  profile_name = bs_obj.find('div',{'class':'profile-name'})
  h1_bp_name = profile_name.find('h1')
  bp_name = h1_bp_name.text

  #extract location
  cover_buttons = bs_obj.find('div',{'class':'cover-buttons'})
  button_label = cover_buttons.find('span',{'class':'button-label'})
  location = button_label.text

  #extract web-site link
  button_labels = bs_obj.find('div',{'class':'cover-buttons'})
  a_tag = button_labels.find('a')
  link = a_tag['href']
  
  dictionary = {}
  dictionary['name'] = bp_name
  dictionary['location'] = location
  dictionary['link'] = link
  return dictionary
sites_info = [get_bp_info(href) for href in hrefs]
# print(hrefs[0:5])
for site_info in sites_info :
  print(site_info)