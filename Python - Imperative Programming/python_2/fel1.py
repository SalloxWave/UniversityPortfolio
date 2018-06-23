#!/usr/bin/env python3
# -*- coding: utf-8 -*-

def factor(multiplicator):
    return 10*multiplicator

def fraction(divisor):
    return 10.0 / divisor

def power(exponent):
    return 10**exponent

#BUG
#SyntaxError: Missing parentheses in call to 'print'
#print-satsen måste skrivas med parantes runt texten
#print "with x = 1 to 10:"
#FIX
print("with x = 1 to 10:")

# print 10 multiplicated by numbers 1 to 10
numbers = []
for x in range(10):
	#BUG
	#NameError: name 'facor' is not defined
	#facor är inte definierad, ska antagligen vara factor
    #numbers.append(str(facor(x)))
    #FIX
    numbers.append(str(factor(x)))
print("   10 multiplicated with x:", ", ".join(numbers))

# print 10 divided by the numbers 1 to 10
numbers = []
#BUG
#ZeroDivisionError: float division by zero
#range(10) gör att division med 0 kommer försöka utföras i den första iterationen
#for x in range(10):
for x in range(1,11):
	numbers.append(str(fraction(x)))
print("   10 divided by x:", ", ".join(numbers))

# print 10 raised to the power of the numbers 1 to 10
numbers = []
for x in range(10):
	#BUG
	#TypeError: sequence item 0: expected str instance, int found
	#Resultatet från power(x) måste konverteras till en sträng för att kunna joinas i printsatsen
    #numbers.append(power(x))
    numbers.append(str(power(x)))
print("   10 raised to x:", ", ".join(numbers))