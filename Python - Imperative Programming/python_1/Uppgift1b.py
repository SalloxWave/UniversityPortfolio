#!/usr/bin/env python3
#-*- coding: utf-8 -*-
ans = 1 #måste börja med 1, annars blir det 0*number
for i in range(1, 513):
	ans = ans*i
print(ans)