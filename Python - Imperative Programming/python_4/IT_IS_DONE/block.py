def get_block(state, x, y):
	return state['grid'][y].get(x) #Returns None if x-index does not exist

def check_solidity(block):
	if block is not None:
		for obj in block:
			if obj['solid'] is True:
				return True
	return False