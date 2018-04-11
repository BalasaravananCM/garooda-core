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


#include "geometric_types.hpp"
#include "utilities.hpp"

namespace cm {

bool are_circles_concentric(circle_i a, circle_i b)
{
    return ( ( std::abs(a.center.x - b.center.x) < b.radius ) &&
             ( std::abs(a.center.y - b.center.y) < b.radius ) );
}

bool are_rectangles_concentric(rectangle_i a, rectangle_i b)
{
    return ( ( std::abs((a.origin.x+a.width/2) - (b.origin.x+b.width/2)) < 10 ) &&
             ( std::abs((a.origin.y+a.height/2) - (b.origin.y+b.height/2)) < 10 ) );
}

bool are_circles_overlapping(circle_i a, circle_i b)
{
    rectangle_i rect_a = cm::get_bounding_rect(a);
    rectangle_i rect_b = cm::get_bounding_rect(b);
    int overlapping_area = cm::overlapping_area(rect_a, rect_b);
    return(overlapping_area>(0.2*rect_b.height*rect_b.width));
}

bool are_rectangles_overlapping(rectangle_i rect_a, rectangle_i rect_b)
{
    int rect_a_area = rect_a.height*rect_a.width;
    int rect_b_area = rect_b.height*rect_b.width;
    int overlapping_area = cm::overlapping_area(rect_a, rect_b);
    if(rect_a_area>rect_b_area)
    {
        return(overlapping_area>(0.2*rect_b.height*rect_b.width));
    }
    else
    {
        return(overlapping_area>(0.2*rect_a.height*rect_a.width));
    }
}

}

