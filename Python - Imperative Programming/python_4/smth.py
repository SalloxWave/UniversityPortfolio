from copy import deepcopy
import curses

def sokoban_load(filename):
	with open(filename, 'r') as level_file:
		level_raw = level_file.read().splitlines()

	static = []
	dynamic = {
		'player': {},
		'crates': []
	}
	y = 0
	for line_str in level_raw:
		static.append({})
		x = 0
		for char in line_str:
			if char is not ' ':
				if char is '@':
					dynamic['player'] = {'char': '@', 'x': x, 'y': y}
				elif char is 'o':
					dynamic['crates'].append({'char': 'o', 'x': x, 'y': y})
				else:
					static[y][x] = char
			x += 1
		y += 1
	state = {'static': static, 'dynamic': dynamic}
	return state

def display_state(state):
	grid = deepcopy(state['static'])

	for obj in state['dynamic']:
		if type(state['dynamic'][obj]) is list:
			for obj_item in state['dynamic'][obj]:
				x = obj_item['x']
				y = obj_item['y']
				grid[y][x] = obj_item['char']
		else:
			x = state['dynamic'][obj]['x']
			y = state['dynamic'][obj]['y']
			grid[y][x] = state['dynamic'][obj]['char']

	lines = ''
	for y in grid:
		line = ''
		max_x = max(y.keys())
		for i in range(max_x + 1): # +1 för att inkludera max-värdet på x
			if i in y:
				line += y[i]
			else:
				line += ' '
		lines += line + '\n'
	return lines









state = sokoban_load('levels/first_level.sokoban')

stdscr = curses.initscr()
curses.noecho()
curses.cbreak()
stdscr.keypad(1)

stdscr.addstr(display_state(state))
stdscr.addstr('\nUse \'hjkl\' to move (\'q\' to quit): ')
stdscr.refresh()

key = None
while key != ord('q'):
	key = stdscr.getch()
	stdscr.refresh()
	#if key is ord('h'):
	#	stdscr.addstr('\nDerp')
	#elif key is ord('j'): 

curses.endwin()












def player_move(state, player, direction):

	old_x = player['x']
	old_y = player['y']

	if direction is 'l':
		new_x = player['x'] - 1
		new_y = old_y
	elif direction is 'r':
		new_x = player['x'] + 1
		new_y = old_y
	elif direction is 'u':
		new_x = old_x
		new_y = player['y'] - 1
	elif direction is 'd':
		new_x = old_x
		new_y = player['y'] + 1

	#if player_can_move(state, new_x, new_y):
		#collision_handler(player):


		del state[old_y][old_x]
		state[new_y][new_x] = player

def player_can_move(state, x, y, direction):
	can_move = True
	if y > -1 and y < len(state):
		if x not in state[y]:
			can_move = True
		elif state[y][x] is 'o':
			if crate_can_move(state, direction):
				can_move = True
			else:
				can_move = False
		elif state[y][x] is '#':
			can_move = False
		else:
			can_move = True
	return can_move

#def collision_handler(obj1, obj2):
	#if obj1['type'] is 'player' and obj2['type'] is 'crate':

