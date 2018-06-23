objects = {
	'#': {
		'type': 'wall',
		'chars': {
			'default': '#'
		},
		'color': '#hex',
		'solid': True,
		'z-index': 0
	},

	'o': {
		'type': 'crate',
		'chars': {
			'default': 'o',
			'storage': '*'
		},
		'color': '#hex',
		'solid': True,
		'z-index': 1
	},

	'.': {
		'type': 'storage',
		'chars': {
			'default': '.'
		},
		'color': '#hex',
		'solid': False,
		'z-index': 0
	},

	'@': {
		'type': 'player',
		'chars': {
			'default': '@',
			'storage': '+'
		},
		'color': '#hex',
		'solid': False,
		'z-index': 1
	}
}

levels = {
	'folder_path': 'levels/',
	'level_path': 'levels/level_{0:0>2}.sokoban'
}