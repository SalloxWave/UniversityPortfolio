#!/usr/bin/env python3
#-*- coding: utf-8 -*-
def frame(msg):
	"""Function that outputs text with a frame"""
	star_length = len(msg) + 4
	#alt1
	print("*" * star_length)
	print("*", msg, "*")
	print("*" * star_length)
	#alt2
	#output = "*" * star_length + "\n"
	#output += "* " + msg + " *" + "\n"
	#output += "*" * star_length
	#print(output)
	

frame(input("Write text you want to style: "))

def triangle(height):
	"""Function that outputs a triangle with the desired height"""
	#alt1
	for i in range(height):
		spaces = height - (i+1)
		asterisks = i * 2 + 1
		print(' '*spaces + '*'*asterisks + ' '*spaces)
	#alt2
	#spaces = height - 1
	#asterisks = 1
	#for _ in range(height):
	#	print(' '*spaces + '*'*asterisks + ' '*spaces)
	#	spaces -= 1
	#	asterisks += 2


triangle(int(input("Write the desired height of the triangle: ")))


def flag(size):
	"""Function that builds a flag 22 times wider than the desired input size"""
	part = "*" * (size*11-1)
	for i in range(9):
		if i is not 4:
			print(part + '  ' + part)
		else:
			print()

flag(int(input("Write the desired base size: ")))