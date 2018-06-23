def generate_list(fn, count):
	return [fn(i+1) for i in range(count)]

def mirror(x): return x
mirror_lst = generate_list(mirror, 4)
star_lst = generate_list(lambda x: '*' * x, 5)
