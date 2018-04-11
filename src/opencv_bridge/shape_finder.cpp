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

#include "shape_finder.hpp"
#include "../helper_macros.hpp"
#include  "../utilities.hpp"
#include "geometric_transform.hpp"

using namespace std;
using namespace cv;

namespace cm {

namespace lib_bridge {

bool circle_finder( Mat & in, Mat & out,
                    const algo_pipeline_params & params,
                    computed_objects & result_obj )
{
    double dp = 2.0;
    double minDist = 0.0;
    double cannyParam1 = 100.0;
    double cannyParam2 = 100.0;
    int minRadius = 0;
    int maxRadius = 0;
    bool iterativeFinder = false;

    try
    {
        read_double_param("circle_finder", dp);
        read_double_param("circle_finder", minDist);
        read_double_param("circle_finder", cannyParam1);
        read_double_param("circle_finder", cannyParam2);
        read_int_param("circle_finder", minRadius);
        read_int_param("circle_finder", maxRadius);
        read_bool_param("circle_finder", iterativeFinder);

        if(iterativeFinder == true)
        {
            vector<Mat> images = cm::get_geometric_tranforms(in);

            for(auto &img : images)
            {
                circle_finder_impl(img, dp, minDist, cannyParam1, cannyParam2,
                                   minRadius, maxRadius, false, result_obj);
            }
        }
        else
        {
            circle_finder_impl(in, dp, minDist, cannyParam1, cannyParam2,
                               minRadius, maxRadius, true, result_obj);
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool rectangle_finder( Mat & in, Mat & out,
                       const algo_pipeline_params & params,
                       computed_objects & result_obj )
{
    int threshold_step = 45;
    double shape_appoximator_ratio = 0.03;
    double min_contour_area = 0.001;
    double max_contour_area = 0.25;
    double cosine_angle = 0.4;
    bool iterativeFinder = false;

    try
    {
        read_int_param("rectangle_finder", threshold_step);
        read_double_param("rectangle_finder", shape_appoximator_ratio);
        read_double_param("rectangle_finder", min_contour_area);
        read_double_param("rectangle_finder", max_contour_area);
        read_double_param("rectangle_finder", cosine_angle);
        read_bool_param("rectangle_finder", iterativeFinder);

        if(iterativeFinder == true)
        {
            vector<Mat> images = cm::get_geometric_tranforms(in);

            for(auto &img : images)
            {
                rectangle_finder_impl(img, threshold_step, shape_appoximator_ratio,
                                      min_contour_area, max_contour_area,
                                      cosine_angle, false, result_obj);
            }
        }
        else
        {
            rectangle_finder_impl(in, threshold_step, shape_appoximator_ratio,
                                  min_contour_area, max_contour_area,
                                  cosine_angle, true, result_obj);
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool line_finder( Mat & in, Mat & out,
                  const algo_pipeline_params & params,
                  computed_objects & result_obj )
{
    double rho = 0.0;
    double theta = 0.0;
    double threshold = 100.0;
    double length_factor = 0.20;
    double max_seperation = 20.0;
    bool iterativeFinder = 0;

    try
    {
        read_double_param("line_finder", rho);
        read_double_param("line_finder", theta);
        read_double_param("line_finder", threshold);
        read_double_param("line_finder", length_factor);
        read_double_param("line_finder", max_seperation);
        read_bool_param("line_finder", iterativeFinder);

        if(iterativeFinder == true)
        {
            vector<Mat> images = cm::get_geometric_tranforms(in);

            for(auto &img : images)
            {
                line_finder_impl(img, rho, theta, threshold,
                                 length_factor, max_seperation,
                                 false, result_obj);
            }
        }
        else
        {
            line_finder_impl(in, rho, theta, threshold,
                             length_factor, max_seperation,
                             true, result_obj);
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

}

}
