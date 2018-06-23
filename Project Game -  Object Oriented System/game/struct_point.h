//
// Created by alexanderjlinux on 11/29/16.
//

#ifndef GAME38_STRUCT_POINT_H
#define GAME38_STRUCT_POINT_H

//! Struct representing a point with an x-position and y-position
struct Point
{
    //! A default contructor
    /*!
     * Sets x-position and y-position to 0
     */
    Point()
        : x {0}, y {0}
    {}

    /*!
     * Constructs a point with specified position
     * @param _x The x-position
     * @param _y The y-position
     */
    Point(int _x, int _y)
        : x {_x}, y {_y}
    {}
    int x;
    int y;
};

#endif //GAME38_STRUCT_POINT_H
