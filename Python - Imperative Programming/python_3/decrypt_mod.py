import keystream_mod

def solitaire_decrypt(txt, deck, seed_arg):
	""" Decrypts an encrypted text according to the solitaire-algorithm
		argument | type[: specification] [, ...] [| default value] | description
			txt | string | the text to decrypt
			deck | list: created by deck_mod.create() | which deck to perform action on
			seed_arg | integer, any hashable object: read python docs for random.seed() for more information on
				   	 options other than integers | if not None: specifies a seed which a pseudo shuffle will be performed according to """

	txt = text_to_num(txt)
	length = len(txt)
	key = keystream_mod.solitaire_keystream(deck, length, seed_arg)
	key = text_to_num(key)
	txt_key_sums = subtr_lists(txt, key)
	dec_txt = int_to_text(txt_key_sums)
	print(dec_txt)

def subtr_lists(txt, key):
	""" Subtracts the first value of the key list from the first value of the txt list and returns as a new list, then if sum gets lower than 1, adds 26 (repeat for all values)
		argument | type[: specification] [, ...] [| default value] | description
			txt_ints | list: of integers | list of numbers created from the message to encrypt
			key_ints | list: of integers| list of the numbers created from the solitaire algorithm """
	lst = []
	for i in range(len(txt)):
		num = txt[i] - key[i]
		if num < 1:
			num += 26
		lst.append(num)
	return lst

def text_to_num(txt):
	""" Converts a text to numbers according to the order of the alfphabet
		argument | type[: specification] [, ...] [| default value] | description
			txt | string | text to apply action on """
	temp = []
	for char in txt:
		temp.append(ord(char) - 64)
	return temp


def int_to_text(nums):
	""" Converts a list of numbers to characters according to the order of the alphabet
		argument | type[: specification] [, ...] [| default value] | description
			nums | list: of integers | list of integers to apply action on """
	temp = ''
	for num in nums:
		temp += chr(64 + num)
	return temp