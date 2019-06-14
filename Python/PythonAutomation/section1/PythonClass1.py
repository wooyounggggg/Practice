#!/usr/bin/python3
# -*-coding:UTF-8 -*-

# --------------------------------------------
# Index
# 1.Class
# 2.Class Constructor
# 3.Definition of 'Self'
# 4.Class Name Space
# 5.Class Variable vs Instance Variable
# --------------------------------------------

# 1.Class
# Object : Car
# Class : Car Blueprint
# Field : Velocity, Automatic, Size, ...
# Method : accelerate, go left, go right, Break, ...
# Instance of Class = Object


# 2.Class Constructor
class UserInfo: #Class Declaration
  def __init__(self, name, phone): #생성되는 순간에 실행
    self.name = name
    self.phone = phone
    
  def set_Info(self, name, phone):
    self.name = name
    self.phone = phone
  
  def get_Info(self):
    return [self.name,self.phone]
  
#   def __del__(self): #garbage collection시 삭제되는 순간에 실행되는 함수
#     print('a')
    
user1 = UserInfo('Kim','010-1234-5678')
user2 = UserInfo('Lee','010-2345-7890')

# print(user1.get_Info())
# print(user2.get_Info())
# print(user1.__dict__) #namespace of user1
# print(user2.__dict__) #namespace of user2

# 3.Self
class selfTest:
  def function1():
    print('function1 called!')
    
  def function2(self):
    print(id(self))
    print('function2 called!')

f = selfTest()
# print(dir(f))
# print(f.__dict__)
# print(id(f))
# print(selfTest.function1()) #self는 인자로 넘기지 않아도 자동으로 함께 넘어감.
# f.function2()
# print(id(f)) 

# 5.Class variable vs Instance variable
class Warehouse:
  stock_num = 0
  def __init__(self, name):
    self.name = name
    Warehouse.stock_num += 1
  def __del__(self):
    Warehouse.stock_num -= 1
    
user1 = Warehouse('kim')
user2 = Warehouse('park')

# print(user1.name)
# print(user2.name)
# print(user1.__dict__)
# print(user2.__dict__)
# print(Warehouse.__dict__) # stock_num은 Warehouse 인스턴스의 갯수를 의미한다.
# print(user1.stock_num)

# 중요! : class 변수인 stock_num은 [공유되는 자원]이고,
# Instance 변수인 name은 [공유되지 않는 자원]이다.
# Instance에서 Class 변수에 접근할 수는 있는데 namespace에는 없음!!
# 컴파일러는 Instance namespace -> class namespace순으로 탐색.

# 4.namespace 
class NameTest:
  total = 0
  
print(dir())
print('before : ',NameTest.__dict__)
NameTest.total = 1
print('after : ',NameTest.__dict__)

n1 = NameTest()
n2 = NameTest()
print(id(n1),'vs',id(n2))
print(dir())
print('n1.__dict :',n1.__dict__) #암것도 안나옴
print('n1.__dict :',n2.__dict__)
n1.total = 77
print('n1.__dict :',n1.__dict__) #

print(n1.total) #77 출력 : 인스턴스 네임스페이스에서 이미 77 발견!
print(n2.total) #1 출력  : 인스턴스 네임스페이스에 암것도 없음 -> 클래스 네임스페이스로
