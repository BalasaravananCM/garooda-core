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


#include "helper_macros.hpp"
#include "function_table.hpp"

#include "opencv_bridge/filters.hpp"
#include "opencv_bridge/geometric_transform.hpp"
#include "opencv_bridge/shape_finder.hpp"
#include "opencv_bridge/draw_objects.hpp"
#include "opencv_bridge/ml_finder.hpp"
#include "opencv_bridge/feature_extraction.hpp"
#include "opencv_bridge/facial_analysis.hpp"
#include "opencv_bridge/aruco_extraction.hpp"

#include "dlib_bridge/facial_analysis.hpp"

cm::function_table::function_table()
{
    add_algo_block_function(difference_of_gaussian)
    add_algo_block_function(gaussian_blur);
    add_algo_block_function(median_blur);
    add_algo_block_function(bilateral_filter);
    add_algo_block_function(otsu_binarization);
    add_algo_block_function(convert_color_to_gray);
    add_algo_block_function(canny_edges);
    add_algo_block_function(box_filter_blur);
    add_algo_block_function(laplacian_operator);
    add_algo_block_function(sobel_operator);

    add_algo_block_function(sift_feature_detection);
    add_algo_block_function(sift_descriptor_extraction);
    add_algo_block_function(surf_feature_detection);
    add_algo_block_function(surf_descriptor_extraction);
    add_algo_block_function(fast_feature_detection);
    add_algo_block_function(orb_feature_detection);
    add_algo_block_function(orb_descriptor_extraction);

    add_algo_block_function(brief_descriptor_extraction);
    add_algo_block_function(harris_corner_detection);
    add_algo_block_function(shi_tomasi_corner_detection);

    add_algo_block_function(resize);
    add_algo_block_function(set_region_of_interest);
    add_algo_block_function(translate_result);

    add_algo_block_function(circle_finder);
    add_algo_block_function(rectangle_finder);
    add_algo_block_function(line_finder);
    add_algo_block_function(yolo_model_loader);
    add_algo_block_function(yolo_object_finder);

    add_algo_block_function(caffe_model_loader);
    add_algo_block_function(torch_model_loader);

    add_algo_block_function(face_keypoints_loader);
    add_algo_block_function(face_keypoints_detection);

    add_algo_block_function(facial_analysis);
    add_algo_block_function(face_registration);
    add_algo_block_function(face_matching);

    add_algo_block_function(draw_circles);
    add_algo_block_function(draw_rectangles);
    add_algo_block_function(draw_lines);
    add_algo_block_function(draw_labelled_rectangles);
    add_algo_block_function(draw_keypoints);
    add_algo_block_function(draw_corners);
    add_algo_block_function(draw_facial_keypoints);

    add_algo_block_function(geometric_filtering);

    add_algo_block_function(aruco_finder);
    add_algo_block_function(aruco_loader);
    add_algo_block_function(aruco_draw);


}

cm::function_table &cm::function_table::instance()
{
    static cm::function_table global_function_table;

    return global_function_table;
}

cm::algo_block_fptr
cm::function_table::get_algo_block_function(const std::string &function_name)
{
    auto table_instance = function_table::instance();
    auto function_item = table_instance.algo_block_functions.find(function_name);
    if(function_item != table_instance.algo_block_functions.end())
        return function_item->second;
    else
        return nullptr;
}
