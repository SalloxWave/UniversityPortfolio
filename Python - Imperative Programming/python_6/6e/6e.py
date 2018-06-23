import re
import sys
import os
import argparse

def validate_ext(file, ext_limit):
	if ext_limit is None or os.path.splitext(file)[1][1:] == ext_limit:
		return True
	return False

def _set_copyright(file, crnotice, ext_new = None):
	with open(file) as f:
		filetxt = f.read()
	#Add copyright info to spcified file
	regex_obj = re.compile("(?<=BEGIN COPYRIGHT).*?(?=END COPYRIGHT)", re.DOTALL)
	filetxt = regex_obj.sub('\n' + crnotice + '\n', filetxt)
	with open(file,'w') as f:
		f.write(filetxt)
	if ext_new is not None:
		#Rename file with new extension
		newname = os.path.splitext(file)[0]
		os.rename(file, newname + '.' + ext_new)

def set_copyright(arguments):
	cr_path = arguments[0]
	insert_path = arguments[1]
	optional_args = get_optional_arguments(arguments)
	ext_limit = None if args.l is None else optional_args.l[0]
	ext_new = None if args.s is None else optional_args.s[0]
	#Get copyright text from copyright file
	with open(cr_path) as f:
		crnotice = f.read()
	if os.path.isdir(insert_path):
			for file in os.listdir(insert_path):
				if validate_ext(file, ext_limit):
					_set_copyright(insert_path + '/' + file, crnotice, ext_new)
	elif validate_ext(file, ext_limit):
		_set_copyright(insert_path, crnotice, ext_new)

def get_optional_arguments(arguments):
	parser = argparse.ArgumentParser(description="Specifies file extension options for copyright module",
		epilog="Thanks for reading this awesome help page!")
	parser.add_argument("-l", nargs=1, 
		help="Specifies the limit of what file extensions to add copyright information. Ignore this field to change all file extensions")
	parser.add_argument("-s", nargs=1, 
		help="Specifies what the output-file's file extension should be. Ignore this field to use same extension as before")
	print(arguments)
	args = parser.parse_args(arguments[2:])
	return args


set_copyright(sys.argv[1:])