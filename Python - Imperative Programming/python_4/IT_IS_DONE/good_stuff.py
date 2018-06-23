from readchar import readchar #req: pip install readchar
from termcolor import colored #req: pip install termcolor
from copy import deepcopy
import sys
import os

import modify
import menu
import sokoban_data

def sokoban_load(level_no):
	level_file = sokoban_data.levels['file_path'].format(level_no)
	with open(level_file) as f:
		level = f.read().splitlines()
	state = {
		'complete': False,
		'current_level': level_no,
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
				definition = sokoban_data.objects[char]
				if type(definition) is dict:
					create_obj(state, definition, x, y)
				elif type(definition) is tuple:
					for def_char in definition:
						create_obj(state, sokoban_data.objects[def_char], x, y)
			x += 1
		y += 1
	return state

def get_block(state, x, y):
	return state['grid'][y].get(x) #Returns None if x-index does not exist

def check_block_solidity(block):
	if block is not None:
		for obj in block:
			if obj['solid'] is True:
				return True
	return False

def pre_collision_handler(state, mv_obj, tar_block, xsteps, ysteps):
	for tar_obj in tar_block:
		if mv_obj['type'] is 'player' and tar_obj['type'] is 'crate':
			move_obj(state, tar_obj, xsteps, ysteps)

def post_collision_handler(state, block):
	if block is not None:
		obj_types = []
		for obj in block:
			 obj_types.append(obj['type'])
		if 'crate' in obj_types and 'storage' in obj_types:
			try_complete(state)

def try_complete(state):
	empty_storages = len(state['storages'])
	for s_obj in state['storages']:
		s_block = get_block(state, s_obj['x'], s_obj['y'])
		for obj in s_block:
			if obj['type'] is 'crate':
				empty_storages -= 1
	if empty_storages is 0:
		state['complete'] = True

def display_state(state):
	os.system('clear')
	grid = state['grid']
	for y in range(len(grid)):
		for x in grid[y]:
			if len(grid[y][x]) > 1:
				grid[y][x].sort(key=lambda obj: obj['z-index'], reverse=True)
				top_obj = grid[y][x][0]
				bottom_obj = grid[y][x][1]
				char_key = bottom_obj['type']
			else:
				top_obj = grid[y][x][0]
				char_key = 'default'
			print('\033[' + str(y+3) + ';' + str(x+5) + 'H' + colored(top_obj['chars'][char_key], top_obj['color']))

def run_game(state):
	this_active = True
	while this_active:			
		display_state(state)
		print("\n\n move = hjkl/wasd | restart = r | menu = q\n")
		key = readchar()
		if key in ('h','a'):
			move_obj(state, state['player'], xsteps=-1)
		elif key in ('j','s'):
			move_obj(state, state['player'], ysteps=1)
		elif key in ('k','w'):
			move_obj(state, state['player'], ysteps=-1)
		elif key in ('l','d'):
			move_obj(state, state['player'], xsteps=1)
		elif key == 'r':
			state = sokoban_load(state['current_level'])
		elif key == 'q':
			this_active = False
		if state['complete'] is True:
			this_active = False
	if state['complete'] is True:
		run_menu(state, msg='\n Hooray! You\'ve completed level {}!'.format(state['current_level']))
	else:
		run_menu()

run_menu()