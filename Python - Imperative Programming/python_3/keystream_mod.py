import card_mod
import deck_mod

def get_key_char(deck):
	""" Peeks at the top card and notes the value, then peeks at the card at the position equal to the noted value, counted from the top,
		and converts that card's value into a char if it is not a joker (1 = A ... 26 = Z)
		argument | type[: specification] [, ...] [| default value] | description
			deck | list: created by deck_mod.create() | which deck to perform action on """

	key_char = ''
	top_val = card_mod.get_val(deck_mod.peek(deck))
	key_card = deck_mod.peek(deck, top_val+1) #key_card = deck[1][top_val]
	key_int = card_mod.get_val(key_card)

	if key_int is not 27:
		key_char = chr(64 + key_int)
	return key_char

def solitaire_keystream(deck, length, seed_arg):
	""" Generates a key according to the solitaire-algorithm
		argument | type[: specification] [, ...] [| default value] | description
			deck | list: created by deck_mod.create() | which deck to perform action on
			length | integer | the length of the generated key
			seed_arg | integer, any hashable object: read python docs for random.seed() for more information on
				   	 options other than integers | if not None: specifies a seed which a pseudo shuffle will be performed according to """

	temp_deck = deck[:]
	temp_deck[1] = deck[1][:]

	deck_mod.shuffle(temp_deck, seed_arg)
	key_chars = ''
	while len(key_chars) < length:
		deck_mod.move((27, 'Joker A'), temp_deck)
		deck_mod.move((27, 'Joker B'), temp_deck, 2)
		deck_mod.jsplit(temp_deck)
		deck_mod.bottom_val_mover(temp_deck)
		key_chars += get_key_char(temp_deck)
	return key_chars
	### NOTE length = 10 000 == BUG

