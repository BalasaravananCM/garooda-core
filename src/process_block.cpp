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

#include "process_block.hpp"
#include "function_table.hpp"

cm::process_block::process_block()
{
    input_image = output_image = nullptr;
}

cm::process_block::~process_block()
{
    if(input_image){
        delete input_image;
    }

    if(output_image){
        delete output_image;
    }
}

bool cm::process_block::set_input_image(cm::image_holder *in_image)
{
    input_image = dynamic_cast<cv_image_holder*>(in_image);

    return (input_image != nullptr);
}

bool cm::process_block::set_output_image(cm::image_holder *out_image)
{
    output_image = dynamic_cast<cv_image_holder*>(out_image);

    return (output_image != nullptr);
}

bool cm::process_block::set_process_block_parameters(const std::string & json_buffer)
{
    if(process_params.load_buffer(json_buffer) == false)
    {
        return false;
    }

    if(process_params.setup_methods() == false)
    {
        return false;
    }

    return true;
}

bool cm::process_block::set_process_block_parameters(const std::string & json_buffer,
                                                     const std::string & block_name)
{

    if(process_params.load_buffer(json_buffer) == false)
    {
        return false;
    }

    if(process_params.setup_methods(block_name) == false)
    {
        return false;
    }

    return true;
}

bool cm::process_block::set_results(computed_objects objs)
{
    objects_found = objs;
    return true;
}

cm::computed_objects cm::process_block::get_results()
{
    return objects_found;
}

bool cm::process_block::process_image()
{
    if(process_params.methods_size() == 0)
        return false;

    if(input_image == nullptr)
        return false;

    cv::Mat input_matrix = input_image->get_cv_matrix();
    cv::Mat output_matrix;

    if(output_image != nullptr)
    {
        output_matrix = output_image->get_cv_matrix();
    }

    if(input_matrix.data == nullptr)
        return false;

    if(output_matrix.data == nullptr)
        output_matrix = input_matrix;

    for(int itr = 0 ; itr < process_params.methods_size() ; itr++)
    {
        if(itr == 0)
        {
            prev_node_image = input_matrix;
            next_node_image = input_matrix;
        }

        if(itr == process_params.methods_size() - 1)
        {
            next_node_image = output_matrix;
        }

        auto method_name = process_params.method_name(itr);

        auto algo_fptr = cm::function_table::get_algo_block_function(method_name);

        if(algo_fptr != nullptr)
        {
            CM_LOG << "Running process block " << method_name << std::endl;

            algo_fptr(prev_node_image, next_node_image, process_params, objects_found);

            prev_node_image = next_node_image;
        }

        if(itr == process_params.methods_size() - 1)
        {
             output_image->set_cv_matrix(next_node_image);
        }
    }

    return true;
}

cm::image_holder *cm::process_block::get_output_image()
{
    return output_image;
}

bool cm::process_block::set_parameter(const std::string &block_name,
                                      const std::string &param_name,
                                      int value)
{
    return process_params.set_int_param(block_name, param_name, value);
}

bool cm::process_block::set_parameter(const std::string &block_name,
                                      const std::string &param_name,
                                      double value)
{
    return process_params.set_double_param(block_name, param_name, value);
}

bool cm::process_block::set_parameter(const std::string &block_name,
                                      const std::string &param_name,
                                      std::string value)
{
    return process_params.set_string_param(block_name, param_name, value);
}

bool cm::process_block::reset()
{
    objects_found.clear();

    return true;
}

bool cm::process_block::reset_models()
{
    if (objects_found.dnn_obj.dnn_net.empty()){
        return true;
    }
    dnn::Net reset_dnn;
    objects_found.dnn_obj.dnn_net = reset_dnn;
    return true;
}
