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

#ifndef __json__reader__hpp__
#define __json__reader__hpp__

#include "common_includes.hpp"

namespace cm {

struct block_param
{
    std::string block_name;
    std::map<std::string, json> param_values;
    block_param();
};

class algo_pipeline_params
{
public:

    bool load_file(const std::string &file_name);

    bool load_buffer(const std::string &json_buffer);

    bool setup_methods(const std::string &block_name);

    bool setup_methods();

    int get_int_param(const std::string &block_name,
                      const std::string &param_name,
                      int default_param_value) const;

    double get_double_param(const std::string &block_name,
                            const std::string &param_name,
                            double default_param_value) const;

    std::string get_string_param(const std::string &block_name,
                                 const std::string &param_name,
                                 std::string default_param_value) const;

    bool get_bool_param(const std::string &block_name,
                        const std::string &param_name,
                        bool default_param_value) const;

    std::vector<double> get_vector_double_param(const std::string &block_name,
                                                const std::string &param_name,
                                                std::vector<double> default_param_value) const;

    bool set_int_param(const std::string &block_name,
                       const std::string &param_name,
                       int param_value);

    bool set_double_param(const std::string &block_name,
                          const std::string &param_name,
                          double param_value);

    bool set_string_param(const std::string &block_name,
                          const std::string &param_name,
                          std::string param_value);

    bool set_bool_param(const std::string &block_name,
                        const std::string &param_name,
                        bool param_value);

    bool set_vector_double_param(const std::string &block_name,
                                 const std::string &param_name,
                                 std::vector<double> param_value);


    size_t methods_size();

    std::string method_name(size_t index_num);

private:

    json root_object;

    std::vector<block_param> algo_block_params;
};

}



#endif


