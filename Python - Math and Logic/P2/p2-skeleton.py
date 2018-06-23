# TDP015 Programming Assignment 2

# Do not use any imports!

# Part 1: Implement the following quantifiers. You are not allowed to use
# any libraries or built-in functions for this.

def forall(iterable, predicate):
    """Return `True` if *predicate* returns `True` for all elements of
    *iterable* (or if *iterable* is empty)."""

    for element in iterable:
        if not predicate(element):
            return False
    return True

def test_forall():
    print("test_forall")
    assert forall(set(), lambda x: False)
    assert forall(set(range(5, 10)), lambda x: x >= 5)
    assert not forall(set(range(0, 10)), lambda x: x >= 5)

def exists(iterable, predicate):
    """Return `True` if *predicate* returns `True` for at least one
    element of *iterable*."""

    for element in iterable:
        if predicate(element):
            return True
    return False

def test_exists():
    print("test_exists")
    assert not exists(set(), lambda x: False)
    assert exists(set(range(5, 10)), lambda x: x >= 5)
    assert not exists(set(range(0, 5)), lambda x: x >= 5)

# Part 2: Implement the following functions on sets. You are not allowed
# to use any native operations on sets except for the `in` primitive.
# You are allowed to use the quantifiers from Part 1.

# How are these functions called in native Python?
#------------------------------------------------------------------------------
#- "forall": all(map(lambda x: x in B, A)). We apply a condition to elements in A that they should be in element B. "map"-function then returns a value of the result which in this case is either True or False. "all"-function only returns True if all values in an iterable is True 
#all: (https://docs.python.org/3/library/functions.html#all) 
#map: (https://docs.python.org/3/library/functions.html#map)

#- "subset" is same as "issubset"-function in native Python (https://docs.python.org/2/library/stdtypes.html#set.issubset)

#- "equals" is basically the same as converting the iterable to a set using function "set" which then makes it possible to compare like this set(A) == set(B) (http://stackoverflow.com/questions/9623114/check-if-two-unordered-lists-are-equal)

#- "disjoint": Use "issubset"-function' with negation in front: not A.issubset(B)
#------------------------------------------------------------------------------

def subset(s, t):
    """Test whether *s* is a subset of *t*."""
    #All elements of "s" needs to exists in "t"
    return forall(s, lambda x: x in t)

def test_subset():
    print("test_subset")
    assert subset(set(), set())
    assert subset(set(), set([0]))
    assert subset(set([0]), set([0]))
    assert subset(set([0]), set([0, 1]))
    assert not subset(set([0]), set())
    assert not subset(set([0, 1]), set([0]))

def equal(s, t):
    """Test whether *s* and *t* are equals (as sets)."""

    ####### Solution without subset ##########
    """
    for element in s:
        if element not in t:
            return False
    for element in t:
        if element not in s:
            return False
    """
    #s⊆t ∧ t⊆s
    return subset(s, t) and subset(t, s)

def test_equal():
    print("test_equal")
    assert equal(set(), set())
    assert not equal(set(), set([0]))
    assert equal(set([0]), set([0]))
    assert not equal(set([0]), set([0, 1]))
    assert not equal(set([0]), set())
    assert not equal(set([0, 1]), set([0]))

def proper_subset(s, t):
    """Test whether *s* is a proper subset of *t*."""

    # TODO: Replace the following line with your own code

    ####### Solution without functions above (Part 2) ##########
    """
    for element in s:
        if element not in t:
            return False
    for element in t:
        if element not in s:
            return True
    """
    #s⊆t ∧ ¬(s=t) = s⊆t ∧ ¬(t⊆s)
    return subset(s, t) and not equal(s, t)

def test_proper_subset():
    print("test_proper_subset")
    assert not proper_subset(set(), set())
    assert proper_subset(set(), set([0]))
    assert not proper_subset(set([0]), set([0]))
    assert proper_subset(set([0]), set([0, 1]))
    assert not proper_subset(set([0]), set())
    assert not proper_subset(set([0, 1]), set([0]))

def disjoint(s, t):
    """Test whether *s* and *t* are disjoint."""
    
    ####### Solution without functions above (Part 2) ##########
    """
    for element in s:
        if element in t:
            return False
    return True
    """
    #No element in s should exist in t
    return not exists(s, lambda x: x in t)

def test_disjoint():
    print("test_disjoint")
    assert disjoint(set(), set())
    assert disjoint(set(), set([0]))
    assert not disjoint (set([0]), set([0]))
    assert not disjoint(set([0]), set([0, 1]))
    assert disjoint(set([0]), set())
    assert not disjoint(set([0, 1]), set([0]))
    assert disjoint(set([0]), set([1]))

# Part 3: Implement a Python generator that yields the subsets of a given
# set argument. In this function, you are allowed to use native methods on
# sets, but no external libraries. You may need to read up on generators.

def subsets(s):
    """Yields the subsets of the set *s*."""

    #Current combination
    current = []
    copy = list(s) # [1, 2, 3]

    def inception(i=0):
        if i == len(copy):
            #print(set(current)) # FOR DEBUGGING
            yield set(current)
        else:
            yield from inception(i + 1)
            current.append( copy[i] )
            yield from inception(i + 1)
            current.pop()
    #yield results from different recursive iterations based on 
    #current list of subsets (see picture)
    yield from inception()

def test_subsets():
    print("test_subsets")
    assert len(list(subsets(set()))) == 1
    assert len(list(subsets(set(range(6))))) == 64
    
    # FOR DEBUGGING
    #print("\n== Testing! ==")
    #print(list(subsets(set(range(1,4)))))

if __name__ == '__main__':
    test_forall()
    test_exists()
    test_subset()
    test_equal()
    test_proper_subset()
    test_disjoint()
    test_subsets()