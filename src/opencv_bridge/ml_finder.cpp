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

#include "ml_finder.hpp"

#include "../helper_macros.hpp"
#include  "../utilities.hpp"

using namespace std;
using namespace cv;

namespace cm {

namespace lib_bridge {

bool yolo_model_loader( Mat & in, Mat & out,
                        const algo_pipeline_params & params,
                        computed_objects & result_obj )

{
    try
    {

        result_obj.clear_models();
        result_obj.clear();

        string model_configuration_file , model_weights_file;
        string class_names_file;

        read_string_param("yolo_model_loader", model_configuration_file);
        read_string_param("yolo_model_loader", model_weights_file);
        read_string_param("yolo_model_loader", class_names_file);

        cv::String model_configuration
                = cv::String(model_configuration_file.c_str());
        cv::String model_weights
                = cv::String(model_weights_file.c_str());


        if(result_obj.dnn_obj.dnn_net.empty()){
            result_obj.dnn_obj.dnn_net = cv::dnn::readNetFromDarknet(model_configuration,
                                                                     model_weights);
            return false;
        }
        else
        {
            ifstream class_names_file_obj(class_names_file);
            if (class_names_file_obj.is_open())
            {
                string name_item = "";
                while (std::getline(class_names_file_obj, name_item))
                    result_obj.dnn_obj.dnn_class_names.push_back(name_item);
            }
            class_names_file_obj.close();

            return true;
        }
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool yolo_object_finder( Mat & in, Mat & out,
                         const algo_pipeline_params & params,
                         computed_objects & result_obj )

{
    try
    {
        result_obj.clear();

        int in_width = 416;
        int in_height = 416;
        double confidence_threshold = 0.75;

        read_int_param("yolo_object_finder", in_width);
        read_int_param("yolo_object_finder", in_height);
        read_double_param("yolo_object_finder", confidence_threshold);

        Mat inputBlob = cv::dnn::blobFromImage(in,
                                               1 / 255.F,
                                               Size(in_width, in_height),
                                               Scalar(), true, false);

        result_obj.dnn_obj.dnn_net.setInput(inputBlob, "data");

        Mat detectionMat = result_obj.dnn_obj.dnn_net.forward("detection_out");

        for (int i = 0; i < detectionMat.rows; i++)
        {
            const int probability_index = 5;
            const int probability_size = detectionMat.cols - probability_index;
            float *prob_array_ptr = &detectionMat.at<float>(i, probability_index);

            size_t obj_class_index = max_element(prob_array_ptr,
                                                 prob_array_ptr + probability_size)
                    - prob_array_ptr;
            float confidence = detectionMat.at<float>
                    (i, (int)obj_class_index + probability_index);

            if (confidence > confidence_threshold)
            {
                float x_center = detectionMat.at<float>(i, 0) * in.cols;
                float y_center = detectionMat.at<float>(i, 1) * in.rows;
                float width = detectionMat.at<float>(i, 2) * in.cols;
                float height = detectionMat.at<float>(i, 3) * in.rows;
                Point p1(cvRound(x_center - width / 2),
                         cvRound(y_center - height / 2));
                Point p2(cvRound(x_center + width / 2),
                         cvRound(y_center + height / 2));

                dnn_result dnn_result_item;
                dnn_result_item.object_rectangle = cm::rect_2_bounding_rect(
                            Rect(p1, p2));

                if(obj_class_index > 0 &&
                        obj_class_index < result_obj.dnn_obj.dnn_class_names.size())
                {
                    dnn_result_item.object_label =
                            result_obj.dnn_obj.dnn_class_names[obj_class_index];
                }

                result_obj.dnn_obj.dnn_results.push_back(dnn_result_item);
            }
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
