#!/usr/bin/python3
from tkinter import *
import matplotlib
matplotlib.use('Agg')
def printHello():
  print('hi')

root = Tk()

w = Label(root, text="Python Test")
b = Button(root, text="Hello Python!", command=printHello)
c = Button(root, text="Quit", command=root.quit)

w.pack()
b.pack()
c.pack()
root.mainloop()