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


#ifndef __computed_result__hpp__
#define __computed_result__hpp__

#include "common_includes.hpp"
#include "geometric_types.hpp"

namespace cm {

struct dnn_result
{
    rectangle_i object_rectangle;
    string      object_label;
};

struct dnn_object
{
    cv::dnn::Net        dnn_net;
    vector<string>      dnn_class_names;
    vector<dnn_result>  dnn_results;

    void clear();
};

struct dlib_sp_object
{
    dlib::shape_predictor        sp;
    std::vector<dlib::full_object_detection> facial_keypoints;

    void clear();
};

struct aruco_result{
    vector<vector<Point2f> > aruco_corner_points , rejected_points;
    vector< int > ids;
    Mat tvec , rvec;
};

struct aruco_object
{
    aruco_result aruco_res;
    Ptr<aruco::DetectorParameters> params;
    Mat camMatrix,distCoeffs;

};

class computed_objects
{
public:
    computed_objects();

    void add_result(const rectangle_i& rect_item);

    void add_result(const line_i& rect_item);

    void add_result(const cv::KeyPoint& keypoint_item);

    void add_result(const cv::Mat & descriptor_item);

    void add_result(const cv::Point2f &corner);

    void set_displacement(cm::point_i);

    cm::point_i get_displacement();

    void translate_result(const int pos_x, const int pos_y);

    void displace_result(cm::point_i);

    void clear();

    void clear_models();

    computed_objects operator + (computed_objects const & obj);

    vec_rectangle get_rectangles();

    vec_line    get_lines();

    std::vector<KeyPoint> get_keypoints();

    std::vector<Point2f> get_corners();

    cv::Mat get_descriptor();

    rectangle_i get_tracker_rect();

    void filter_concentric_shapes();

    void filter_overlapping_shapes();

    void set_tracker_rectangle(const rectangle_i& rect_result);

    dnn_object          dnn_obj;
    dlib_sp_object      sp_obj;

    aruco_object       aruco_obj;

private:
    vec_rectangle       rectangles;
    vec_line            lines;
    point_i             anchor_displacement;
    rectangle_i         tracker_rectangle;
    std::vector<KeyPoint>       keypoints;
    cv::Mat                     descriptor;
    std::vector<Point2f>        corners;

    void filter_concentric_rectangles();
    void filter_overlapping_rectangles();

};

}


#endif
