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


#ifndef __cv__image__holder__hpp__
#define __cv__image__holder__hpp__

#include "common_includes.hpp"
#include "image_holder.hpp"

namespace cm {

class cv_image_holder : public image_holder
{
public:
    cv_image_holder();
    cv_image_holder(const std::string & in_file);
    cv_image_holder(const cv::Mat & in_matrix, bool duplicate_locally = false);
    ~cv_image_holder();

    virtual int get_buffer_height() const;
    virtual int get_buffer_width() const;
    virtual int get_height() const;
    virtual int get_width() const;
    virtual const void * get_pixels() const;
    virtual bool is_valid() const;
    virtual bool write_file(const std::string &file_name) const;

    cv::Mat get_cv_matrix();

    void    set_cv_matrix(const cv::Mat & in_matrix, bool duplicate_locally = false);

private:
    cv::Mat cv_image;
};

}

#endif
