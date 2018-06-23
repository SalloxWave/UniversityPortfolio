def compose(fn1,fn2):
	return lambda x: fn1(fn2(x))

def multiply_five(n):
	return n * 5

def add_ten(x):
	return x + 10

composition = compose(multiply_five, add_ten)

another_composition = compose(add_ten, multiply_five)

print(composition(3))

print(another_composition(3))