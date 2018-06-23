// This test program uses a test framework supporting TDD and BDD.
// You are not required to use the framework, but encouraged to.
// Documentation:
// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md

// You ARE however required to implement all test cases outlined here,
// even if you do it by way of your own function for each case.  You
// are recommended to solve the cases in order, and rerun all tests
// after you modify your code.

// This define lets Catch create the main test program
// (Must be in only one place!)
#include "catch.hpp"
#include "sortedlist.h"
#include <random>

//=======================================================================
// Test cases
//=======================================================================

TEST_CASE( "Create an empty list" ) 
{
    Sorted_List l{};

    REQUIRE( l.is_empty() );
    REQUIRE( l.size() == 0 );
}



TEST_CASE( "Insert an item in an empty list" ) 
{
    Sorted_List l{};

    REQUIRE( l.is_empty() == true);
    l.insert(5);    
    REQUIRE( l.size() == 1 );
    REQUIRE( l.is_empty() == false );  
}



SCENARIO( "Empty lists" )
{  
    GIVEN( "An empty list" ) 
    {
        Sorted_List l{};
        
        WHEN( "an item is inserted" )
        {
            l.insert(5);
            
            THEN( "the size increase and the item is first in the list" )
            {
            REQUIRE( l.is_empty() == false );
            REQUIRE( l.size() == 1 );
            // test that item is first in list
            REQUIRE( l.get(0) == 5);
            }
        }
        
        WHEN( "an item is removed" )
        {
            l.remove(0);
            THEN( "the list is still empty" )
            {
                REQUIRE( l.is_empty() == true );
                REQUIRE( l.size() == 0 );
            }
        }
        
        WHEN( "the list is copied to a new list" )
        {
            // copy your list to a new variable (copy constructor)
            Sorted_List newList {l};
          
            THEN( "the new list is also empty" )
            {
                REQUIRE(newList.is_empty() == true);
                REQUIRE(newList.size() == 0);
            }
        }
    
        WHEN( "the list is copied to an existing non-empty list" )
        {
            // create and fill a list to act as the existing list
            Sorted_List newList;
            // copy (assign) your empty list to the existing
            l = newList;
          
            THEN( "the existing list is also empty" )
            {
                REQUIRE(l.is_empty() == true);
                REQUIRE(l.size() == 0);
            }
          
        }
    }
}



SCENARIO( "Single item lists" )
{

    GIVEN( "A list with one item in it" )
    {
        // create the given scenario
        Sorted_List l{};
        l.insert(3);
    
        
        WHEN( "a smaller item is inserted" )
        {        
            l.insert(2);
            THEN( "the smaller item will be at index 0" )
            {
                REQUIRE(l.get(0) == 2);
            }
        }
        
        
        WHEN( "a larger item is inserted" )
        {        
            l.insert(7);
            THEN( "the larger item will be at last index (2)" )
            {
                REQUIRE(l.get(1) == 7);
            }
        }
        
        WHEN( "an item is removed" )
        {        
            l.remove(0);
            THEN( "size of the list will be 2" )
            {
                REQUIRE(l.size() == 0);
            }
        }
        
        WHEN( "the list is copied to a new list" )
        {        
            Sorted_List newList {l};
            THEN( "new list has same first value and has size 1" )
            {
                REQUIRE(newList.size() == 1);
                REQUIRE(newList.get(0) == l.get(0));
            }
        }

        WHEN ("an item is removed from the new list")
        {
            Sorted_List newList {l};
            newList.remove(0);
            THEN("new list is empty and list still has one value")
            {
                REQUIRE(l.size() == 1);
                REQUIRE(newList.size() == 0);
            }
        }

        WHEN( "the list is copied to an existing non-empty list" )
        {
            Sorted_List newList;
            Sorted_List nonEmpty {l};
            newList = nonEmpty;
            THEN( "The list will still have size 2, existing list as well" )
            {
                REQUIRE(nonEmpty.size() == 1);
                REQUIRE(newList.size() == 1);            
            }
        }
    }
}


SCENARIO( "Multi-item lists" )
{

    GIVEN( "A list with five items in it" )
    {
        Sorted_List l{};
        l.insert(5);
        l.insert(7);
        l.insert(8);
        l.insert(9);
        l.insert(11);

    // create the given scenario and all THEN statements
    // and all REQUIRE statements
    
    WHEN( "an item smaller than all other is inserted" )
    {
        l.insert(4);
        THEN( "first value of list is the inserted value")
        {
            REQUIRE(l.get(0) == 4);
        }
    }
    
    WHEN( "an item larger than all other is inserted" )
    {
        l.insert(12);
        THEN( "last value in list if the inserted value")
        {
            REQUIRE(l.get(5) == 12);
        }
    }
    
    WHEN( "an item smaller than all but one item is inserted" )
    {
        l.insert(6);
        THEN( "second value in list is inserted value")
        {
            REQUIRE(l.get(1) == 6);
        }
    }
    
    WHEN( "an item larger than all but one item is inserted" )
    {
        l.insert(10);
        THEN( "value before last is inserted value")
        {
            REQUIRE(l.get(4) == 10);
        }
    }
    
    WHEN( "an item is removed" )
    {
        l.remove(2);
        THEN( "the list size is 4")
        {
            REQUIRE(l.size() == 4);
            REQUIRE(l.is_empty() == false);
        }
    }

    WHEN( "all items are removed" )
    {
        l.remove(0);
        l.remove(0);
        l.remove(0);
        l.remove(0);
        l.remove(0);
        THEN( "list is empty")
        {
            REQUIRE(l.size() == 0);
            REQUIRE(l.is_empty() == true);
        }
    }
    WHEN( "the list is copied to a new list" )
    {
        Sorted_List newList {l};
        THEN( "first and second value value is the same for both lists")
        {
            REQUIRE(newList.get(0) == l.get(0));
            REQUIRE(newList.get(1) == l.get(1));
        }
    }
    WHEN( "both lists have the same size" )
    {
        Sorted_List newList;
        newList = l;
        THEN( "last value is the same for both lists")
        {
            REQUIRE(newList.get(4) == l.get(4));
        }
    }
    }
}

SCENARIO( "Lists can be copied" )
{

    GIVEN( "A list with five items in it, and a new copy of that list" )
    {
        Sorted_List l{};
        l.insert(5);
        l.insert(7);
        l.insert(8);
        l.insert(9);
        l.insert(11);

        Sorted_List copy{l};

        WHEN( "the original list is changed" )
        {
            l.remove(0);
            THEN( "the copy remain unmodified" )
            {
                REQUIRE(copy.size() == 5);
                REQUIRE(copy.get(0) == 5);
                REQUIRE(copy.get(4) == 11);
            }
        }

        WHEN( "the copied list is changed" )
        {
            copy.remove(0);
            THEN( "the original remain unmodified" )
            {
                REQUIRE(l.size() == 5);
                REQUIRE(l.get(0) == 5);
                REQUIRE(l.get(4) == 11);
            }
        }
    }
}



SCENARIO( "Lists can be heavily used" )
{

    GIVEN( "A list with 1000 random items in it" )
    {    
        // create the given list with 1000 random items
        Sorted_List l{};
        std::random_device rd;
        std::uniform_int_distribution<int> uniform(1,1000);
        
        for (int i{0}; i < 1000; ++i)
        {
            int random { uniform(rd) }; // generate a random number
            l.insert(random);        
        }
        
        WHEN( "the list have 1000 random numbers inserted" )
        {
            std::random_device rd;
            std::uniform_int_distribution<int> uniform(1,1000);
            
            for (int i{0}; i < 1000; ++i)
            {
                int random { uniform(rd) }; // generate a random number
                l.insert(random);        
            }
            THEN("the list have 2000 items in correct order")
            {
                REQUIRE(l.size() == 2000);
                REQUIRE(l.get(0) <= l.get(1));
                REQUIRE(l.get(50) <= l.get(51));
                REQUIRE(l.get(500) <= l.get(501));
                REQUIRE(l.get(998) <= l.get(999));
            }
            // (assumes unique inserts or duplicates allowed)
        }

        WHEN( "the list have 1000 random numbers removed" )
        {
            int list_size = l.size();
            for (int i {0}; i < list_size; ++i)
            {
                l.remove(0);
            }
            // THEN the list is empty
            // (assumes successful removes)
        }
    }
}


Sorted_List trigger_move(Sorted_List l)
{
    // do some (any) modification to list
    l.insert(1000);
    l.insert(101);
    l.remove(0);
    return l;
}

SCENARIO( "Lists can be passed to functions" )
{

    GIVEN( "A list with 5 items in it" )
    {
        Sorted_List given{};
        // insert 5 items
        given.insert(5);
        given.insert(7);
        given.insert(8);
        given.insert(9);
        given.insert(11);            
      
        WHEN( "the list is passed to trigger_move()" )
        {
            Sorted_List l{ trigger_move(given) };
          
            THEN( "the given list remain and the result have the change" )
            {
                REQUIRE(given.get(0) == 5);
                REQUIRE(given.get(4) == 11);
                REQUIRE(given.size() == 5);

                REQUIRE(l.size() == 6);
                REQUIRE(l.get(0) == 7);
                REQUIRE(l.get(5) == 1000);
            }
        }
    }
}

// Solve one TEST_CASE or WHEN at a time!
//
// Move this comment and following #if 0 down one case at a time!
// Make sure to close any open braces before this comment.
// The #if 0 will disable the rest of the file.
#if 0

// In addition you must of course verify that the list is printed
// correct and that no memory leaks occur during use. You can solve
// the printing part on your own. Here's how to run the (test) program
// when you check for memory leaks:

// You must of course verify that no memory leaks occur during use.
// To do so, an external program must be used to track what memory
// is allocated and free'd, you can run such a program like so:
//
// $ valgrind --tool=memcheck a.out

// Finally you need to check that you can do all operations that make
// sense also on a immutable list (eg all operations but insert):  
void use_const_list(Sorted_List const& l)
{
    // perform every operation that do not modify the list here
    return l;
}

#endif
