#
# Makefile
#
# Om avlusning ska göras måste -g läggas till CCFLAGS.
#

    CCC  = g++	
CCFLAGS += -std=c++11 -pedantic -Wall

avl_tree-test: avl_tree-test.o
	$(CCC) $(CCFLAGS) -o avl_tree-test avl_tree-test.o

avl_tree-test.o: avl_tree-test.cc AVL_Tree.cc
	$(CCC) $(CCFLAGS) -c avl_tree-test.cc

clean:
	@ \rm -f *.o

zap: clean
	@ \rm -rf avl_tree-test *.o *~ 
