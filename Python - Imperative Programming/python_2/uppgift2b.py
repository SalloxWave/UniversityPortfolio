#!/usr/bin/env python3
def create_shopping_list():
	return ["Kurslitteratur","Anteckningsblock","Penna"]

def shopping_list(slist):
	for i in range(len(slist)):
		print(str(i+1) + ". " + slist[i])

def shopping_add(slist):
	sitem = input("Vad ska läggas till i listan? ")
	slist.append(sitem)

def shopping_remove(slist):
	sindex = int(input("Vilken sak vill du ta bort ur listan? ")) - 1
	del slist[sindex]

def shopping_edit(slist):
	sindex = int(input("Vilken sak vill du ändra på? ")) - 1
	sitem = input("Vad ska det stå istället för \"" + slist[sindex] + "\"? ")
	slist[sindex] = sitem