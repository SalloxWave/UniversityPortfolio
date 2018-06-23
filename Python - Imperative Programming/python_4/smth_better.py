def sokoban_load(filename):
	level_list = []
	with open(filename, 'r') as level_file:
		level_list = level_file.read().splitlines()

	level = []
	y = 0
	#Loop through each line in file
	for line in level_list:
		#Add a new row to the level
		level.append([])
		x = 0
		for char in line:
			if char is not ' ':
				level[y].append(create_obj(char,x,y))
			x += 1
		y += 1
	return level

def create_obj(char,x,y):
	obj = []
	#Create a wall
	if char is '#':
		obj = {'char':'#','x':x,'y':y}
	#Create a box
	elif char is 'o':
		obj = {'char':'o','x':x,'y':y}
	#Create a player
	elif char is '@':
		obj = {'char':'@','x':x,'y':y}
	#Create a boxplace
	elif char is '.':
		obj = {'char':'.','x':x,'y':y}
	return obj

def get_x_pos(obj):
	return obj['x']

def get_y_pos(obj):
	return obj['y']

def get_char(obj):
	return obj['char']

def display_sokoban(level):
	"""
	#print(chr(27) + "[2J")
	lvl_text = ''
	#Loop through all rows in level
	for row in level:
		x_values = []
		#Loop through the objects in the row
		for i in range(len(row)):
			#Add the object's x-value
			x_values.append(row[i]['x'])
		#Decide the highest x-value in that row
		max_x = max(x_values)
		row_length = max_x + 1 #+1 because there are one column than the highest x-value
		obj_counter = 0
		#Iterate the length of the loop
		for i in range(row_length): 
			#Check if object can be found on the row position
			if i in x_values:
				#Add the object to the level string
				lvl_text += get_char(row[obj_counter])
				#Keep track of amount of objects found
				obj_counter += 1
			#Position should be an empty space
			else:
				lvl_text+=' '
		#Add a new line after row
		lvl_text+='\n'
	print(lvl_text)
	"""


	#for row in level:
	#	x_obj =[]
	#	for i in range(len(row)):
	#		x_obj.append(row[i]['x'])
	#	x_max = max(x_obj)
	#	for i in range(x_max):
	#		for j in range(len(x_obj)):
	#			if i in x_obj[j]:
	#				print(get_char(row[i]))
			
			#max_x = max(row[i]['x'])
			#print(str(max_x))
				#print(get_x_pos(row[i]))
			#if i is row[i]['x']:
				#print("hej")


		#for obj in row:
		#	max_x = max(row.keys())
		#	print(max_x)
			#for i in range(max_x):
			#	if i is in row
			#	lvl_text+=get_char(row[i])
	print(chr(27) + "[2J")
	for row in level:
		for obj in row:
			print_anywhere(get_char(obj),get_x_pos(obj),get_y_pos(obj))
	#print(row_counter)
	#print(object_counter)
	#print(lvl_text)

def print_anywhere(char,x,y):
	print("\033[" + str(y) + ';' + str(x) + 'H' + char)

def get_object_by_cordinates(level,x,y):
	found_obj = None
	#Check for all objects in row at position y
	for obj in level[y]:
		#Look for object with correct x-position
		if obj['x'] is x:
			found_obj = obj
			break
	return found_obj

def get_cordinates_by_object(level,obj):
	x = 0
	y = 0
	#Loop through all rows in the level
	for row in level:
		#Loop through all objects in that row
		for row_obj in row:
			#Look for correct object
			if row_obj is obj:
				#Add correct object's cordinates
				x = row_obj['x']
				y = row_obj['y']
	return x,y

def get_player(level):
	player = {}
	for row in level:
		for obj in row:
			if get_char(obj) is '@':
				player = obj
				break
	return player

def can_move(level,obj,direction):
	_can_move = False
	new_x = 0
	new_y = 0
	if direction == 'up':
		new_x = get_x_pos(obj)
		new_y = get_y_pos(obj) - 1
	elif direction == 'down':
		new_x = get_x_pos(obj)
		new_y = get_y_pos(obj) + 1
	elif direction == 'left':
		new_x = get_x_pos(obj) - 1
		new_y = get_y_pos(obj)
	elif direction == 'right':
		new_x = get_x_pos(obj) + 1
		new_y = get_y_pos(obj)
	
	#Get object on new position
	new_obj = get_object_by_cordinates(level,new_x,new_y)
	#Object is a white space
	if new_obj is None or get_char(new_obj) is '.':
		_can_move = True
	#Object is a dynamic object
	elif moveable(new_obj) and get_char(obj) != get_char(new_obj):
		#Check if the object at new space can be moved
		if can_move(level,new_obj,direction):
			move(level,new_obj,direction)
			_can_move = True
		else:
			_can_move = False
	return _can_move

def move(level,obj,direction):
	print(direction)
	new_obj = obj
	new_x = 0
	new_y = 0
	if direction == 'up':
		new_x = get_x_pos(obj)
		new_y = get_y_pos(obj) - 1
	elif direction == 'down':
		new_x = get_x_pos(obj)
		new_y = get_y_pos(obj) + 1
	elif direction == 'left':
		new_x = get_x_pos(obj) - 1
		new_y = get_y_pos(obj)
	elif direction == 'right':
		new_x = get_x_pos(obj) + 1
		new_y = get_y_pos(obj)
	
	lvl_y = get_y_pos(obj)
	lvl_x = get_x_pos(obj)
	lvl_row = level[lvl_y]
	for i in range(len(lvl_row)):
		if obj is lvl_row[i]:
			del level[lvl_y][i]
			break

	new_obj['x'] = new_x
	new_obj['y'] = new_y
	level[new_y].append(new_obj)
	#print(obj)
	#return level
	#level[lvl_y][str(lvl_x)][]

def moveable(obj):
	if get_char(obj) in '@o':
		return True
	return False
"""
def collision(level, obj, direction):
	collision = False
	new_x = 0
	new_y = 0
	if direction == 'up':
		new_x = get_x_pos(obj)
		new_y = get_y_pos(obj) - 1
	elif direction == 'down':
		new_x = get_x_pos(obj)
		new_y = get_y_pos(obj) + 1
	elif direction == 'left':
		new_x = get_x_pos(obj) - 1
		new_y = get_y_pos(obj)
	elif direction == 'right':
		new_x = get_x_pos(obj) + 1
		new_y = get_y_pos(obj)
	
	#Get object on new position
	new_obj = get_object_by_cordinates(level,new_x,new_y)
	#Object is a dynamic object
	if moveable(new_obj):
		collision = True
	return collision
"""
def get_user_direction():
	direction = input("Which direction do you want to go?")
	return direction

#Initialize the level
level = sokoban_load('levels/first_level.sokoban')
print(level)
#Initialize the player
player = get_player(level)
#Start the game
game_over = False
while game_over == False:
	
	display_sokoban(level)
	
	#Get desired direction
	direction = get_user_direction()
	#Check if player can move the direction
	if can_move(level,player,direction):
		#Move the player
		move(level,player,direction)



#level = sokoban_load('levels/first_level.sokoban')

#display_sokoban(level)
#player = get_player(level)
#print(player)

#if can_move(level,player,'up'):
#	move(level,player,'up')
#print(player)
#display_sokoban(level)

#if can_move(level,player,'left'):
	#move(level,player,'left')
#display_sokoban(level)






#i = 0
#for row in level:
#	for obj in row:
#		print(i,obj['x'])
#	i+=1
#print(level)
#for i in range(len(level)):
	#print("row",i,"has",len(level[i]))
#print(len(level))
#print(get_x_pos(level[0][0]))
#print(get_y_pos(level[0][0]))
#print(get_char(level[0][0]))