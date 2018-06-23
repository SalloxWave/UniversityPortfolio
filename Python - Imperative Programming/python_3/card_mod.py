def create(desc_val, suit_val):
	""" Creates a card with the specified value and suit. In the actual tuple returned,
		the first index will contain a value from 1-27 (hearts 1-13, spades 14-26, jokers 27),
		and the second a description of which card it is (e.g. 2 of Spades)
		argument | type[: specification] [, ...] [| default value] | description
			 desc_val | integer: 1-13 or 27 | Ace to King or Joker
			 suit_val | integer: between 1 and 2 | Hearts and Spades """

	if desc_val is not 27:
		if suit_val is 1:
			real_val = desc_val
			suit_str = 'Hearts'
		elif suit_val is 2:
			real_val = desc_val+13
			suit_str = 'Spades'
		desc = str(desc_val) + ' of ' + suit_str
	elif desc_val is 27: 
		if suit_val is 1:
			suit_str = 'A'
		elif suit_val is 2:
			suit_str = 'B'
		real_val = 27
		desc = 'Joker ' + suit_str
	return (real_val, desc)

#get_value
def get_val(card):
	""" Get value of a card 
		argument | type[: specification] [, ...] [| default value] | description
			card | tuple: created by card_mod.create() | which card to perform action with """

	return card[0]

#get_description
def get_desc(card):
	""" Get description of a card 
		argument | type[: specification] [, ...] [| default value] | description
			card | tuple: created by card_mod.create() | which card to perform action with """

	return card[1]