# TDP015 Programming Assignment 3

# Do not use any imports!

# In this assignment you are asked to implement functions on *nested
# pairs*. The set of nested pairs is defined recursively:
#
# 1. The empty tuple () forms a nested pair.
#
# 2. If a and b are nested pairs, then the tuple (a, b) forms a nested
#    pair.
#
# Here are some examples of nested pairs sorted by their *degree*. The
# degree of a nested pair is the number of empty tuples contained in it.
#
# degree 1 (1 pair):
#
# ()
#
# degree 2 (1 pair):
#
# ((), ())
#
# degree 3 (2 pairs):
#
# ((), ((), ()))
# (((), ()), ())
#
# degree 4 (5 pairs):
#
# ((), ((), ((), ())))
# ((), (((), ()), ()))
# (((), ()), ((), ()))
# (((), ((), ())), ())
# ((((), ()), ()), ())
#
# The following sequence gives the number of nested pairs with degrees
# 1, 2, 3, ...:
#
# 1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796, 58786, 208012, 742900,
# 2674440, 9694845, 35357670, 129644790, 477638700, 1767263190, ...

# ## Problem 1
#
# Implement a function nested_pairs() that yields all nested pairs
# with a specified degree. Each nested pair should be yielded exactly
# once. For example, nested_pairs(4) should yield the 5 nested pairs
# listed above. Use recursion. Test your function by counting the
# number of nested pairs yielded by it and comparing against the
# sequence of numbers given above.

def nested_pairs(n):
    """Yield all nested pairs with degree *n*."""

    #Base case when you've gone too deep, return an empty tuple
    if (n <= 1):
        yield ()
    #Recursive case
    else:
        for i in range(1, n):
            #Go down one step to find all pairs of degree below
            for ls in nested_pairs(n-i):
                #Go through current pairs
                for rs in nested_pairs(i):
                    #Create pair with lefthand side and righthand side
                    yield (ls, rs)

# ## Problem 2
#
# Implement a function count_nested_pairs() that counts the number of
# nested pairs with a specified degree. A naive implementation of this
# function would call the nested_pairs() function from Problem 1. This
# is not what you are supposed to do! Instead, try to come up with a
# solution that counts the number of nested pairs without generating
# them. There is a way to solve this problem using a formula; but your
# solution should use recursion. Test your implementation by comparing
# your numbers to the numbers that you got above. What is the maximal
# degree for which you can compute the number of nested pairs in under
# one minute?

def count_nested_pairs(n):
    """Count the number of nested pairs with degree *n*."""

    #Base case when you've gone too deep, return one
    if (n <= 1):
        return 1
    #Recursive case
    else:
        count = 0
        for i in range(1, n):
            #One step below times current pairs (for-loop from "nested_pairs()")
            count+=count_nested_pairs(n-i) * count_nested_pairs(i)
        return count

# ## Problem 3
#
# Because it uses recursion, the function that you implemented in
# Problem 2 will call itself many times, and many times with the same
# argument. One way to speed things up is to cache the results of
# these calls. This strategy is called *memoization*.
#
# The idea is the following: All recursive calls of the function get
# access to a common cache in the form of a dictionary. Before a
# recursive call computes the number of nested pairs of a given degree
# *n*, it first checks whether that number is already stored in the
# cache. If yes, then the recursive call simply returns that
# value. Only if the value has not already been cached, the recursive
# call starts a computation on its own; but then it stores the result
# of that computation in the common cache, under the key *n*, so that
# subsequent calls will not have to recompute it.
#
# Write a function count_nested_pairs_memoized() that implements this
# idea. Test your implementation as above. How long does it take you to
# compute the number of nested pairs for the maximal degree that you
# could do in under one minute in Problem 2?

memo = {}
def count_nested_pairs_memoized(n):
    """Count the number of nested pairs with degree *n*."""
    global memo

    #Check if value has been cached
    if (n in memo):
        return memo[n]
    #Base case when you've gone too deep, return one
    elif (n <= 1):
        return 1
    #Recursive case
    else:
        count = 0
        for i in range(1, n):
            #One step below times current pairs (for-loop from "nested_pairs()")
            count+=count_nested_pairs_memoized(n-i) * count_nested_pairs_memoized(i)
        #Save result with degree number as key and count as value
        memo[n] = count
        return count

#Highest degree taking under one minute is 18
pairs = 1
for i in nested_pairs(pairs):
    print(i)

pair_testes = 4
for i in range(1, pair_testes+1):
    for i in nested_pairs(i):
        print(i)
    print()

count = 18
print(count_nested_pairs(count))

count_testes = 0
for i in range(1, count_testes+1):
    print(count_nested_pairs(i))

"""Remove this comment to be able to use higher degrees
import sys
sys.setrecursionlimit(10000)
"""

#Maximum degree (18) from problem 2 now completes instant
#The maximum degree in this case is about 2800 with "i7@4Ghz"
memo_count = 18
print(count_nested_pairs_memoized(memo_count))

memo_testes = 0
for i in range(1, memo_testes+1):
    print(count_nested_pairs_memoized(i))