/*
MIT License

Copyright (c) 2018 Cognitive Machines

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifndef __geometric__types__hpp__
#define __geometric__types__hpp__

#include "common_includes.hpp"

namespace cm {

template <typename type_t> struct point {
    point() {}
    point(type_t x_, type_t y_) : x(x_), y(y_) {}
    type_t x;
    type_t y;
};

typedef point<int> point_i;

typedef point<float> point_f;

template <typename source_type, typename dest_type>
inline point<dest_type> change_point_type(point<source_type> point_obj)
{
    point<dest_type> result_point_obj;
    result_point_obj.x = static_cast<dest_type>(point_obj.x);
    result_point_obj.y = static_cast<dest_type>(point_obj.y);
    return result_point_obj;
}

inline point_i get_int_point(point_f pt)
{
    return change_point_type<float, int>(pt);
}


inline point_f get_float_point(point_i pt)
{
    return change_point_type<int, float>(pt);
}

template <typename T> struct rectangle {
    point<T>    origin;
    T           width;
    T           height;
};

typedef rectangle<int> rectangle_i;

typedef rectangle<float> rectangle_f;

template <typename T> struct circle {
    point<T>    center;
    T           radius;
};

typedef circle<int> circle_i;

typedef circle<float> circle_f;

template <typename T> struct line {
    point<T>    p1;
    point<T>    p2;
};

typedef line<int> line_i;

typedef line<float> line_f;

template <typename T> struct polygon {
    std::vector<cm::point_i> contour;
    double perimeter;
    cm::point_i centroid;
};

typedef polygon<int> polygon_i;

typedef polygon<float> polygon_f;

bool are_circles_concentric(circle_i a, circle_i b);

bool are_rectangles_concentric(rectangle_i a, rectangle_i b);

bool are_circles_overlapping(circle_i a, circle_i b);

bool are_rectangles_overlapping(rectangle_i a, rectangle_i b);

typedef std::vector<circle_i> vec_circle;

typedef std::vector<rectangle_i> vec_rectangle;

typedef std::vector<line_i> vec_line;

typedef std::vector<vec_circle> hist_circles;

typedef std::vector<vec_rectangle> hist_rectangles;

typedef std::vector<vec_line> hist_lines;

}
#endif
