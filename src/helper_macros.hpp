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


#ifndef __opencv__bridge__helper__macros__hpp__
#define __opencv__bridge__helper__macros__hpp__

#define read_int_param(block_name, param_name) { \
    (param_name) = params.get_int_param( (block_name), (#param_name), (param_name)); \
    }

#define read_double_param(block_name, param_name) { \
    (param_name) = params.get_double_param((block_name), (#param_name), (param_name)); \
    }

#define read_bool_param(block_name, param_name) { \
    (param_name) = params.get_bool_param((block_name), (#param_name), (param_name)); \
    }

#define read_string_param(block_name, param_name) { \
    (param_name) = params.get_string_param((block_name), (#param_name), (param_name)); \
    }

#define read_vector_double_param(block_name, param_name) { \
    (param_name) = params.get_vector_double_param((block_name), (#param_name), (param_name)); \
    }

#define add_algo_block_function(function_name) { \
    algo_block_functions[#function_name] = &cm::lib_bridge::function_name ; \
    }


#endif
