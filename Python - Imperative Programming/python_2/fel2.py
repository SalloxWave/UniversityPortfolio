#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""A recursive descent LL(1) parser for evaluating a simple lisp-like language
that handles floating point arithmetics."""

# functions for parsing the language

def read_blanks(string, start):
    """Find the next non-blank character"""
    while string[start] in '\n\t ':
        start += 1
    return start

def parse(string, start=0):
    """Parse an expression in the language."""
    start = read_blanks(string, start)
    if string[start] == '(':
        return parse_call(string, start+1)
    else:
        return parse_symbol(string, start)

def parse_symbol(string, start):
    """Parse a symbol (number or string)"""
    end = start
    number = True
    while string[end] not in " \t\n'()":
        if string[end] not in "0123456789.":
            number = False
        end += 1
     
    #FIX   
    #Lösning med special fall, sätter number till False eftersom ny rad inte är ett nummer
    if start is end:
        number = False
        
    #Här kommer start vara lika med end när det är en ny rad(\n) vilket resulterar i en sträng utan index
    result = string[start:end]
    if number:
        #Bug
        #ValueError: could not convert string to float
        return float(result), end
    else:
        return result, end

def parse_call(string, start):
    """Parse a function call.
    Form: (function_name some arguments...)"""
    list, end = parse_list(string, start)
    function_name = list[0]
    arguments = list[1:]
    function = function_table[function_name]
    return (function, arguments), end
    
def parse_list(string, start):
    """Parse a list:
    Form: (some elements...)"""
    result = []
    while string[start] != ')':
        part, start = parse(string, start)
        result.append(part)
    return result, start + 1

# functions for evaluating the language

def evaluate(string):
    """Evaluate a program from a string (parse then evaluate)."""
    program, _ = parse(string)
    return Eval(program, {})

def Eval(program, state):
    """Evaluate an expression."""
    if isinstance(program, tuple):
        return program[0](program[1], state)
    elif isinstance(program, float):
        return program # numbers are self evaluating
    #BUG
    #KeyError: ''
    #Kan inte returna state[program] om program är en tomsträng
    #eftersom som det inte finns någon nyckel='' i state
    #else:
    #    return state[program] # symbols get looked up
    #FIX
    elif program is not '':
        return state[program] # symbols get looked up
    else:
        return None

# functions in the language

def program(arguments, state):
    innerState = {}
    innerState.update(state)
    result = None
    for statement in arguments:
        result = Eval(statement, state)
    return result

def printout(arguments, state):
    for arg in arguments:
        if isinstance(arg, tuple):
            print(Eval(arg, state), end=' ')
        else:
            print(arg, end=' ')
    print() # new line

def plus(arguments, state):
    result = 0
    for arg in arguments:
        result += Eval(arg, state)
    return result

def minus(arguments, state):
    result = Eval(arguments[0], state)
    if len(arguments) == 1:
        return -result
    else:
        return result - plus(arguments[1:], state)
    
def times(arguments, state):
    result = 1
    for arg in arguments:
        result *= Eval(arg, state)
    return result

def divide(arguments, state):
    result = Eval(arguments[0], state)
    return result / times(arguments[1:], state)

def power(arguments, state):
    base = Eval(arguments[0], state)
    return base ** plus(arguments[1:], state)

def setValue(arguments, state):
    assert len(arguments) == 2, "Wrong number of arguments to 'set'."
    name = arguments[0]
    value = Eval(arguments[1],state)
    state[name] = value
    return value

def lookup(arguments, state):
    assert len(arguments) == 1, "Wrong number of arguments to 'lookup'."
    return state[arguments[0]]

def quote(arguments, state):
    assert len(arguments) == 1, "Wrong number of arguments to 'quote'."
    return arguments[0]

# the table of functions in the language

function_table = {'program':program,
                  '+':plus,
                  '-':minus,
                  '*':times,
                  '/':divide,
                  '^':power,
                  'set':setValue,
                  'lookup':lookup,
                  'quote':quote,
                  'print':printout,
                  }

if __name__ == '__main__':
    # Working testcases
    evaluate("(program (set var 1) (set var1 10) (set var2 20)"
             "(print this should be 30.0: (+ var1 var2)))")
    evaluate("(program (print what do you get if you multiply "
             "six by nine?) (print -> (* 6 9)))")

    # Failing testcase,
    # these should work, use pdb to find the error in this module
    # then correct the parser above so that it accepts this input
    evaluate("""(program
(set six 6)
(set nine 7)
(print what do you get if you multiply six by nine?)
(print -> (* six nine))
)""")
    #läser en tomsträng eftersom den börjar läsa vid \n och slutar vid )
    evaluate("( print test )")
