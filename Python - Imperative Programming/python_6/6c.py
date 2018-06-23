def insertion_sort(lst, fn = lambda x: x):
	for i in range(1, len(lst)):
		j = i
		while j > 0 and fn(lst[j]) < fn(lst[j-1]):
			lst[j], lst[j-1] = lst[j-1], lst[j]
			j -= 1
	return lst

db = [
	7,5,4,1,3,2
]
#5,7,4,1,3,2

#5,4,7,1,3,2
#4,5,7,1,3,2

#4,5,1,7,3,2
#4,1,5,7,3,2
#1,4,5,7,3,2

#1,4,5,3,7,2
#1,4,3,5,7,2
#1,3,4,5,7,2

#1,3,4,5,2,7
#1,3,4,2,5,7
#1,3,2,4,5,7
#1,2,3,4,5,7

db2 = [
	('j', 'g'), ('a', 'u'), ('k', 'l'), ('o', 'i'),
	('b', 's'), ('@', '.'), ('p', 's'), ('o', 'e')
]

print("db:",db)
insertion_sort(db)
print("db:",db)

print("db2:",db2)
insertion_sort(db2, lambda x: x[0])
print("db2:",db2)