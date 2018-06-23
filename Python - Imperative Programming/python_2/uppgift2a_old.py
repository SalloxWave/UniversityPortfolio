#!/usr/bin/env python3
#-*- coding: utf-8 -*-

def frame(inputStr):
	mainStr = '* ' + inputStr + ' *'
	asterisks = ''
	for char in mainStr:
		asterisks += '*'
	print(asterisks + '\n' + mainStr + '\n' + asterisks)

frame('en massa asterisker')

def triangle(num):
	asterisks = '*'
	for i in range(1, num+1):
		print(asterisks)
		asterisks += '**'

triangle(3)

#flag - bilden är ju bara 21 gånger bredden? ska man avrunda nedåt om det inte går att få den symmetriskt 22 gånger?