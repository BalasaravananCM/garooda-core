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

#ifndef __opencv__bridge__shape__finder__impl__hpp__
#define __opencv__bridge__shape__finder__impl__hpp__

#include "../common_includes.hpp"
#include "../computed_result.hpp"
#include "../json_reader.hpp"

using namespace cv;
using namespace std;

namespace cm {

namespace lib_bridge {

const int k_threshold_min = 0 ;
const int k_threshold_max = 255 ;

bool circle_finder_impl(Mat& in,
                        double dp,
                        double min_dist,
                        double canny_param_1,
                        double canny_param_2,
                        int min_radius,
                        int max_radius,
                        bool clear_and_add,
                        computed_objects & circles_found);

bool rectangle_finder_impl(Mat &in,
                           int threshold_step,
                           double shape_appoximator_ratio,
                           double min_contour_area,
                           double max_contour_area,
                           double cosine_angle,
                           bool clear_and_add,
                           computed_objects & rectangles_found);

bool line_finder_impl(Mat &in,
                      double rho,
                      double theta,
                      double threshold,
                      double length_factor,
                      double max_seperation,
                      bool clear_and_add,
                      computed_objects & lines_found);

//Utility functions
double angle_between_points(cv::Point pt1, cv::Point pt2, cv::Point pt0);

bool get_tracker(Mat & image1, Mat & image2, cm::rectangle_i &tracker, int min_good_features, int nfeatures);

void drawBoundingBox(Mat image, vector<Point2f> bb);

struct time_counter
{
    TickMeter tm;
    explicit time_counter() {
        tm.start();
    }
    ~time_counter() {
        tm.stop();
        CM_LOG << "FPS = " << 1.0 / tm.getTimeSec() << std::endl;
    }
};


}

}

#endif
