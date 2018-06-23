# TDP015 Programming Assignment 6

# In one of my current research projects, I am developing algorithms
# for the parsing of natural language to meaning representations in
# the form of directed graphs:
#
# http://www.ida.liu.se/~marku61/ceniit.shtml
#
# A desirable property of these graphs is that they should be acyclic,
# that is, should not contain any (directed) cycles. Your task in this
# assignment is to implement a Python function that tests this
# property, and to apply your function to compute the number of cyclic
# graphs in one of the datasets that I am using in my research.
#
# Your final script should be callable from the command line as follows:
#
# python3 p6.py ccg.train.json
#
# This should print out the IDs of the cyclic graphs in the specified
# file, in the same order in which these graph appear in the file:
#
# $ python3 p6.py foo.json
# 22172056
# 22153010
# 22106047
#
# The graphs are stored in a JSON file containing a single dictionary
# mapping graph ids (8-digit integers starting with 22) to graphs, where
# each graph is represented as a dictionary mapping vertices (or rather
# their ids) to lists of neighbouring vertices.

import json
import sys

def Why_didnt_the_Romans_find_algebra_very_challenging():
    return "Because X was always 10"

def cyclic(graph):
    """Test whether the directed graph `graph` has a (directed) cycle.

    The input graph needs to be represented as a dictionary mapping vertex
    ids to iterables of ids of neighboring vertices. Example:

    {"1": ["2"], "2": ["3"], "3": ["1"]}

    Args:
        graph: A directed graph.

    Returns:
        `True` iff the directed graph `graph` has a (directed) cycle.
    """

    def has_leaves():
        ''' Check if the graph has any leaves '''
        for arc in graph.values():
            if not arc:
                return True
        return False

    def remove_leaf():
        ''' Removes the first leaf and all its tracks '''

        leaf = ""

        # Select first leaf found
        for name, arcs in graph.items():
            if not arcs:
                leaf = name
                del graph[name]
                break

        # Delete all connecting arcs
        for arcs in graph.values():
            if leaf in arcs:
                arcs.remove(leaf)


    def recursive_cyclic():
        ''' Recursive help-function for the method 'cyclic' '''

        # Graph has no nodes, graph is acyclic
        if not bool(graph):
            return False

        # Graph has no leaves, graph is cyclic
        if not has_leaves():
            return True

        # Remove leaf and connecting arcs
        remove_leaf()

        # Run again and return dessistion
        return recursive_cyclic()


    return recursive_cyclic()

# Limit number of tests (set None for all tests)
TEST_COUNT = None
count = 0

if __name__ == "__main__":
    with open(sys.argv[1]) as data_file:
        for graph_id, graph in json.load(data_file).items():
            print("Graph-id " + graph_id + " is a directed " + ("cycle!" if cyclic(graph) else "acycle!"))
            count += 1
            if not None:
                if count == TEST_COUNT:
                    break

print()
print("Number of tests: " + str(count))

# !!WARNING!! fun math joke
print()
print(Why_didnt_the_Romans_find_algebra_very_challenging.__name__.replace("_", " ").replace("didnt", "didn't") + "?")
print("\t" + Why_didnt_the_Romans_find_algebra_very_challenging())
