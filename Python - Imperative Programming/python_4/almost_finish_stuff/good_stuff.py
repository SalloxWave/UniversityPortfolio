from copy import deepcopy
import sys
import os
import readchar #req: pip install readchar
import sokoban_data

def sokoban_load(data, level_no):
	level_path = data.levels['level_path'].format(level_no)
	with open(level_path, 'r') as level_file:
		level = level_file.read().splitlines()
	state = {
		'levels': data.levels,


		'level_no': level_no,
		'grid': [],
		'player': None,
		'storages': []
	}

	y = 0
	for line in level:
		state['grid'].append({})
		x = 0
		for char in line:
			if char is not ' ':
				obj = deepcopy(data.objects[char])
				obj['x'] = x
				obj['y'] = y
				add_obj(state, obj, x, y)
				if obj['type'] is 'player':
					state['player'] = obj
				if obj['type'] is 'storage':
					state['storages'].append(obj)
			x += 1
		y += 1
	return state

def display(state):
	os.system('clear')
	grid = state['grid']
	for y in range(len(grid)):
		for x in grid[y]:
			top = grid[y][x][0]
			char = 'default'
			if len(grid[y][x]) > 1:
				for i in range(1,len(grid[y][x])):
					if len(grid[y][x]) > 1:
						if grid[y][x][i]['z-index'] > top['z-index']:
							bottom = grid[y][x][i-1]
							top = grid[y][x][i]
							if bottom['type'] in top['chars']:
								char = bottom['type']
			print('\033[' + str(y+3) + ';' + str(x+5) + 'H' + top['chars'][char] + '\n\n')

def obj_move(state, mv_obj, xsteps=0, ysteps=0):
	old_x = mv_obj['x']
	old_y = mv_obj['y']
	tar_x = old_x + xsteps
	tar_y = old_y + ysteps
	#Execute any collision interactions between moving object and the objects in target block
	tar_block = get_block(state, tar_x, tar_y)
	if tar_block is not None:
		pre_collision_handler(state, mv_obj, tar_block, xsteps, ysteps)
	#Execute the move if target block is not occupied by a solid object
	tar_block = get_block(state, tar_x, tar_y) #Refresh tar_block as it might have changed during collision
	if check_solidity(tar_block) is False:
		add_obj(state, mv_obj, tar_x, tar_y)
		del_obj(state, mv_obj, old_x, old_y)
		post_collision_handler(state, tar_block)

def add_obj(state, obj, x, y):
	grid = state['grid']
	obj['x'] = x
	obj['y'] = y
	if x not in grid[y]:
		grid[y][x] = []
	grid[y][x].append(obj)

def del_obj(state, obj, x, y):
	grid = state['grid']
	if len(grid[y][x]) > 1:
		for i in range(len(grid[y][x])):
			if grid[y][x][i] is obj:
				del grid[y][x][i]
	else:
		del grid[y][x]

def get_block(state, x, y):
	return state['grid'][y].get(x)

def check_solidity(block):
	if block is not None:
		for obj in block:
			if obj['solid'] is True:
				return True
	return False

def pre_collision_handler(state, mv_obj, tar_block, xsteps, ysteps):
	for tar_obj in tar_block:
		if mv_obj['type'] is 'player' and tar_obj['type'] is 'crate':
			obj_move(state, tar_obj, xsteps, ysteps)

def post_collision_handler(state, block):
	if block is not None:
		objs = []
		for obj in block:
			 objs.append(obj['type'])
		if 'crate' in objs and 'storage' in objs:
			try_complete(state)

def try_complete(state):
	empty_storages = len(state['storages'])
	for s_obj in state['storages']:
		s_block = get_block(state, s_obj['x'], s_obj['y'])
		for obj in s_block:
			if obj['type'] is 'crate':
				empty_storages -=1
	if empty_storages is 0:
		complete(state)

def complete(state):
	os.system('clear')
	print('\n Hooray! You\'ve completed level {}!'.format(state['level_no']))
	level_no = get_level_no(state)
	state = sokoban_load(data=sokoban_data, level_no=level_no)

def get_level_no(state=None, herpaderp=True):
	old_level_no = 0
	levels_amount = len([file for file in os.listdir(state['levels']['folder_path']) if os.path.isfile(os.getcwd() + '/' + state['levels']['folder_path'] + file) and file.endswith('.sokoban')])
	if state is not None:
		old_level_no = state['level_no']
	if old_level_no < levels_amount and herpaderp:
		new_level_no = input("\n Select a level (1-{}) or leave empty for level {} | quit = q: ".format(levels_amount, old_level_no+1))
		if new_level_no == 'q':
			os.system('clear')
			sys.exit()
		if new_level_no == '':
			new_level_no = old_level_no + 1
		else:
			new_level_no = int(new_level_no)
	else:
		levels_amount = len([file for file in os.listdir(state['levels']['folder_path']) if os.path.isfile(os.getcwd() + '/' + state['levels']['folder_path'] + file) and file.endswith('.sokoban')])
		new_level_no = input("\n Select a level (1-{}) | quit = q: ".format(levels_amount))
		if new_level_no == 'q':
			os.system('clear')
			sys.exit()
		else:
			new_level_no = int(new_level_no)
	return new_level_no

os.system('clear')
print('\n Welcome to Sokoban!')
state = sokoban_load(data=sokoban_data, level_no=1)
level_no = get_level_no(state, herpaderp=False)
state = sokoban_load(data=sokoban_data, level_no=level_no)
while True:
	display(state)
	print(" move = hjkl/wasd | restart = r | give up = q\n")
	key = readchar.readchar()
	if key in ('h','a'):
		obj_move(state, state['player'], xsteps=-1)
	elif key in ('j','s'):
		obj_move(state, state['player'], ysteps=1)
	elif key in ('k', 'w'):
		obj_move(state, state['player'], ysteps=-1)
	elif key in ('l','d'):
		obj_move(state, state['player'], xsteps=1)
	elif key in 'r':
		state = sokoban_load(data=sokoban_data, level_no=level_no)
	elif key in 'q':
		level_no = get_level_no(state, herpaderp=False)
		state = sokoban_load(data=sokoban_data, level_no=level_no)