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

#ifndef __cm__utilities__hpp__
#define __cm__utilities__hpp__

#include "common_includes.hpp"
#include "geometric_types.hpp"

namespace cm {

std::string extract_function_name(std::string);

cm::circle_i get_circle(cv::Vec3f);

cm::point_i get_cm_point(cv::Point in_pt);

cv::Point get_cv_point(cm::point_i in_pt);

std::vector<cm::point_i> get_cm_contour(std::vector<cv::Point>);

std::vector<cv::Point> get_cv_contour(std::vector<cm::point_i>);

cv::Point get_centroid(std::vector<cv::Point>);

rectangle_i get_bounding_rect(circle_i);

cv::Rect get_cv_rect(const cm::rectangle_i &);

double get_length(line_i);

cm::rectangle_i  rect_2_bounding_rect(Rect2d);

cm::rectangle_i polygon_i_2_bounding_rect(polygon_i);

vector<cm::rectangle_i> vector_polygon_i_2_vector_bounding_rect(std::vector<polygon_i> );

vector<cm::rectangle_i> get_vector_bounding_rect(vector<cm::circle_i>);

cm::rectangle_i bounding_rect_shifting(cm::rectangle_i, int, int);

cm::polygon_i vector_point_2_polygon_i( std::vector<cv::Point> , double );

vector<Point2d> vector_Point_i_2_vector_Point2d(std::vector<cm::point_i> );

std::vector<cv::Point> bounding_rect_2_vector_point(cm::rectangle_i );

std::pair<int, int> find_histogram(int, vector<cm::rectangle_i> );

vector<Rect> extract_faces(Mat in, string cascade_file);

vector<std::pair<int, int>> find_histogram_based_area_groups(int , vector<cm::rectangle_i> );

int overlapping_area(cm::rectangle_i , cm::rectangle_i );

void draw2DPoints(cv::Mat image, std::vector<cv::Point2f> &list_points, cv::Scalar color);

//otsu thresholding
double otsu_threshold(Mat );

Mat translate_image(Mat img, int offset_x, int offset_y);

Mat rotate_image(Mat mg, int rotation_angle);

Mat scale_image(Mat img, int increase_percentage);

vector<Mat> get_geometric_tranforms(Mat img);

float distance_btw_point_i(point_i, point_i);

float get_eucliedean_distance(Point2f a, Point2f b);

template<typename type_t>
std::vector<type_t> add_vectors(std::vector<type_t> a, std::vector<type_t> b)
{
    std::vector<type_t> result;
    result.insert(result.end(), a.begin(), a.end());
    result.insert(result.end(), b.begin(), b.end());

    return result;
}

}

#endif
