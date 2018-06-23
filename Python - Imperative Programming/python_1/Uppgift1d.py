#!/usr/bin/env python3
#-*- coding: utf-8 -*-
def checkPrime(testNum):
	"""Function to check if a number is a prime number"""
	if testNum == 2:
		return True

	for modNum in range(2, testNum): 
		if testNum % modNum == 0:
			return False
	return True

primeSum = 0
for i in range(2, 1000):
	if checkPrime(i):
		primeSum += i
print(primeSum)