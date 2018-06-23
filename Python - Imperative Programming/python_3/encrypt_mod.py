import keystream_mod

def solitaire_encrypt(txt, deck, seed_arg):
	""" Encrypts a text according to the solitaire-algorithm
		argument | type[: specification] [, ...] [| default value] | description
			txt | string | the text to encrypt
			deck | list: created by deck_mod.create() | which deck to perform action on
			seed_arg | integer, any hashable object: read python docs for random.seed() for more information on
				   	 options other than integers | if not None: specifies a seed which a pseudo shuffle will be performed according to """

	txt = purify_text(txt)
	key_length = len(txt)
	key = keystream_mod.solitaire_keystream(deck, key_length, seed_arg)
	txt_ints = text_to_int(txt)
	key_ints = text_to_int(key)
	txt_key_sums = sum_lists(txt_ints, key_ints)
	enc_txt = int_to_text(txt_key_sums)
	return enc_txt

def sum_lists(txt_ints, key_ints):
	""" Sums the first value of the text list with first value of the key list and returns as a new list, then if sum gets higher then 26, subtracs with 26 (repeat for all values)
		argument | type[: specification] [, ...] [| default value] | description
			txt_ints | list: of integers | list of numbers created from the message to encrypt
			key_ints | list: of integers| list of the numbers created from the solitaire algorithm """
	txt_key_sums = []
	for i in range(len(txt_ints)):
		num = txt_ints[i] + key_ints[i]
		if num > 26:
			num -= 26
		txt_key_sums.append(num)
	return txt_key_sums

def purify_text(txt):
	""" Removes all character except A-Z and returns a new string
		argument | type[: specification] [, ...] [| default value] | description
			txt | string | text to apply action on """
	purified_txt = ''
	for char in txt:
		if ord(char.upper()) >= ord('A') and ord(char.upper()) <= ord('Z'):
			purified_txt += char.upper()
	return purified_txt

def text_to_int(txt):
	""" Converts a text to numbers according to the order of the alfphabet
		argument | type[: specification] [, ...] [| default value] | description
			txt | string | text to apply action on """
	nums = []
	for char in txt:
		nums.append(ord(char) - 64)
	return nums

def int_to_text(nums):
	""" Converts a list of numbers to characters according to the order of the alphabet
		argument | type[: specification] [, ...] [| default value] | description
			nums | list: of integers | list of integers to apply action on """
	txt = ''
	for num in nums:
		txt += chr(64 + num)
	return txt
