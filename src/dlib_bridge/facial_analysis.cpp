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

#include "facial_analysis.hpp"
#include "../helper_macros.hpp"
#include  "../utilities.hpp"


using namespace std;
using namespace cv;

namespace cm{

namespace lib_bridge{

bool face_keypoints_loader(Mat & in, Mat & out,
                           const algo_pipeline_params & params,
                           computed_objects & result_obj)
{
    try
    {
        result_obj.clear_models();
        result_obj.clear();

        string model_dat_file;

        read_string_param("face_keypoints_loader", model_dat_file);

        if(result_obj.sp_obj.sp.num_parts() == 0)
        {
            dlib::deserialize(model_dat_file) >> result_obj.sp_obj.sp;
            return false;
        }
    }
    catch(...)
    {
        return false;
    }
    return true;
}


bool face_keypoints_detection( Mat & in, Mat & out,
                               const algo_pipeline_params & params,
                               computed_objects & result_obj )
{
    try
    {
        dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

        dlib::array2d<dlib::rgb_pixel> img;
        dlib::assign_image(img, dlib::cv_image<dlib::bgr_pixel>(in));

        std::vector<dlib::rectangle> dets = detector(img);

        CM_LOG << "Number of faces detected: " << dets.size()<< endl;

        for (unsigned long j = 0; j < dets.size(); ++j)
        {
            dlib::full_object_detection shape = result_obj.sp_obj.sp(img, dets[j]);
            result_obj.sp_obj.facial_keypoints.push_back(shape);
        }

    }
    catch(...)
    {
        return false;
    }

    return true;
}

}

}
