#include <iostream>

#include "AABB.h"

using namespace std;

int main()
{
    AABB my_aabb {5, 2, 8, 15};
    AABB my_aabb2 {15, 12, 25, 18};
    Point point {50, 30};

    if (my_aabb2.will_not_collide(my_aabb, point))
    {
        cout << "Will not collide!" << endl;
    }
    else
    {
        cout << "Will collide!" << endl;   
    }

    //cout << my_aabb.intersect(my_aabb2) << endl;


    //cout << my_aabb.get_width() << endl;

    return 0;
}