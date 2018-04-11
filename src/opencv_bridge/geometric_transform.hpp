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

#ifndef __opencv__bridge__geometric__transforms__hpp__
#define __opencv__bridge__geometric__transforms__hpp__

#include "../common_includes.hpp"
#include "../json_reader.hpp"
#include "../computed_result.hpp"

using namespace cv;
using namespace std;

namespace cm {

namespace lib_bridge {

bool resize                 ( Mat & in, Mat & out,
                              const algo_pipeline_params & params,
                              computed_objects & result_obj );

bool set_region_of_interest ( Mat & in, Mat & out,
                              const algo_pipeline_params & params,
                              computed_objects & result_obj );

bool translate_result       ( Mat & in, Mat & out,
                              const algo_pipeline_params & params,
                              computed_objects & result_obj );

bool geometric_filtering    ( Mat & in, Mat & out,
                              const algo_pipeline_params & params,
                              computed_objects & result_obj );
}

}


#endif
