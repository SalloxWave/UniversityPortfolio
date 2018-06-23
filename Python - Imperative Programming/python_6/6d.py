"""def old_quicksort(lst, lo = None, hi = None, fn = lambda x: x):
	if lo is None: lo = 0
	if hi is None: hi = len(lst)-1
	if lo < hi:
		pivot = hi
		i = lo
		for j in range(lo, hi):
			if fn(lst[j]) <= fn(lst[pivot]):
				lst[i], lst[j] = lst[j], lst[i]
				i += 1
		lst[i], lst[pivot] = lst[pivot], lst[i]
		new_quicksort(lst, lo, i-1, fn)
		new_quicksort(lst, i+1, hi, fn)"""

def quicksort(lst, lo = None, hi = None, fn = lambda x: x):
	if lo is None: lo = 0
	if hi is None: hi = len(lst)-1
	if lo < hi:
		pivot = partition(lst, lo, hi, fn)
		quicksort(lst, lo, pivot-1, fn)
		quicksort(lst, pivot+1, hi, fn)

def partition(lst, lo, hi, fn):
	pivot = hi
	i = lo
	for j in range(lo, hi):
		if fn(lst[j]) <= fn(lst[pivot]):
			lst[i], lst[j] = lst[j], lst[i]
			i += 1
	print(i)
	lst[i], lst[pivot] = lst[pivot], lst[i]
	return i

#7,8,9,5,5
#5,8,9,7,5
#5,5,9,7,8
#5,5,8,7,9

#...,8,7,9
#...,7,8,9
lst = [5, 8, 9, 11, 77, 3, 3,7,8,5,2,1,9,5,4,9]
#lst = [3,7,8,5,2,1,9,5,4]
quicksort(lst)
print(lst)