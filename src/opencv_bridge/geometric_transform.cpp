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

#include "geometric_transform.hpp"
#include "../helper_macros.hpp"
#include "../utilities.hpp"


using namespace std;
using namespace cv;

namespace cm {

namespace lib_bridge {

bool resize( Mat & in, Mat & out,
             const algo_pipeline_params & params,
             computed_objects & result_obj )
{
    cv::Size dsize(640, 480);
    (void)(result_obj);
    try
    {
        read_int_param("resize", dsize.width);
        read_int_param("resize", dsize.height);

        cv::resize(in, out, dsize);

    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool set_region_of_interest( Mat & in, Mat & out,
                             const algo_pipeline_params & params,
                             computed_objects & result_obj )
{
    int pos_x = 0;
    int pos_y = 0;
    int width = 0;
    int height = 0;
    (void)(result_obj);
    try
    {
        read_int_param("set_region_of_interest", pos_x);
        read_int_param("set_region_of_interest", pos_y);
        read_int_param("set_region_of_interest", width);
        read_int_param("set_region_of_interest", height);

        if(width == 0 || height == 0)
            return true;

        in = cv::Mat(in, cv::Rect(pos_x, pos_y, width, height));
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool translate_result( Mat & in, Mat & out,
                       const algo_pipeline_params & params,
                       computed_objects & result_obj )
{
    int pos_x = 0;
    int pos_y = 0;
    (void)(result_obj);

    try
    {
        read_int_param("translate_result", pos_x);
        read_int_param("translate_result", pos_y);

        if(pos_x == 0 && pos_y == 0)
            return true;

        result_obj.translate_result(pos_x, pos_y);

    }
    catch(...)
    {
        return false;
    }

    return true;
}


bool geometric_filtering( Mat & in, Mat & out,
                          const algo_pipeline_params & params,
                          computed_objects & result_obj )
{
    try
    {
        result_obj.filter_concentric_shapes();
        result_obj.filter_overlapping_shapes();
    }
    catch(...)
    {
        return false;
    }


    return true;
}

}

}
