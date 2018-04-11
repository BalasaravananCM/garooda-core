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

#include "../helper_macros.hpp"
#include  "../utilities.hpp"

#include "aruco_extraction.hpp"

namespace cm {

namespace lib_bridge {

bool aruco_finder(cv::Mat &in, cv::Mat &out,
                  const cm::algo_pipeline_params &params,
                  cm::computed_objects &result_obj)
{
    try{

        result_obj.clear();

        bool detect_pose ;
        read_bool_param("aruco_finder", detect_pose);

        Ptr<aruco::Dictionary> dictionary  = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(16));
        aruco::detectMarkers(in, dictionary, result_obj.aruco_obj.aruco_res.aruco_corner_points,
                             result_obj.aruco_obj.aruco_res.ids,
                             result_obj.aruco_obj.params,
                             result_obj.aruco_obj.aruco_res.aruco_corner_points);


        if(result_obj.aruco_obj.aruco_res.ids.size() > 0 && detect_pose) {

            aruco::estimatePoseSingleMarkers(result_obj.aruco_obj.aruco_res.aruco_corner_points,
                                             1.0 * 0.5f,
                                             result_obj.aruco_obj.camMatrix,
                                             result_obj.aruco_obj.distCoeffs,
                                             result_obj.aruco_obj.aruco_res.rvec,
                                             result_obj.aruco_obj.aruco_res.tvec);
        }


    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool aruco_loader(cv::Mat &in, cv::Mat &out,
                  const cm::algo_pipeline_params &params,
                  cm::computed_objects &result_obj)
{
    try{


        string camera_params ;
        read_string_param("aruco_loader", camera_params);

        string aruco_params ;
        read_string_param("aruco_loader", aruco_params);

        result_obj.aruco_obj.params = aruco::DetectorParameters::create();

        if(result_obj.aruco_obj.camMatrix.empty() ||
                result_obj.aruco_obj.distCoeffs.empty()) {

            bool read_camera = read_camera_parameters(camera_params, result_obj.aruco_obj.camMatrix, result_obj.aruco_obj.distCoeffs);
            if(!read_camera) {
                cerr << "Invalid camera file" << endl;
                return 0;
            }

            bool read_aruco_params = read_detector_parameters(camera_params ,  result_obj.aruco_obj.params );
            if(!read_aruco_params) {
                cerr << "Invalid Detector file" << endl;
                return 0;
            }
        }

    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool aruco_draw(cv::Mat &in, cv::Mat &out,
                const cm::algo_pipeline_params &params,
                cm::computed_objects &result_obj)
{
    bool detect_pose ;
    read_bool_param("aruco_finder", detect_pose);

    aruco::drawDetectedMarkers(out, result_obj.aruco_obj.aruco_res.aruco_corner_points,
                               result_obj.aruco_obj.aruco_res.ids);

    if(detect_pose){
        for(unsigned int i = 0; i < result_obj.aruco_obj.aruco_res.ids.size(); i++){
            aruco::drawAxis(out,  result_obj.aruco_obj.camMatrix,  result_obj.aruco_obj.distCoeffs,
                            result_obj.aruco_obj.aruco_res.rvec, result_obj.aruco_obj.aruco_res.tvec,
                            1.0 * 0.5f);
        }
    }
}


bool read_camera_parameters(string filename, Mat &camMatrix, Mat &distCoeffs)
{
    FileStorage fs(filename, FileStorage::READ);
    if(!fs.isOpened())
        return false;
    fs["camera_matrix"] >> camMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    return true;
}

bool read_detector_parameters(string filename , Ptr<aruco::DetectorParameters>&params )
{

    if(filename.empty()){
        cout<<"Detactor parameters file is empty . Using Default values."<<endl;
        return false;
    }

    FileStorage fs(filename, FileStorage::READ);
    if(!fs.isOpened())
        return false;
    fs["adaptiveThreshWinSizeMin"] >> params->adaptiveThreshWinSizeMin;
    fs["adaptiveThreshWinSizeMax"] >> params->adaptiveThreshWinSizeMax;
    fs["adaptiveThreshWinSizeStep"] >> params->adaptiveThreshWinSizeStep;
    fs["adaptiveThreshConstant"] >> params->adaptiveThreshConstant;
    fs["minMarkerPerimeterRate"] >> params->minMarkerPerimeterRate;
    fs["maxMarkerPerimeterRate"] >>params->maxMarkerPerimeterRate;
    fs["polygonalApproxAccuracyRate"] >> params->polygonalApproxAccuracyRate;
    fs["minCornerDistanceRate"] >> params->minCornerDistanceRate;
    fs["minDistanceToBorder"] >> params->minDistanceToBorder;
    fs["minMarkerDistanceRate"] >> params->minMarkerDistanceRate;
    fs["cornerRefinementMethod"] >>params->cornerRefinementMethod;
    fs["cornerRefinementWinSize"] >> params->cornerRefinementWinSize;
    fs["cornerRefinementMaxIterations"] >> params->cornerRefinementMaxIterations;
    fs["cornerRefinementMinAccuracy"] >> params->cornerRefinementMinAccuracy;
    fs["markerBorderBits"] >> params->markerBorderBits;
    fs["perspectiveRemovePixelPerCell"] >> params->perspectiveRemovePixelPerCell;
    fs["perspectiveRemoveIgnoredMarginPerCell"] >> params->perspectiveRemoveIgnoredMarginPerCell;
    fs["maxErroneousBitsInBorderRate"] >> params->maxErroneousBitsInBorderRate;
    fs["minOtsuStdDev"] >> params->minOtsuStdDev;
    fs["errorCorrectionRate"] >>params->errorCorrectionRate;

    return true;
}


}

}
