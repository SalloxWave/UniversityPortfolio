import os
import sys
import game
import sokoban_data

def get_menu_selection(state=None):
	last_level = None if state is None else state['current_level']
	level_count = len([f for f in os.listdir(os.getcwd() + '/' + sokoban_data.levels['folder_path']) if f.endswith('.sokoban')])
	input_msg = "\n Select a level (1-{}){}| quit = q: "
	if last_level is not None and last_level < level_count:
		menu_input = input(input_msg.format(level_count, ' or leave empty for level {} '.format(last_level+1)))
		if menu_input == '':
			menu_input = last_level+1
	else:
		menu_input = input(input_msg.format(level_count, ' '))
	return menu_input

def run_menu(state=None, msg=None):
	this_active = True
	while this_active:
		os.system('clear')
		if msg is None:
			print('\n Sokoban! Probably the best game in the world.')
		else:
			print(msg)
		selection = get_menu_selection(state)
		if selection is 'q':
			os.system('clear')
			sys.exit()
		else:
			try:
				state = sokoban_load(int(selection))
				this_active = False
			except (FileNotFoundError, ValueError):
				pass
	game.run_game(state)