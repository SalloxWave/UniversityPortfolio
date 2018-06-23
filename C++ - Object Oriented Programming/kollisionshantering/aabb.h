//komplettering
//flytta hjälpfunktioner som används internt till private

#ifndef AABB_H
#define AABB_H

struct Point
{
    int x;
    int y;
};

class AABB
{
public:
    AABB(int _top, int _left, int _bottom, int _right);
    AABB(Point corner1, Point corner2);

    bool collision_point(AABB const & from, Point const & to, Point & where) const;

    int get_top() const;
    int get_left() const;
    int get_bottom() const;
    int get_right() const;

    bool intersect(AABB const & a) const;
    AABB min_bounding_box (AABB const & a) const;
    bool will_not_collide(AABB const & from, Point const & to) const;
    bool contain(int x, int y) const;
    bool contain(Point const & point) const;
private:
    int top;
    int left;
    int bottom;
    int right;

    int get_height() const;
    int get_width() const;
};

#endif