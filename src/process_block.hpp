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

#ifndef __process__block__hpp__
#define __process__block__hpp__

#include "image_holder.hpp"
#include "json_reader.hpp"
#include "cv_image_holder.hpp"
#include "computed_result.hpp"

namespace cm {

class process_block
{
public:

    process_block();

    virtual ~process_block();

    virtual bool set_input_image(image_holder *in_image);

    virtual bool set_output_image(image_holder *out_image);

    virtual bool set_process_block_parameters(const std::string & json_buffer);

    virtual bool set_process_block_parameters(const std::string & json_buffer,
                                              const std::string & block_name);

    virtual bool set_results(computed_objects objs);

    virtual computed_objects get_results();

    virtual bool process_image();

    virtual image_holder * get_output_image();

    virtual bool set_parameter(const std::string &block_name,
                               const std::string &param_name,
                               int value);

    virtual bool set_parameter(const std::string &block_name,
                               const std::string &param_name,
                               double value);

    virtual bool set_parameter(const std::string &block_name,
                               const std::string &param_name,
                               std::string value);

    virtual bool reset();
    virtual bool reset_models();

protected:

    cv_image_holder * input_image, * output_image;

    cv::Mat prev_node_image, next_node_image;

    computed_objects objects_found;

    algo_pipeline_params process_params;
};


}

#endif
