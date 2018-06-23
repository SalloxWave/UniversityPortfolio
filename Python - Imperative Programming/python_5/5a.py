from functools import reduce

def calc_list_badder(fn):
	return reduce(fn, range(1,513))

	#x * y = 1 * 2
	#(1*2) * 3
	#((1*2) * 3) * 4

add_res = calc_list_badder(lambda x,y: x+y)
mult_res = calc_list_badder(lambda x,y: x*y)

#def calc_list_better(fn, lst):
#	return reduce(fn, lst)

#add_res = calc_list_better(lambda x,y: x+y, range(1,513))
#mult_res = calc_list_better(lambda x,y: x*y, range(1,513))

print()
print(add_res)
print()
print(mult_res)
print()

