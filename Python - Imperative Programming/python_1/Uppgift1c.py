#!/usr/bin/env python3
#-*- coding: utf-8 -*-
dividable = False
testNum = 1
while dividable == False:
	successes = 0
	for modNum in range(1,14):
		if testNum % modNum == 0:
			successes += 1
	if successes == 13:
		dividable = True
	else:
		testNum += 1
print(testNum)