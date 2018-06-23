from copy import deepcopy
import mod_block
import mod_game

def create_obj(state, definition, x, y):
	obj = deepcopy(definition)
	add_obj(state, obj, x, y)
	if obj['type'] is 'player':
		state['player'] = obj
	if obj['type'] is 'storage':
		state['storages'].append(obj)

def move_obj(state, mv_obj, xsteps=0, ysteps=0):
	old_x = mv_obj['x']
	old_y = mv_obj['y']
	tar_x = old_x + xsteps
	tar_y = old_y + ysteps
	#Execute any collision interactions between moving object and the objects in target block
	tar_block = mod_block.get_block(state, tar_x, tar_y)
	if tar_block is not None:
		mod_game.pre_collision_handler(state, mv_obj, tar_block, xsteps, ysteps)
	#Execute the move if target block is not occupied by a solid object
	tar_block = mod_block.get_block(state, tar_x, tar_y) #Refresh tar_block as it might have changed during collision
	if mod_block.check_solidity(tar_block) is False:
		add_obj(state, mv_obj, tar_x, tar_y)
		del_obj(state, mv_obj, old_x, old_y)
		mod_game.post_collision_handler(state, tar_block)

def add_obj(state, obj, x, y):
	grid = state['grid']
	obj['x'] = x
	obj['y'] = y
	if x not in grid[y]:
		#Create block able to hold multiple items
		grid[y][x] = []
	grid[y][x].append(obj)

def del_obj(state, obj, x, y):
	grid = state['grid']
	if len(grid[y][x]) > 1:
		grid[y][x].remove(obj)
	else:
		del grid[y][x]