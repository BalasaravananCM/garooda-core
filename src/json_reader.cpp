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


#include "json_reader.hpp"
#include "utilities.hpp"

cm::block_param::block_param()
{
    block_name = "";
}

bool cm::algo_pipeline_params::load_file(const std::string &in_file)
{
    std::ifstream in_file_stream(in_file);
    try
    {
        in_file_stream >> root_object;
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool cm::algo_pipeline_params::load_buffer(const std::string &json_buffer)
{
    try
    {
        root_object = json::parse(json_buffer);
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool cm::algo_pipeline_params::setup_methods()
{
    algo_block_params.clear();
    //json block_object = root_object.at(0);

    for(auto itr1 = root_object.begin() ; itr1 != root_object.end() ; ++itr1)
    {
        json obj_itr1 = *itr1;
        for(auto itr2 = obj_itr1.begin() ; itr2 != obj_itr1.end() ; ++itr2)
        {
            json obj_itr2 = *itr2;
            for(auto itr3 = obj_itr2.begin() ; itr3 != obj_itr2.end() ; ++itr3)
            {
                json obj_itr3 = *itr3;
                block_param bp;
                bp.block_name = itr3.key();
                for(auto itr4 = obj_itr3.begin() ; itr4 != obj_itr3.end() ; ++itr4)
                {
                    bp.param_values.insert(std::pair<std::string, json>(itr4.key(), itr4.value()));
                }
                algo_block_params.push_back(bp);
            }
        }
    }

    return true;
}

bool cm::algo_pipeline_params::setup_methods(const std::string &block_name)
{
    algo_block_params.clear();
    json block_object = root_object[block_name];

    for(auto itr1 = block_object.begin() ; itr1 != block_object.end() ; ++itr1)
    {
        json obj_itr1 = *itr1;
        for(auto itr2 = obj_itr1.begin() ; itr2 != obj_itr1.end() ; ++itr2)
        {
            json obj_itr2 = *itr2;
            block_param bp;
            bp.block_name = itr2.key();
            for(auto itr3 = obj_itr2.begin() ; itr3 != obj_itr2.end() ; ++itr3)
            {
                bp.param_values.insert(std::pair<std::string, json>(itr3.key(), itr3.value()));
            }
            algo_block_params.push_back(bp);
        }
    }

    return true;
}

size_t cm::algo_pipeline_params::methods_size()
{
    return algo_block_params.size();
}

std::string cm::algo_pipeline_params::method_name(size_t index_num)
{
    std::string fn_name = "";

    if(index_num >= 0 && index_num < algo_block_params.size())
        fn_name = algo_block_params[index_num].block_name;

    return fn_name;
}

int cm::algo_pipeline_params::get_int_param(const std::string &block_name,
                                            const std::string &param_name,
                                            int default_param_value) const
{
    for(auto itr = algo_block_params.begin() ; itr != algo_block_params.end() ; ++itr)
    {
        if ( itr->block_name == block_name )
        {
            auto param_item = itr->param_values.find(param_name);
            if(param_item != itr->param_values.end())
            {
                return param_item->second.get<int>();
            }
        }
    }
    return default_param_value;
}

double cm::algo_pipeline_params::get_double_param(const std::string &block_name,
                                                  const std::string &param_name,
                                                  double default_param_value) const
{
    for(auto itr = algo_block_params.begin() ; itr != algo_block_params.end() ; ++itr)
    {
        if ( itr->block_name == block_name )
        {
            auto param_item = itr->param_values.find(param_name);
            if(param_item != itr->param_values.end())
            {
                return param_item->second.get<double>();
            }
        }
    }
    return default_param_value;
}

std::string cm::algo_pipeline_params::
get_string_param(const std::string &block_name,
                 const std::string &param_name,
                 std::string default_param_value) const
{
    for(auto itr = algo_block_params.begin() ; itr != algo_block_params.end() ; ++itr)
    {
        if ( itr->block_name == block_name )
        {
            auto param_item = itr->param_values.find(param_name);
            if(param_item != itr->param_values.end())
            {
                return param_item->second.get<std::string>();
            }
        }
    }
    return default_param_value;
}


std::vector<double> cm::algo_pipeline_params::get_vector_double_param(const std::string &block_name,
                                                                      const std::string &param_name,
                                                                      std::vector<double> default_param_value) const
{
    for(auto itr = algo_block_params.begin() ; itr != algo_block_params.end() ; ++itr)
    {
        if ( itr->block_name == block_name )
        {
            auto param_item = itr->param_values.find(param_name);
            if(param_item != itr->param_values.end())
            {
                return param_item->second.get<std::vector<double>>();
            }
        }
    }
    return default_param_value;
}

bool cm::algo_pipeline_params::get_bool_param(const string &block_name,
                                              const string &param_name,
                                              bool default_param_value) const
{
    for(auto itr = algo_block_params.begin() ; itr != algo_block_params.end() ; ++itr)
    {
        if ( itr->block_name == block_name )
        {
            auto param_item = itr->param_values.find(param_name);
            if(param_item != itr->param_values.end())
            {
                int bool_param_int_value = param_item->second.get<int>();
                if(bool_param_int_value == 1)
                    return true;
                else
                    return false;
            }
        }
    }
    return default_param_value;
}


bool cm::algo_pipeline_params::set_int_param(const std::string &block_name,
                                             const std::string &param_name,
                                             int param_value)
{
    for(auto itr = algo_block_params.begin() ; itr != algo_block_params.end() ; ++itr)
    {
        if ( itr->block_name == block_name )
        {
            auto param_item = itr->param_values.find(param_name);
            if(param_item != itr->param_values.end())
            {
                param_item->second = param_value;
                return true;
            }
        }
    }
    return false;
}

bool cm::algo_pipeline_params::set_double_param(const std::string &block_name,
                                                const std::string &param_name,
                                                double param_value)
{
    for(auto itr = algo_block_params.begin() ; itr != algo_block_params.end() ; ++itr)
    {
        if ( itr->block_name == block_name )
        {
            auto param_item = itr->param_values.find(param_name);
            if(param_item != itr->param_values.end())
            {
                param_item->second = param_value;
                return true;
            }
        }
    }
    return false;
}

bool cm::algo_pipeline_params::set_string_param(const std::string &block_name,
                                                const std::string &param_name,
                                                std::string param_value)
{
    for(auto itr = algo_block_params.begin() ; itr != algo_block_params.end() ; ++itr)
    {
        if ( itr->block_name == block_name )
        {
            auto param_item = itr->param_values.find(param_name);
            if(param_item != itr->param_values.end())
            {
                param_item->second = param_value;
                return true;
            }
        }
    }
    return false;
}

bool cm::algo_pipeline_params::set_bool_param(const string &block_name,
                                              const string &param_name,
                                              bool param_value)

{
    for(auto itr = algo_block_params.begin() ; itr != algo_block_params.end() ; ++itr)
    {
        if ( itr->block_name == block_name )
        {
            auto param_item = itr->param_values.find(param_name);
            if(param_item != itr->param_values.end())
            {
                int bool_param_int_value = 0;
                if(param_value == true)
                    bool_param_int_value = 1;
                param_item->second = bool_param_int_value;
                return true;
            }
        }
    }
    return false;
}


bool cm::algo_pipeline_params::set_vector_double_param(const std::string &block_name,
                                                const std::string &param_name,
                                                std::vector<double> param_value)
{
    for(auto itr = algo_block_params.begin() ; itr != algo_block_params.end() ; ++itr)
    {
        if ( itr->block_name == block_name )
        {
            auto param_item = itr->param_values.find(param_name);
            if(param_item != itr->param_values.end())
            {
                param_item->second = param_value;
                return true;
            }
        }
    }
    return false;
}
