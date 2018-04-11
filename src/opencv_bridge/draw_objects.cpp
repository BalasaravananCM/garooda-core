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

bool draw_circles( Mat & in, Mat & out,
                   const algo_pipeline_params & params,
                   computed_objects & result_obj )
{
    int line_thickness = 1;
    int red = 255;
    int green = 0;
    int blue = 0;

    try
    {
        read_int_param("draw_circles", line_thickness);
        read_int_param("draw_circles", red);
        read_int_param("draw_circles", green);
        read_int_param("draw_circles", blue);

        const Scalar k_pen_color(blue,green,red);


        CM_LOG << "Number of objects to draw = " <<
                  result_obj.get_rectangles().size() << std::endl;

        for(auto &c : result_obj.get_rectangles())
        {
            int c_radius = ( c.width + c.height ) / 2;
            cv::Point c_centre(c.origin.x + c.width/2 , c.origin.y + c.height/2);
            cv::circle( out,
                        c_centre,
                        c_radius,
                        k_pen_color,
                        line_thickness);
        }
    }
    catch(...)
    {
        return false;
    }


    return true;
}

bool draw_rectangles( Mat & in, Mat & out,
                      const algo_pipeline_params & params,
                      computed_objects & result_obj )
{
    int line_thickness = 1;
    int red = 255;
    int green = 0;
    int blue = 0;

    try
    {
        read_int_param("draw_rectangles", line_thickness);
        read_int_param("draw_rectangles", red);
        read_int_param("draw_rectangles", green);
        read_int_param("draw_rectangles", blue);


        const Scalar k_pen_color(blue,green,red);

        auto found_rectangles = result_obj.get_rectangles();
        CM_LOG << "Number of rectangles = " << result_obj.get_rectangles().size()
               << std::endl;

        for(int i=0; i<found_rectangles.size();i++)
        {
            cv::rectangle(out,
                          cm::get_cv_rect(found_rectangles[i]),
                          k_pen_color,
                          line_thickness);
        }
    }
    catch(...)
    {
        return false;
    }


    return true;
}

bool draw_lines( Mat & in, Mat & out,
                 const algo_pipeline_params & params,
                 computed_objects & result_obj )
{
    int line_thickness = 1;
    int red = 255;
    int green = 0;
    int blue = 0;

    try
    {
        read_int_param("draw_lines", line_thickness);
        read_int_param("draw_lines", red);
        read_int_param("draw_lines", green);
        read_int_param("draw_lines", blue);


        const Scalar k_pen_color(blue,green,red);

        auto found_lines = result_obj.get_lines();

        for(int i=0; i<found_lines.size();i++)
        {
            cv::line(out,
                     cv::Point(cm::get_cv_point(found_lines[i].p1)),
                     cv::Point(cm::get_cv_point(found_lines[i].p2)),
                     k_pen_color,
                     line_thickness);

        }
    }
    catch(...)
    {
        return false;
    }


    return true;
}

bool draw_labelled_rectangles(Mat &in, Mat &out,
                              const algo_pipeline_params &params,
                              computed_objects &result_obj)
{
    int line_thickness = 1;
    int red = 255;
    int green = 0;
    int blue = 0;
    int lred = 0;
    int lgreen = 255;
    int lblue = 0;

    try
    {
        read_int_param("draw_rectangles", line_thickness);
        read_int_param("draw_rectangles", red);
        read_int_param("draw_rectangles", green);
        read_int_param("draw_rectangles", blue);
        read_int_param("draw_rectangles", lred);
        read_int_param("draw_rectangles", lgreen);
        read_int_param("draw_rectangles", lblue);


        const Scalar k_pen_color(blue,green,red);
        const Scalar lk_pen_color(lblue,lgreen,lred);

        auto found_rectangles = result_obj.dnn_obj.dnn_results;
        CM_LOG << "Number of labelled rectangles = " << found_rectangles.size()
               << std::endl;

        for(int i=0; i<found_rectangles.size();i++)
        {
            auto rect_cv = cm::get_cv_rect(found_rectangles[i].object_rectangle);

            cv::rectangle(out,
                          rect_cv,
                          k_pen_color,
                          line_thickness);

            cv::putText(out,
                        found_rectangles[i].object_label.c_str(),
                        rect_cv.tl(),
                        FONT_HERSHEY_SIMPLEX, 0.8,
                        lk_pen_color);
        }
    }
    catch(...)
    {
        return false;
    }


    return true;
}

bool draw_keypoints(Mat &in, Mat &out,
                    const algo_pipeline_params &params,
                    computed_objects &result_obj)
{
    int matcher_flag = 0;
    int red = 255;
    int green = 0;
    int blue = 0;

    try
    {
        read_int_param("draw_keypoints", matcher_flag);
        read_int_param("draw_keypoints", red);
        read_int_param("draw_keypoints", green);
        read_int_param("draw_keypoints", blue);

        const Scalar k_pen_color(blue,green,red);

        CM_LOG << "Number of keypoints = " << result_obj.get_keypoints().size()
               << std::endl;

        auto keypoints = result_obj.get_keypoints();

        drawKeypoints( out, keypoints, out, k_pen_color, matcher_flag );
    }
    catch(...)
    {
        return false;
    }


    return true;
}

bool draw_corners(Mat &in, Mat &out,
                  const algo_pipeline_params &params,
                  computed_objects &result_obj)
{
    int radius = 4;
    int line_thickness = 1;
    int red = 255;
    int green = 0;
    int blue = 0;

    try
    {
        read_int_param("draw_corners", radius);
        read_int_param("draw_corners", line_thickness);
        read_int_param("draw_corners", red);
        read_int_param("draw_corners", green);
        read_int_param("draw_corners", blue);

        const Scalar k_pen_color(blue,green,red);

        auto corners = result_obj.get_corners();

        CM_LOG <<"Number of corners detected = "<<corners.size()<<std::endl;

        for( int i = 0; i < corners.size(); i++ )
        {
            cv::circle( out,
                        corners[i],
                        radius,
                        k_pen_color,
                        line_thickness);
        }
    }
    catch(...)
    {
        return false;
    }


    return true;
}

bool draw_facial_keypoints(Mat &in, Mat &out,
                           const algo_pipeline_params &params,
                           computed_objects &result_obj)
{
    int line_thickness = 1;
    int radius = 2;
    int red = 255;
    int green = 0;
    int blue = 0;
    int lred = 0;
    int lgreen = 255;
    int lblue = 0;

    try
    {
        read_int_param("draw_facial_keypoints", line_thickness);
        read_int_param("draw_facial_keypoints", radius);
        read_int_param("draw_facial_keypoints", red);
        read_int_param("draw_facial_keypoints", green);
        read_int_param("draw_facial_keypoints", blue);
        read_int_param("draw_facial_keypoints", lred);
        read_int_param("draw_facial_keypoints", lgreen);
        read_int_param("draw_facial_keypoints", lblue);

        const Scalar k_pen_color(blue,green,red);
        const Scalar lk_pen_color(lblue,lgreen,lred);


        CM_LOG << "Number of objects = " << result_obj.sp_obj.facial_keypoints.size()
               << std::endl;

        auto facial_keypoints = result_obj.sp_obj.facial_keypoints;

        for (auto shape: facial_keypoints)
        {
            for( int l = 0; l < shape.num_parts(); l++ )
            {
                cv::circle( out,
                            cv::Point(shape.part(l).x(), shape.part(l).y()),
                            radius,
                            k_pen_color,
                            line_thickness);

                cv::putText(out, std::to_string(l+1),
                            cv::Point(shape.part(l).x(), shape.part(l).y()),
                            cv::FONT_HERSHEY_SIMPLEX,
                            0.5 ,
                            lk_pen_color,
                            line_thickness);
            }
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
