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

#include "filters.hpp"
#include "../helper_macros.hpp"

using namespace std;
using namespace cv;

namespace cm {

namespace lib_bridge {

bool difference_of_gaussian( Mat & in, Mat & out,
                             const algo_pipeline_params & params,
                             computed_objects & result_obj )
{
    cv::Size ksize_1(9,9), ksize_2(3,3);
    double sigmaX = 2.0;
    double sigmaY = 2.0;
    int  k_min_threshold = 127;
    int  k_max_threshold = 255;

    (void)(result_obj);

    try
    {
        Mat out1, out2;
        read_int_param("difference_of_gaussian", ksize_1.width);
        read_int_param("difference_of_gaussian", ksize_1.height);
        read_int_param("difference_of_gaussian", ksize_2.width);
        read_int_param("difference_of_gaussian", ksize_2.height);
        read_double_param("difference_of_gaussian", sigmaX);
        read_double_param("difference_of_gaussian", sigmaY);
        read_double_param("difference_of_gaussian", k_min_threshold);
        read_double_param("difference_of_gaussian", k_max_threshold);

        cv::GaussianBlur(in, out1, ksize_1, sigmaX, sigmaY);
        cv::GaussianBlur(in, out2, ksize_2, sigmaX, sigmaY);

        out = out2 - out1;

        cv::cvtColor( out, out, CV_BGR2GRAY );
        cv::equalizeHist( out, out );
        cv::threshold(out, out, k_min_threshold, k_max_threshold, THRESH_BINARY);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool gaussian_blur( Mat & in, Mat & out,
                    const algo_pipeline_params & params,
                    computed_objects & result_obj )
{
    cv::Size ksize(5,5);
    double sigmaX = 2.0;
    double sigmaY = 2.0;

    (void)(result_obj);

    try
    {
        read_int_param("gaussian_blur", ksize.width);
        read_int_param("gaussian_blur", ksize.height);
        read_double_param("gaussian_blur", sigmaX);
        read_double_param("gaussian_blur", sigmaY);

        cv::GaussianBlur(in, out, ksize, sigmaX, sigmaY);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool median_blur( Mat & in, Mat & out,
                  const algo_pipeline_params & params,
                  computed_objects & result_obj )
{
    int ksize = 5;

    (void)(result_obj);

    try
    {
        read_int_param("median_blur", ksize);

        if(ksize %2==0){
            ksize = ksize+1;
        }

        cv::medianBlur(in, out, ksize);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool bilateral_filter( Mat & in, Mat & out,
                       const algo_pipeline_params & params,
                       computed_objects & result_obj )
{
    int d = 9;
    double sigmaColor = 75.0;
    double sigmaSpace = 75.0;

    (void)(result_obj);

    try
    {
        read_int_param("bilateral_filter", d);
        read_double_param("bilateral_filter", sigmaColor);
        read_double_param("bilateral_filter", sigmaSpace);

        cv::bilateralFilter(in, out, d, sigmaColor, sigmaSpace);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool convert_color_to_gray( Mat & in, Mat & out,
                            const algo_pipeline_params & params,
                            computed_objects & result_obj )
{
    (void)(params);
    (void)(result_obj);

    try
    {
        cv::cvtColor(in, out, CV_BGR2GRAY);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool canny_edges( Mat & in, Mat & out,
                  const algo_pipeline_params & params,
                  computed_objects & result_obj )
{
    double kernel_size = 3.0;
    double min_threshold = 75.0;
    double max_threshold = 150.0;

    (void)(result_obj);

    try
    {
        read_double_param("canny_edges", kernel_size);
        read_double_param("canny_edges", min_threshold);
        read_double_param("canny_edges", max_threshold);

        cv::Canny(in, out, min_threshold, max_threshold, kernel_size);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool otsu_binarization( Mat & in, Mat & out,
                        const algo_pipeline_params & params,
                        computed_objects & result_obj )
{
    double min_threshold = 0.0;
    double max_threshold = 100.0;

    (void)(result_obj);

    try
    {
        read_double_param("otsu_binarization", min_threshold);
        read_double_param("otsu_binarization", max_threshold);

        cv::threshold(out, out, min_threshold, max_threshold, THRESH_BINARY | THRESH_OTSU);
    }
    catch(...)
    {
        return false;
    }

    return true;
}


bool box_filter_blur( Mat & in, Mat & out,
                      const algo_pipeline_params & params,
                      computed_objects & result_obj )
{
    cv::Size ksize(3,3);

    (void)(result_obj);

    try
    {
        read_int_param("box_filter_blur", ksize.width);
        read_int_param("box_filter_blur", ksize.height);

        cv::blur(in, out, ksize);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool laplacian_operator( Mat & in, Mat & out,
                         const algo_pipeline_params & params,
                         computed_objects & result_obj )
{
    int ddepth = 1;
    int ksize = 1;
    double scale = 1.0;
    double delta = 0.0;

    (void)(result_obj);

    try
    {
        read_int_param("laplacian_operator", ddepth);
        read_int_param("laplacian_operator", ksize);
        read_double_param("laplacian_operator", scale);
        read_double_param("laplacian_operator", delta);

        Mat dst;
        cv::Laplacian(in, dst, ddepth, ksize, scale, delta);
        cv::convertScaleAbs(dst, out);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool sobel_operator( Mat & in, Mat & out,
                     const algo_pipeline_params & params,
                     computed_objects & result_obj )
{
    int ddepth = 1;
    int ksize = 1;
    int dx = 0;
    int dy = 0;
    double scale = 1.0;
    double delta = 0.0;

    (void)(result_obj);

    try
    {
        read_int_param("sobel_operator", ddepth);
        read_int_param("sobel_operator", dx);
        read_int_param("sobel_operator", dy);
        read_int_param("sobel_operator", ksize);
        read_double_param("sobel_operator", scale);
        read_double_param("sobel_operator", delta);

        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;

        cv::Sobel(in, grad_x, ddepth, dx, 0, ksize, scale, delta, cv::BORDER_DEFAULT);
        cv::convertScaleAbs(grad_x, abs_grad_x);

        cv::Sobel(in, grad_y, ddepth, 0, dy, ksize, scale, delta, cv::BORDER_DEFAULT);
        cv::convertScaleAbs(grad_y, abs_grad_y);

        cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, out);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

}

}
