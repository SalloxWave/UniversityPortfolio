#!/usr/bin/env python3

def check_joker(card):
	"""Checks if card is a joker"""
	return get_value(card) is 0

def create_card(value, suit):
	"""Creates a card with the desired value and suit"""
	return (value, suit)

def get_value(card):
	"""Get value of card"""
	return card[0]

def get_suit(card):
	"""Get suit of card(1 = Hearts, 2 = Spades)"""
	return card[1]

def display_card(card):
	"""Display the card's values"""
	suit = get_suit(card)
	msg = ''

	if check_joker(card):
		if suit is 1:
			msg = 'joker A'
		else:
			msg = 'joker B'
	else:
		value = get_value(card)
		if get_suit(card) is 1:
			suit = 'Hearts'
		else:
			suit = 'Spades'
		msg = '{} of {}'.format(value, suit)

	print(msg)

def create_deck():
	"""Creates a deck with 28 (2 jokers)"""
	deck = ['deck', []]
	for i in range(14):
		for j in range(2):
			deck[1].append(create_card(i, j+1))
	return deck

def pick_card(deck):
	"""Pick out card from deck"""
	card = deck[1][0]
	deck[1].pop(0)
	return card

def insert_card(card, deck):
	"""Inserts a card to the top of the deck"""
	deck[1].insert(0, card)

def shuffle_deck(deck):
	"""Divide the card's value and suit representation value by
	3.38, then sort from lowest fraction part to highest"""
	div = 3.38
	frac_list = []
	for i in range(len(deck[1])):
		frac_part = ((deck[1][i][0] + deck[1][i][1]) / div) % 1
		frac_list.append((frac_part, i))
	frac_list.sort()

	temp_deck = []
	for i in range(len(frac_list)):
		temp_deck.append(deck[1][frac_list[i][1]])
	deck[1] = temp_deck

def move_up(card,deck,steps=1):
	move_card(card,deck,steps*-1)

def move_down(card,deck,steps=1):
	move_card(card,deck,steps)

def move_card(card,deck,steps):
	if steps is 0:
		return None

	len_deck = len(deck[1])
	for i in range(len_deck):
		if deck[1][i] is card:

			if i+1+steps > len_deck:
				diff = (i+1+steps) - len_deck
				steps = steps-diff+1
				deck[1].insert(steps, card)
				deck[1].pop(i+1)
			#elif i+steps < len_deck:
			#	diff = (i+steps) - len_deck
			#	steps = steps-diff+1
			#	deck[1].insert(steps+len_deck-1, card)
			#	deck[1].pop(i)"""
			else:
				deck[1].insert(i+steps, card)
				if steps > 0:
					deck[1].pop(i)
				else:
					deck[1].pop(i+1)

def split_on_jokers(deck):
	jokers_index = []
	for i in range(len(deck[1])):
		if get_value(deck[1][i]) is 0:
			jokers_index.append(i)

	deck_a = deck[1][:jokers_index[0]]
	deck_b = deck[1][jokers_index[0]:jokers_index[1]+1]
	deck_c = deck[1][jokers_index[1]+1:]

	deck[1] = deck_c + deck_b + deck_a

def matcu(deck):
	value = get_value(deck[1][-1])
	for _ in range(value):
		deck[1].insert(-1,deck[1][0]) #kommer näst sist med -1
		deck[1].pop(0)

def get_letter(number):
	alfa = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
	return alfa[number-1]

def above_card(deck):
	"""Go to card according to value of top card"""
	value = get_value(deck[1][0])
	key_card = deck[1][value]

	if check_joker(key_card) is False:
		key = get_letter(get_value(key_card))
	else:
		key = None
	return key

def get_solitaire_key(deck):

	key = None
	while key is None:
		for card in deck:
			if get_value(card) is 0:
				if get_suit(card) is 1:
					move_down(card,deck,1)
				else:
					move_down(card,deck,2)
		split_on_jokers(deck)
		matcu(deck)
		key = above_card(deck)

	return key

def solitaire_keystream(length,deck):
	key = ""
	shuffle_deck(deck)
	for i in range(length):
		key += get_solitaire_key(deck)
	return key
