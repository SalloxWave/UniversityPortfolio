imdb = [
	{ 'title': 'Raise your voice', 'actor': 'Hilary Duff', 'score': 10 },
	{ 'title': 'I\'m a Cyborg, but that\'s OK' , 'actor': 'Su-jeong Lim', 'score': 7. },
	{ 'title': 'Kung Pow', 'actor': 'Steve Oedekerk', 'score': 6}
]

def linear_search(haystack, needle, fn=None):
	
	for item in haystack:
		if fn is None:
			for field in item:
				if str(needle).lower() in str(item[field]).lower():
					return item
		else:
			if str(needle).lower() in str(fn(item)).lower():
				return item
	return None



a = linear_search(imdb, 10)
b = linear_search(imdb, 10, lambda item: item['score'])
c = linear_search(imdb, 's')
d = linear_search(imdb, 's', lambda item: item['actor'])
e = linear_search(imdb, 'Kung', lambda item: item['title'])