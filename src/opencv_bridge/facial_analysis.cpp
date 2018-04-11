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

bool caffe_model_loader(Mat & in, Mat & out,
                        const algo_pipeline_params & params,
                        computed_objects & result_obj){


    try
    {

        result_obj.clear_models();
        result_obj.clear();

        string model_prototxt_file , caffe_model_file;
        string emotions_class_file;

        read_string_param("caffe_model_loader", model_prototxt_file);
        read_string_param("caffe_model_loader", caffe_model_file);
        read_string_param("caffe_model_loader", emotions_class_file);



        if(result_obj.dnn_obj.dnn_net.empty())
        {
            result_obj.dnn_obj.dnn_net = cv::dnn::readNetFromCaffe(model_prototxt_file, caffe_model_file);
            ifstream class_names_file_obj(emotions_class_file);
            if (class_names_file_obj.is_open())
            {
                string name_item = "";
                while (std::getline(class_names_file_obj, name_item))
                    result_obj.dnn_obj.dnn_class_names.push_back(name_item);
            }
            class_names_file_obj.close();
            std::cout << "Model Loaded!" <<endl;

        }
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool torch_model_loader(Mat & in, Mat & out,
                        const algo_pipeline_params & params,
                        computed_objects & result_obj){


    try
    {

        result_obj.clear_models();
        result_obj.clear();

        string torch_model_file;

        read_string_param("torch_model_loader", torch_model_file);

        if(result_obj.dnn_obj.dnn_net.empty())
        {
            result_obj.dnn_obj.dnn_net = cv::dnn::readNetFromTorch(torch_model_file);
            std::cout << "Model Loaded!" <<endl;

        }
    }
    catch(...)
    {
        return false;
    }
    return true;
}



bool facial_analysis(Mat & in, Mat & out,
                        const algo_pipeline_params & params,
                        computed_objects & result_obj)

{

    try {
        string face_extraction_cascade_file;
        std::vector<double> mean_values = {0, 0, 0};
        std::vector<Rect> faces;
        read_string_param("facial_analysis", face_extraction_cascade_file);
        read_vector_double_param("facial_analysis", mean_values);


        if (mean_values.size() != 3){
            mean_values = {0, 0, 0};
        }

        cv::String cascade_file
                = cv::String(face_extraction_cascade_file.c_str());

        faces = cm::extract_faces(in, cascade_file);
        for (auto rect: faces)

         {

            cout << rect.x << ", " << rect.y << "," << rect.width << "," <<rect.height <<endl;
            cout << rect.x << ", " << rect.y << "," << rect.width << "," <<rect.height<<endl;

            cv::Mat face = in(rect);
            Mat inputBlob = cv::dnn::blobFromImage(face,
                                                   1.0f  ,
                                                   Size(256, 256),
                                                   Scalar(mean_values[0], mean_values[1], mean_values[2]), true, false);

            result_obj.dnn_obj.dnn_net.setInput(inputBlob);

            Mat detectionMat = result_obj.dnn_obj.dnn_net.forward();

            cout <<"Output Size" << detectionMat.size<< endl;
            std::vector<float> detected_scores;
            for (int col=0; col < detectionMat.cols; col++)
            {
                cout << detectionMat.at<float>(0, col);
                detected_scores.push_back(detectionMat.at<float>(0, col));
            }
            int index = distance(detected_scores.begin(), max_element(detected_scores.begin(), detected_scores.end()));
            dnn_result result_item;
            if(index < result_obj.dnn_obj.dnn_class_names.size())
            {
                result_item.object_label =
                        result_obj.dnn_obj.dnn_class_names[index];
            }
            result_item.object_rectangle = cm::rect_2_bounding_rect(rect);
            result_obj.dnn_obj.dnn_results.push_back(result_item);
        }

    }
    catch (...){

        return false;
    }
    return true;


}


bool face_registration(Mat & in, Mat & out,
                       const algo_pipeline_params & params,
                       computed_objects & result_obj)
{

    try {
        string save_location;
        string name_identifier;
        int current_sample_number;
        string face_extraction_cascade_file;
        std::vector<Rect> faces;

        read_string_param("face_registration", save_location);
        read_string_param("face_registration", name_identifier);
        read_string_param("face_registration", face_extraction_cascade_file);

        vector<String> files_saved;
        vector<int> previous_samples;
        glob(save_location + "/" + name_identifier + "_" + "*.yml", files_saved);

        std::string file_match;
        std::string name_match;

        for ( String file: files_saved)
        {
            int beginIdx = file.rfind('_');
            if ( std::string::npos !=  beginIdx){
                file_match = file.substr(beginIdx + 1);
                int endIdx = file_match.rfind('.');
                if (std::string::npos != endIdx){
                    name_match = file_match.substr(0, endIdx);
                }
            }

            previous_samples.push_back(stoi(name_match));
        }


        if (!previous_samples.empty()){
        current_sample_number = *max_element(previous_samples.begin(), previous_samples.end());
        }
        else{
            current_sample_number = 1;
        }


        std::cout << "Cascade file:" << face_extraction_cascade_file<<endl;
        std::cout << "Saving embedding for: " << name_identifier << endl;
        std::cout << "Save location:" << save_location <<endl;

        faces = cm::extract_faces(in, face_extraction_cascade_file);
        std::cout << "Found faces: " << faces.size() <<endl;


        std::vector<int> face_areas;
        for ( Rect rect: faces){

            int area = rect.width * rect.height;
            face_areas.push_back(area);




        }

        if ( !faces.empty()){

        int maxface_index = distance(face_areas.begin(), max_element(face_areas.begin(), face_areas.end()));


        cv::Mat face = in(faces[maxface_index]);
        rectangle_i registered_face = cm::rect_2_bounding_rect(faces[maxface_index]);


        cv::Mat inputBlob = cv::dnn::blobFromImage(face, 1. / 255, Size(96, 96), Scalar(), true, false);


        dnn_result result_item;
        result_obj.dnn_obj.dnn_net.setInput(inputBlob);
        Mat detectionMat = result_obj.dnn_obj.dnn_net.forward().clone();
        result_item.object_rectangle = registered_face;
        result_item.object_label = "Registered face:";
        result_obj.dnn_obj.dnn_results.push_back(result_item);
        cout << "Output size: " << detectionMat.size <<endl;

        std::cout << current_sample_number <<endl;
        FileStorage fs(save_location + "/" + name_identifier + "_" + to_string(current_sample_number + 1) +".yml", FileStorage::WRITE);
        fs << "Embeddings" << detectionMat;
        fs.release();
        }
        else
        {
            return false;
        }
    }
    catch(...){
        return false;
    }

    return true;
}

bool face_matching(Mat & in, Mat & out,
                   const algo_pipeline_params & params,
                   computed_objects & result_obj)
{

    string embedding_location;
    string face_extraction_cascade_file;

    read_string_param("face_matching", embedding_location);
    read_string_param("face_matching", face_extraction_cascade_file);

    vector<String> result;
    glob(embedding_location +"/*.yml", result);

    std::vector<Rect> faces;
    faces = cm::extract_faces(in, face_extraction_cascade_file);
    std::cout << "Faces found: " << faces.size() << endl;

    FileStorage fs;
    cv::Mat detectionMat, face, inputBlob;
    dnn_result result_item;
    rectangle_i matched_face;
    Mat stored_embeddings;


    for ( Rect rect: faces){
        face = in(rect);
        inputBlob = cv::dnn::blobFromImage(face, 1. / 255, Size(96, 96), Scalar(), true, false);
        result_obj.dnn_obj.dnn_net.setInput(inputBlob);
        detectionMat = result_obj.dnn_obj.dnn_net.forward().clone();

        std::vector<float> embedding_distance;
        for ( auto file: result){
            try {
            fs.open(file, FileStorage::READ);
            fs["Embeddings"] >> stored_embeddings;
            }
            catch (...){
                continue;
            }

            float score = 0;

            for (int col =0; col < stored_embeddings.cols; col++){
                score  += std::pow(stored_embeddings.at<float>(0, col) - detectionMat.at<float>(0, col), 2);
            }
            std::cout << "File: "<< file << " ,Distance:" << score << endl;
            embedding_distance.push_back(score);
            }
            int minl2score_index = distance(embedding_distance.begin(), min_element(embedding_distance.begin(), embedding_distance.end()));

            if ( embedding_distance[minl2score_index] > 0.15){
                result_item.object_label = "No Match";
                matched_face = cm::rect_2_bounding_rect(rect);
                result_item.object_rectangle = matched_face;

                result_obj.dnn_obj.dnn_results.push_back(result_item);
                return 0;
            }
            int beginIdx = result[minl2score_index].rfind('/');

            std::string name_match;
            std::string file_match;

            if ( std::string::npos !=  beginIdx){
                file_match = result[minl2score_index].substr(beginIdx + 1);
                int endIdx = file_match.rfind('_');
                if (std::string::npos != endIdx){
                    name_match = file_match.substr(0, endIdx);
                }
            }

            result_item.object_label = name_match;
            matched_face = cm::rect_2_bounding_rect(rect);
            result_item.object_rectangle = matched_face;

            result_obj.dnn_obj.dnn_results.push_back(result_item);


    }
    return 0;


}

}

}
