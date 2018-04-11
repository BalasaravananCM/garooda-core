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

#ifndef __opencv__bridge__filters__hpp__
#define __opencv__bridge__filters__hpp__

#include "../common_includes.hpp"
#include "../json_reader.hpp"
#include "../computed_result.hpp"

using namespace cv;
using namespace std;

namespace cm {

namespace lib_bridge {

bool difference_of_gaussian     ( Mat & in, Mat & out,
                                const algo_pipeline_params & params,
                                computed_objects & result_obj );

bool gaussian_blur              ( Mat & in, Mat & out,
                                 const algo_pipeline_params & params,
                                 computed_objects & result_obj );

bool median_blur                ( Mat & in, Mat & out,
                                  const algo_pipeline_params & params,
                                  computed_objects & result_obj );

bool bilateral_filter           ( Mat & in, Mat & out,
                                  const algo_pipeline_params & params,
                                  computed_objects & result_obj );

bool convert_color_to_gray      ( Mat & in, Mat & out,
                                  const algo_pipeline_params & params,
                                  computed_objects & result_obj );

bool canny_edges                ( Mat & in, Mat & out,
                                  const algo_pipeline_params & params,
                                  computed_objects & result_obj );

bool otsu_binarization          ( Mat & in, Mat & out,
                                  const algo_pipeline_params & params,
                                  computed_objects & result_obj );

bool box_filter_blur            ( Mat & in, Mat & out,
                                 const algo_pipeline_params & params,
                                 computed_objects & result_obj );

bool laplacian_operator         ( Mat & in, Mat & out,
                                  const algo_pipeline_params & params,
                                  computed_objects & result_obj );

bool sobel_operator             ( Mat & in, Mat & out,
                                  const algo_pipeline_params & params,
                                  computed_objects & result_obj );


}

}


#endif
