def partial(fn, val):
	return lambda x: fn(val, x)

def add(n, m): return n + m

add_five = partial(add, 5)

print(add_five(3))