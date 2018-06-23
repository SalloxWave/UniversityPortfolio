#!/usr/bin/env python3
#-*- coding: utf-8 -*-
def temp():
	if __name__ == '__main__':
		print("Running as a program")
		print("__name__ = " + __name__)
	else:
		print("Running as a module")
		print("__name__ = " + __name__)

def num_stuff(x):
	"""Adds 5 to the supplied number"""
	return x + 5

if __name__ == '__main__':
	temp()
#temp()