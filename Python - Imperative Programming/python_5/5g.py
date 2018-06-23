def partial(fn, val):
	return lambda x: fn(val, x)

def compose(fn1,fn2):
	return lambda x: fn1(fn2(x))

def make_filter_map(filter_fn, map_fn):
	#filter(filter_fn, ...)
	part_filter = partial(filter, filter_fn)
	#map(map_fn, ...)
	part_map = partial(map, map_fn)
	#part_map(part_filter(...)) = map(map_fn,filter(filter_fn, ...))
	composition = compose(part_map, part_filter)
	#map(map_fn,filter(filter_fn, lst))
	#Om det är ojämnt tal, kvadrera
	return lambda lst: list(composition(lst))

#def make_filter_map(filter_fn, map_fn):
#	return lambda lst: list(map(map_fn, filter(filter_fn, lst)))

process = make_filter_map(lambda x: x % 2 == 1, lambda x: x * x)
print(process(range(10)))

#map(map_fn, filter(filter_fn, range(10)))
#==> map(map_fn, [1,3,5,7,9])
#====> [1, 9, 25, 49, 81]