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

#ifndef __object_finder__pipeline__hpp__
#define __object_finder__pipeline__hpp__

#include "common_includes.hpp"
#include "process_block.hpp"
#include "cv_image_holder.hpp"

namespace cm {

class simple_finder_pipeline
{

public:

    simple_finder_pipeline();

    ~simple_finder_pipeline();

    bool set_input_image(const cv::Mat & in_image);

    bool set_output_image(const cv::Mat & out_image);

    cv::Mat get_output_image();

    bool set_pipeline_configuration(const std::string & config_buffer);

    bool process_pipeline();

    bool set_parameter(const std::string &block_name,
                       const std::string &param_name,
                       int value);

    bool set_parameter(const std::string &block_name,
                       const std::string &param_name,
                       double value);

    bool set_parameter(const std::string &block_name,
                       const std::string &param_name,
                       std::string value);

protected:

    cv_image_holder * input_image, * output_image;

    process_block * finder_process;
};

}

#endif
