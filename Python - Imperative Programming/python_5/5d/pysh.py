import os
import getpass
from socket import gethostname
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

def _get_public_fns(module):
	return [fn[0] for fn in inspect.getmembers(sys.modules[module], inspect.isfunction) if not fn[0].startswith('_')]

def _pretty_input():
	return input(getpass.getuser() + '@' + gethostname() + ':' + os.getcwd().replace('/home/{}'.format(getpass.getuser()), '~') + '$ ')

def _run():
	fns = _get_public_fns(__name__)
	while True:
		inp = _pretty_input().split()
		if len(inp) is not 0:
			cmd = inp[0]
			args = inp[1:] if len(inp) > 1 else None
			if cmd in fns:
				try:
					eval(cmd+'({})'.format(args))
				except FileNotFoundError as e:
					print(cmd + ': ' + e.filename + ': Filen eller katalogen finns inte')
				except IsADirectoryError as e:
					print(cmd + ': ' + e.filename + ': Är en katalog')
			else:
				print('pysh: ' + cmd + ': kommandot finns inte')

_run()

