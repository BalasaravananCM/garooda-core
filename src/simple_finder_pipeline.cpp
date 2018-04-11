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

#include "simple_finder_pipeline.hpp"
#include "json_reader.hpp"
#include "utilities.hpp"

namespace cm {

simple_finder_pipeline::simple_finder_pipeline()
{
    finder_process = new process_block();

    input_image = output_image = nullptr;
}

simple_finder_pipeline::~simple_finder_pipeline()
{
    delete finder_process;

    delete input_image;

    delete output_image;
}

bool simple_finder_pipeline::set_input_image(const cv::Mat & in_image)
{
    if(input_image != nullptr)
        delete input_image;

    input_image = new cm::cv_image_holder(in_image);

    return input_image->is_valid();
}

bool simple_finder_pipeline::set_output_image(const cv::Mat & out_image)
{
    if(output_image != nullptr)
        delete output_image;

    output_image = new cm::cv_image_holder(out_image);

    return output_image->is_valid();
}

Mat simple_finder_pipeline::get_output_image()
{
    return output_image->get_cv_matrix();
}

bool simple_finder_pipeline::set_pipeline_configuration(const std::string & json_buffer)
{

    finder_process->reset_models();
    return finder_process->set_process_block_parameters(json_buffer,
                                                 "simple_finder_pipeline");
}

bool simple_finder_pipeline::process_pipeline()
{
    finder_process->reset();

    if ( finder_process->set_input_image(input_image) == false )
    {
        CM_LOG << "Error setting input image for simple finder process \n";
        return false;
    }

    if ( finder_process->set_output_image(output_image) == false )
    {
        CM_LOG << "Error setting output image for simple finder process \n";
        return false;
    }

    return finder_process->process_image();
}


bool simple_finder_pipeline::set_parameter(const std::string &block_name,
                                      const std::string &param_name,
                                      int value)
{
    return finder_process->set_parameter(block_name, param_name, value);
}

bool simple_finder_pipeline::set_parameter(const std::string &block_name,
                                      const std::string &param_name,
                                      double value)
{
    return finder_process->set_parameter(block_name, param_name, value);
}

bool simple_finder_pipeline::set_parameter(const std::string &block_name,
                                      const std::string &param_name,
                                      std::string value)
{
    return finder_process->set_parameter(block_name, param_name, value);
}

}
