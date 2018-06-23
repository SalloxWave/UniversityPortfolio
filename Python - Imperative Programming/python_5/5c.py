def contains(needle, haystack):
	filtered = list(filter(lambda straw: straw == needle, haystack))
	return len(filtered) > 0

def old_contains(needle, haystack):
	booleans = list(map(lambda straw: straw == needle, haystack)) #list() not needed in this case, sorted() returns a list
	return sorted(booleans)[-1]


haystack = 'Can you find the needle in this haystack?'.split()