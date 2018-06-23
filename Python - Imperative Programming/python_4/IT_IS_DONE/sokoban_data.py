objects = {
	'#': {
		'type': 'wall',
		'chars': {
			'default': '#'
		},
		'color': 'white',
		'solid': True,
		'z-index': 0
	},

	'o': {
		'type': 'crate',
		'chars': {
			'default': 'o',
			'storage': '*'
		},
		'color': 'yellow',
		'solid': True,
		'z-index': 1
	},

	'.': {
		'type': 'storage',
		'chars': {
			'default': '.'
		},
		'color': 'cyan',
		'solid': False,
		'z-index': 0
	},

	'@': {
		'type': 'player',
		'chars': {
			'default': '@',
			'storage': '+'
		},
		'color': 'magenta',
		'solid': False,
		'z-index': 1
	},

	'*': ('o', '.'),

	'+': ('@', '.')
}

levels = {
	'folder_path': 'levels/',
	'file_path': 'levels/level_{:0>2}.sokoban'
}