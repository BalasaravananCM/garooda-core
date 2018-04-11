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

#include "cv_image_holder.hpp"

cm::cv_image_holder::cv_image_holder()
{
}

cm::cv_image_holder::cv_image_holder(const std::string & in_file)
{
}

cm::cv_image_holder::cv_image_holder(const cv::Mat & in_matrix, bool duplicate_locally)
{
    set_cv_matrix(in_matrix, duplicate_locally);
}

cm::cv_image_holder::~cv_image_holder()
{
}

int cm::cv_image_holder::get_buffer_height() const
{
    return 0;
}


int cm::cv_image_holder::get_buffer_width() const
{
    return 0;
}


int cm::cv_image_holder::get_height() const
{
    return ( cv_image.data != nullptr ) ? cv_image.cols : 0;
}


int cm::cv_image_holder::get_width() const
{
    return ( cv_image.data != nullptr ) ? cv_image.rows : 0;
}


const void * cm::cv_image_holder::get_pixels() const
{
    return ( cv_image.data != nullptr ) ?
                (static_cast < const void * > (cv_image.data)) :
                nullptr;

}

bool cm::cv_image_holder::is_valid() const
{
    return ( cv_image.data != nullptr ) ? true : false;
}

bool cm::cv_image_holder::write_file(const std::string &file_name) const
{
    return cv::imwrite(file_name.c_str(), cv_image);
}

cv::Mat cm::cv_image_holder::get_cv_matrix()
{
    return cv_image;
}

void cm::cv_image_holder::set_cv_matrix(const Mat &in_matrix, bool duplicate_locally)
{
    if(duplicate_locally)
    {
        cv_image = in_matrix.clone();
    }
    else
    {
        cv_image = in_matrix;
    }
}
