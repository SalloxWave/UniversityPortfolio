// komplettering
//Check // 1-4
//Check // 2-4
//Check // 4-9

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "aabb.h"

using namespace std;

//Public
AABB::AABB(int _top, int _left, int _bottom, int _right)
:   top {std::min(_top, _bottom)}, 
    left {std::min(_left, _right)}, 
    bottom {std::max(_top, _bottom)}, 
    right {std::max(_left, _right)}
{}

AABB::AABB(Point corner1, Point corner2)
:   top {std::min(corner1.y, corner2.y)}, 
    left {std::min(corner1.y, corner2.y)}, 
    bottom {std::max(corner1.x, corner2.x)}, 
    right {std::max(corner1.x, corner2.x)} 
{}

bool AABB::collision_point(AABB const & from, Point const & to, Point & where) const
{
    bool collided {false};
    //Get x and y distance to point
    int x_diff {to.x - from.get_left()};
    int y_diff {to.y - from.get_top()};
    //Create collison x and y points
    double collision_x {static_cast<double>(from.get_left())};
    double collision_y {static_cast<double>(from.get_top())};
    //Get longest side length and shortest
    int long_side = max(abs(x_diff), abs(y_diff));
    int short_side = min(abs(x_diff), abs(y_diff));
    //Get which direction is the longest
    bool x_is_longest {false};
    if (abs(x_diff) > abs(y_diff))
    {
        x_is_longest = true;
    }
    //Get shortest step length (longest is 1)
    double long_step = 
        static_cast<double>(long_side) / static_cast<double>(long_side);
    double short_step = 
        static_cast<double>(short_side) / static_cast<double>(long_side);
    //If division resulted in wrong negative value (-/- = +) 
    if (x_diff < 0 && y_diff < 0)
    {
        short_step = -short_step;
        long_step = -long_step;
    }
    //Iterate through longest side
    for (int i {0}; i < long_side && collided == false; ++i)
    {
        //Create box with current collison points (converted to integer)
        AABB temp_box {static_cast<int>(round(collision_y)), 
                       static_cast<int>(round(collision_x)), 
                       static_cast<int>(round(collision_y)) +
                       (from.get_bottom() - from.get_top()),
                       static_cast<int>(round(collision_x)) +
                       (from.get_right() - from.get_left())
                      }; 
        //Check if "this"-box collides with "from"-box
        if (intersect(temp_box))
        {
            collided = true;
        }
        //Change collision x and y depending on x is or y is longest
        else if (x_is_longest)
        {
            collision_x += long_step;
            collision_y += short_step;
        }
        else
        {   
            collision_x += short_step;      
            collision_y += long_step;            
        }
    }
    //Check if "from"-box has collided
    if (collided == true)
    {
        //Update where box is when can't move anymore
        where = {static_cast<int>(round(collision_x)), 
                 static_cast<int>(round(collision_y))};
    }
    return collided;
}

bool AABB::intersect(AABB const & a) const
{
    //Create bounding box from both boxes
    AABB bound_box = min_bounding_box(a);
    //Check if width or height of bounding box is lesser than total width of box boxes
    return (  (bound_box.get_right() - bound_box.get_left()) <= 
              ((a.get_right() - a.get_left()) + get_width())
              && (bound_box.get_bottom() - bound_box.get_top()) <= 
              ((a.get_bottom() - a.get_top()) + get_height())
           );
}

AABB AABB::min_bounding_box (AABB const & a) const
{
    //Return new AABB with the most extreme of each side
    return AABB 
    {
        std::min(top, a.get_top()),
        std::min(left, a.get_left()),
        std::max(bottom, a.get_bottom()),
        std::max(right, a.get_right())
    };
}

bool AABB::will_not_collide(AABB const & from, Point const & to) const
{
    //Create aabb where that point is
    AABB new_aabb { to.y, to.x, to.y + 
                    (from.get_bottom() - from.get_top()), 
                    to.x + (from.get_right() - from.get_left())};
    //Create bounding box from moving box and box on new point
    AABB bound_box {from.min_bounding_box(new_aabb)};

    //If "this" box intersects with bounding box
    return intersect(bound_box) == false;
}

int AABB::get_height() const
{   
    return (bottom - top);
}
int AABB::get_width() const
{
    return (right - left);
}

int AABB::get_top() const
{
    return top;
}
int AABB::get_left() const
{
    return left;
}
int AABB::get_bottom() const
{
    return bottom;
}
int AABB::get_right() const
{
    return right;
}

bool AABB::contain(int x, int y) const
{
    //If point is within the AABB box
    return (x <= right && x >= left
        && y <= bottom && y >= top);
}
bool AABB::contain(Point const & point) const
{
    //If point is within the AABB box
    return (point.x <= right && point.x >= left
        && point.y <= bottom && point.y >= top);
}