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

#include "common_includes.hpp"

#include "simple_finder_pipeline.hpp"
#include "tracker_pipeline.hpp"

#include "opencv2/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

int simple_finder_example(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cout << "./ipe_example <shape_finder_config>\n";
        return 0;
    }

    std::string config_file = argv[1];

    cm::simple_finder_pipeline sfp;
    std::ifstream file_stream(config_file);
    std::string json_buffer((std::istreambuf_iterator<char>(file_stream)),
                            std::istreambuf_iterator<char>());

    sfp.set_pipeline_configuration(json_buffer);

    VideoCapture video_stream(0);
    if(!video_stream.isOpened())
        return -1;

    for( ; ; )
    {
        Mat frame;
        video_stream >> frame;
        if(frame.data == nullptr)
            break;

        cv::imshow("Simple finder - In frame" , frame);

        Mat out_frame = frame.clone();
        sfp.set_input_image(frame);
        sfp.set_output_image(out_frame);
        sfp.process_pipeline();

        cv::imshow("Simple finder - Out frame" , sfp.get_output_image());

        if(waitKey(1) == 27)
            break;
    }

    return 0;
}

int simple_finder_image_example(int argc, char **argv)
{
    if(argc != 3)
    {
        std::cout << "./ipe_example <shape_finder_config> <image_path>\n";
        return 0;
    }

    std::string config_file = argv[1];
    std::string image_path = argv[2];
    cm::simple_finder_pipeline sfp;
    std::ifstream file_stream(config_file);
    std::string json_buffer((std::istreambuf_iterator<char>(file_stream)),
                            std::istreambuf_iterator<char>());

    sfp.set_pipeline_configuration(json_buffer);


        Mat frame = cv::imread(image_path);
        if(frame.data == nullptr)
            return 0;

        cv::imshow("Simple finder - In frame" , frame);

        Mat out_frame = frame.clone();
        sfp.set_input_image(frame);
        sfp.set_output_image(out_frame);
        sfp.process_pipeline();

        cv::imshow("Simple finder - Out frame" , sfp.get_output_image());

       waitKey();



    return 0;
}

int tracker_example(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cout << "./ipe_example <reference_image>\n";
        return 0;
    }

    std::string ref_file = argv[1];

    cm::tracker_pipeline tp;

    VideoCapture video_stream(2);
    if(!video_stream.isOpened())
        return -1;

    Mat ref_image = cv::imread(ref_file);
    tp.set_reference_image(ref_image);

    for( ; ; )
    {
        Mat frame;
        video_stream >> frame;
        if(frame.data == nullptr)
            break;

        Mat out_frame = frame.clone();
        tp.set_input_image(frame);
        tp.set_output_image(out_frame);
        tp.process_pipeline();

        cv::imshow("Tracker" , out_frame);
        if(waitKey(1) == 27)
            break;
    }

    return 0;
}

int face_registration_example(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cout << "./ipe_example <shape_finder_config>\n";
        return 0;
    }

    std::string config_file = argv[1];

    cm::simple_finder_pipeline sfp;
    std::ifstream file_stream(config_file);
    std::string json_buffer((std::istreambuf_iterator<char>(file_stream)),
                            std::istreambuf_iterator<char>());

    sfp.set_pipeline_configuration(json_buffer);

    VideoCapture video_stream(1);
    if(!video_stream.isOpened())
        return -1;
    int frame_count = 0;
    for( ; ; )
    {
        Mat frame;
        video_stream >> frame;
        if(frame.data == nullptr)
            break;

        cv::imshow("Simple finder - In frame" , frame);
        if ( frame_count == 30){
            break;
        }

        if ( frame_count % 5 == 0 ){
            Mat out_frame = frame.clone();
            sfp.set_input_image(frame);
            sfp.set_output_image(out_frame);
            sfp.process_pipeline();

            cv::imshow("Simple finder - Out frame" , sfp.get_output_image());

        }
        else{
             cv::imshow("Simple finder - Out frame" , frame);
        }



        if(waitKey(1) == 27)
            break;
        frame_count += 1;
    }

    return 0;
}

int neural_net_finder_example(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cout << "./ipe_example <neural_net_finder_config>\n";
        return 0;
    }

    std::string config_file = argv[1];

    cm::simple_finder_pipeline nnfp;
    std::ifstream file_stream(config_file);
    std::string json_buffer((std::istreambuf_iterator<char>(file_stream)),
                            std::istreambuf_iterator<char>());

    nnfp.set_pipeline_configuration(json_buffer);
    nnfp.set_parameter(
                "yolo_model_loader",
                "model_configuration_file",
                "/home/bala/cm/01_image_processing/00_workspace/yolo_detect/build/test_01/tiny-yolo.cfg");

    nnfp.set_parameter(
                "yolo_model_loader",
                "model_weights_file",
                "/home/bala/cm/01_image_processing/00_workspace/yolo_detect/build/test_01/tiny-yolo.weights");

    nnfp.set_parameter(
                "yolo_model_loader",
                "class_names_file",
                "/home/bala/cm/01_image_processing/00_workspace/yolo_detect/build/test_01/coco.names");

    VideoCapture video_stream(0);
    if(!video_stream.isOpened())
        return -1;

    for( ; ; )
    {
        Mat frame;
        video_stream >> frame;
        if(frame.data == nullptr)
            break;

        Mat out_frame = frame.clone();
        nnfp.set_input_image(frame);
        nnfp.set_output_image(out_frame);
        nnfp.process_pipeline();

        cv::imshow("Neural network finder" , nnfp.get_output_image());
        if(waitKey(1) == 27)
            break;
    }

    return 0;
}

int main(int argc, char **argv)
{
    simple_finder_example(argc, argv);
    return 0;
}

