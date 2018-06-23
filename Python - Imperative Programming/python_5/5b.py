def dbsearch(db, field, search):
	return [item for item in db if search.lower() in item[field].lower()]

db = [
	{'name': 'Jakob', 'position': 'assistant'},
	{'name': 'Åke', 'position': 'assistant'},
	{'name': 'Ola', 'position': 'examiner'},
	{'name': 'Henrik', 'position': 'assistant'}
]





"""
def old_dbsearch(db, field, search, matching='in'):
	res = []
	for item in db:
		if matching == 'in':
			if search.lower() in item[field].lower():
				res.append(item)
		elif matching == '==':
			if search.lower() == item[field].lower():
				res.append(item)
		elif matching == 'startswith':
			if item[field].lower().startswith(search.lower()):
				res.append(item)
	return res
"""