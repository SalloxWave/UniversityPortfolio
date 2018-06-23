def binary_search(haystack, needle, fn=lambda x: x):
	result = None
	lst = sorted(haystack[:], key=fn)
	while len(lst) > 0:
		middle = len(lst) // 2
		if needle < fn(lst[middle]):
			lst = lst[:middle]
		elif needle > fn(lst[middle]):
			lst = lst[middle+1:]
		else:
			result = lst[middle]
			break
	return result

print(binary_search(range(21), 5))
print(binary_search(range(1,21), 0))
print(binary_search(['a', 'b', 'c'], 'a'))
print(binary_search(['a', 'b', 'c', 'D'], 'D'))
print(binary_search([{'smth': 2, 'smthelse': 4}, {'smth':3, 'smthelse':6}], 2, lambda x: x['smth']))
print(binary_search([{'smth': 2, 'smthelse': 4}, {'smth':3, 'smthelse':6}], 2, lambda x: x['smthelse']))
print(binary_search([{'smth': 2, 'smthelse': 4}, {'smth':3, 'smthelse':6}], 6, lambda x: x['smthelse']))