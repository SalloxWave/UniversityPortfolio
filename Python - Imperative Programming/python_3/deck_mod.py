import random
import card_mod

def create():
	""" Returns a list with a 'deck' string and a list representing a deck with 28 cards,
		with each card being representent by a tuple with two values,
		the first value ranges from 1-27 and is the value of the card
		the second value is a string with a description of the card (e.g. 2 of Spades) """

	deck = ['deck', []]
	for i in range(1, 27):
		if i < 14:
			suit = 1
		elif i >= 14:
			i -= 13
			suit = 2
		deck[1].append(card_mod.create(i, suit))
	deck[1].append(card_mod.create(27, 1))
	deck[1].append(card_mod.create(27, 2))
	return deck

def peek(deck, pos = 1, bottom = False):
	""" Peeks at a card in the deck on the specifed position (default: the top card of the deck)
		argument | type[: specification] [, ...] [| default value] | description
			deck | list: created by deck_mod.create() | which deck to perform action on
			pos | integer: between 1 and len(deck) | default = 1 | position of the card
			bottom | boolean | False | if True count position from the bottom, else from the top """

	if not bottom:
		pos -= 1
	elif bottom:
		pos = len(deck[1]) - pos
	card = deck[1][pos]
	return card

def pick(deck, pos = 1, bottom = False):
	""" Picks a card from the deck from the specifed position (default: from the top of the deck)
		argument | type[: specification] [, ...] [| default value] | description
			deck | list: created by deck_mod.create() | which deck to perform action on
			pos | integer: between 1 and len(deck) | default = 1 | position of the card
			bottom | boolean | False | if True count position from the bottom, else from the top """

	card = peek(deck, pos, bottom)
	deck[1].pop(pos-1)
	return card

def add(card, deck, pos = 1, bottom = False):
	""" Adds a card into the deck on the specifed position (default: to the top of the deck)
		argument | type[: specification] [, ...] [| default value] | description
			card | tuple: created directly by card_mod.create() or indirectly by deck_mod.create() | which card to perform action with
			deck | list: created by deck_mod.create() | which deck to perform action on
			pos | integer: between 1 and len(deck) | 1 | position to insert the card on
			bottom | boolean | False | if True count position from the bottom, else from the top """

	if not bottom:
		pos -= 1
	elif bottom:
		if pos is 1:
			pos = len(deck)
		else:
			pos = len(deck) - (pos - 1)
	deck[1].insert(pos, card)

def remove(deck, pos = 1):
	""" Removes a card from the deck on the spcified position (default: top card of the deck)
		argument | type[: specification] [, ...] [| default value] | description
			deck | list: created by deck_mod.create() | which deck to perform action on
			pos | integer: between 1 and len(deck) | 1 | position to insert the card on """

	deck[1].pop(pos - 1)
	# EVENTUELLT LÄGGA TILL BOTTOM=FALSE

def move(card, deck, steps = 1, up = False):
	""" Moves a card a number of steps down/up in the deck (default: 1 step down)
		If a card is moved down/up when it is already at the bottom/top, that singular step will instead place the card right after/before the top/bottom card
		argument | type[: specification] [, ...] [| default value] | description
			card | tuple: created directly by card_mod.create() or indirectly by deck_mod.create() | which card to perform action with
			deck | list: created by deck_mod.create() | which deck to perform action on
			steps | integer | 1 | steps to move the card
			up | boolean | False | if True move card up, else down """

	if up:
		steps = steps*(-1)
	len_deck = len(deck[1])
	for i in range(len_deck):
		if deck[1][i] == card: #DOKUMENTERA VARFÖR == OCH INTE IS
			for j in range(steps):
				if i+j+1 >= len_deck:
					deck[1].pop(i+j)
					deck[1].insert(1, card)
				else:
					deck[1].pop(i+j)
					deck[1].insert(i+j+1, card)
			break
	### KANSKE TESTA FIXA UP

def shuffle(deck, seed_arg = None):
	""" Shuffle the deck (optional: pseudo shuffle according to a seed)
		argument | type[: specification] [, ...] [| default value] | description
			deck | list: created by deck_mod.create() | which deck to pick from
			seed_arg | None, integer, any hashable object: read python docs for random.seed() for more information on
				   	 options other than integers | None | if not None: specifies a seed which a pseudo shuffle will be performed according to """

	if seed_arg is not None:
		random.seed(seed_arg)
	random.shuffle(deck[1])

#joker_split
def jsplit(deck):
	""" Splits the deck in three parts
		A = top card until, but excluding, first joker
		B = first joker until, and including, second
		C = card below second joker until, and including, end of deck
		Then puts the deck together in the order C + B + A where C is top
		argument | type[: specification] [, ...] [| default value] | description
			deck | list: created by deck_mod.create() | which deck to perform action on """

	jindex = []
	for i in range(len(deck[1])):
		if card_mod.get_val(peek(deck, i+1)) is 27:
			jindex.append(i)
	a = deck[1][:jindex[0]]
	b = deck[1][jindex[0]:jindex[1]+1]
	c = deck[1][jindex[1]+1:]
	deck[1] = c + b + a

def bottom_val_mover(deck):
	""" Peeks at the bottom card of the deck and then, repeated as many times as the value of the peeked card, puts the top card right above the peeked card
		argument | type[: specification] [, ...] [| default value] | description
			deck | list: created by deck_mod.create() | which deck to perform action on """
	bottom_val = card_mod.get_val(peek(deck, bottom=True))
	for _ in range(bottom_val):
		card = pick(deck)
		deck[1].insert(-1, card)
		#add(card, deck, 2, True)