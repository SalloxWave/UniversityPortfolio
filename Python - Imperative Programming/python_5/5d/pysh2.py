import os
import getpass
import socket
import sys
import inspect

def pwd(args):
	print(os.getcwd())

def ls(args):
	if args is None:
		_ls()
	else:
		curr_path = os.getcwd()
		os.chdir(args[0])
		_ls()
		os.chdir(curr_path)

def _ls():
	for f in os.listdir(os.getcwd()):
		if os.path.isdir(os.getcwd() + '/' + f):
			print('\033[94m' + f + '\033[0m')
		else:
			print(f)

def cd(args):
	path = os.path.expanduser('~') if args is None else args[0]
	os.chdir(path)

def cat(args):
	if args is None:
		args = ['']
	with open(args[0]) as f:
		lines = f.read().splitlines()
	for line in lines:
		print(line)

def exit(args):
	sys.exit("Returning to bash")

def _pretty_input():
	return input(getpass.getuser() + '@' + socket.gethostname() + ':' + os.getcwd().replace('/home/{}'.format(getpass.getuser()), '~') + '$ ')

def _run():
	while True:
		inp = _pretty_input().split()
		if len(inp) is not 0:
			cmd = inp[0]
			args = inp[1:] if len(inp) > 1 else None
			try:
				if cmd == 'pwd':
					pwd(args)
				elif cmd == 'ls':
					ls(args)
				elif cmd == 'cd':
					cd(args)
				elif cmd == 'cat':
					cat(args)
				elif cmd == 'exit':
					exit(args)
				else:
					print('pysh: ' + cmd + ': kommandot finns inte')
			except FileNotFoundError as e:
				print(cmd + ': ' + e.filename + ': Filen eller katalogen finns inte')
			except IsADirectoryError as e:
				print(cmd + ': ' + e.filename + ': Är en katalog')

_run()

