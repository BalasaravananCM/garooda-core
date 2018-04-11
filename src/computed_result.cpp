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


#include "computed_result.hpp"
#include "utilities.hpp"

cm::computed_objects::computed_objects()
{
    anchor_displacement.x = anchor_displacement.y = 0;
}

void cm::computed_objects::add_result(const cm::rectangle_i &rect_item)
{
    rectangles.push_back(rect_item);
}

void cm::computed_objects::add_result(const cm::line_i &line_item)
{
    lines.push_back(line_item);
}

void cm::computed_objects::add_result(const cv::KeyPoint &keypoint_item)
{
    keypoints.push_back(keypoint_item);
}

void cm::computed_objects::add_result(const cv::Mat &descriptor_item)
{
    descriptor = descriptor_item.clone();
    descriptor.push_back(descriptor_item);
    CM_LOG <<"descriptor cols : "<<descriptor.cols<<", descriptor rows : "<<descriptor.rows<<std::endl;
}

void cm::computed_objects::add_result(const cv::Point2f &corner)
{
    corners.push_back(corner);
}

void cm::computed_objects::set_displacement(cm::point_i displacement)
{
    anchor_displacement = displacement;
}

cm::point_i cm::computed_objects::get_displacement()
{
    return anchor_displacement;
}

void cm::computed_objects::displace_result(cm::point_i displacement)
{
    for( auto & rectangle : rectangles)
    {
        rectangle.origin.x += displacement.x;
        rectangle.origin.y += displacement.y;
    }

    for( auto & line_item : lines)
    {
        line_item.p1.x += displacement.x;
        line_item.p1.y += displacement.y;
        line_item.p2.x += displacement.x;
        line_item.p2.y += displacement.y;
    }
}

void cm::computed_objects::translate_result(const int pos_x, const int pos_y)
{
    for( auto & rectangle : rectangles)
    {
        rectangle.origin.x += pos_x;
        rectangle.origin.y += pos_y;
    }

    for( auto & line_item : lines)
    {
        line_item.p1.x += pos_x;
        line_item.p1.y += pos_y;
        line_item.p2.x += pos_x;
        line_item.p2.y += pos_y;
    }
    return;
}

void cm::computed_objects::clear()
{
    lines.clear();
    rectangles.clear();
    keypoints.clear();
    descriptor = cv::Mat();
    corners.clear();
    anchor_displacement.x = anchor_displacement.y = 0;
}

void cm::computed_objects::clear_models()
{
    dnn_obj.clear();
    sp_obj.clear();
}

cm::computed_objects cm::computed_objects::operator +(const cm::computed_objects &obj)
{
    computed_objects result;
    result.rectangles = cm::add_vectors<rectangle_i>(rectangles, obj.rectangles);
    result.lines = cm::add_vectors<line_i>(lines, obj.lines);
    return result;
}

cm::rectangle_i cm::computed_objects::get_tracker_rect()
{
    return tracker_rectangle;
}

cm::vec_rectangle cm::computed_objects::get_rectangles()
{
    return rectangles;
}

cm::vec_line cm::computed_objects::get_lines()
{
    return lines;
}

std::vector<KeyPoint> cm::computed_objects::get_keypoints()
{
    return keypoints;
}

std::vector<Point2f> cm::computed_objects::get_corners()
{
    return corners;
}

cv::Mat cm::computed_objects::get_descriptor()
{
    return descriptor;
}

void cm::computed_objects::filter_concentric_shapes()
{
    filter_concentric_rectangles();
}

void cm::computed_objects::filter_overlapping_shapes()
{
    filter_overlapping_rectangles();
}

void cm::computed_objects::set_tracker_rectangle(const cm::rectangle_i &rect_result)
{
    tracker_rectangle = rect_result;
}

void cm::computed_objects::filter_concentric_rectangles()
{
    if(rectangles.size() == 0)
        return;

    std::vector<rectangle_i> filtered_rectangles;

    auto in_itr = rectangles.begin();
    filtered_rectangles.push_back(*in_itr);

    ++in_itr;
    while(in_itr != rectangles.end())
    {
        bool similar_rectangle_added = false;
        for(auto &f_rect : filtered_rectangles)
        {
            if( are_rectangles_concentric(*in_itr, f_rect) )
            {
                similar_rectangle_added = true;
                break;
            }
        }

        if(similar_rectangle_added == false)
        {
            filtered_rectangles.push_back(*in_itr);
        }

        ++in_itr;
    }

    rectangles = filtered_rectangles;
}

void cm::computed_objects::filter_overlapping_rectangles()
{
    if(rectangles.size() == 0)
        return;

    std::vector<rectangle_i> filtered_rectangles;

    auto in_itr = rectangles.begin();
    filtered_rectangles.push_back(*in_itr);

    ++in_itr;
    while(in_itr != rectangles.end())
    {
        bool similar_rectangle_added = false;
        for(auto &f_rect : filtered_rectangles)
        {
            if( are_rectangles_overlapping(*in_itr, f_rect) )
            {
                similar_rectangle_added = true;
                break;
            }
        }

        if(similar_rectangle_added == false)
        {
            filtered_rectangles.push_back(*in_itr);
        }

        ++in_itr;
    }

    rectangles = filtered_rectangles;
}

void cm::dnn_object::clear()
{
    dnn_results.clear();
}

void cm::dlib_sp_object::clear()
{
    facial_keypoints.clear();
}
