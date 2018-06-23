#define CATCH_CONFIG_MAIN

#include "sortedlist.h"
#include "catch.hpp"


TEST_CASE("Insert function test", "[insert]" )
{
    Sorted_List insert_list;
    //1
    insert_list.insert(5);
    CHECK(insert_list.get(0) == 5); 
    //2
    insert_list.insert(2);
    CHECK(insert_list.get(0) == 2);
    //3
    insert_list.insert(3);
    CHECK(insert_list.get(1) == 3);
    //4
    insert_list.insert(7);
    CHECK(insert_list.get(3) == 7);
}

TEST_CASE("Remove function by index test", "[remove]")
{
    Sorted_List insert_list;
    insert_list.insert(5);
    insert_list.insert(2);
    insert_list.insert(3);
    insert_list.insert(7);

    //2,3,5,7    
    insert_list.remove(2);
    CHECK(insert_list.get(2) == 7);
    //2,3,7

    insert_list.remove(0);
    CHECK(insert_list.get(0) == 3);
    //3,7

    insert_list.remove(1);
    CHECK(insert_list.get(0) == 3);
    //3

    //insert_list.remove(1);
    //CHECK(insert_list == nullptr);
    //...

}

/*TEST_CASE("Print list function test", "[print_list]")
{
    Sorted_List insert_list;
    insert_list.insert(5);
    insert_list.print_list();
    insert_list.insert(2);
    insert_list.print_list();
    insert_list.insert(3);
    insert_list.print_list();
    insert_list.insert(7);
    //2,3,5,7
    insert_list.print_list();
}*/