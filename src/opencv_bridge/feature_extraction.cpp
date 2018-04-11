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

#include "feature_extraction.hpp"
#include "../helper_macros.hpp"
#include "../utilities.hpp"


using namespace std;
using namespace cv;

namespace cm {

namespace lib_bridge {

bool sift_feature_detection( Mat & in, Mat & out,
                             const algo_pipeline_params & params,
                             computed_objects & result_obj )
{
    int nfeatures = 0;
    int nOctaveLayers = 3;
    double contrastThreshold = 0.04;
    double edgeThreshold = 10;
    double sigma = 1.6;

    try
    {
        read_int_param("sift_feature_detection", nfeatures);
        read_int_param("sift_feature_detection", nOctaveLayers);
        read_double_param("sift_feature_detection", contrastThreshold);
        read_double_param("sift_feature_detection", edgeThreshold);
        read_double_param("sift_feature_detection", sigma);

        cv::Ptr<cv::Feature2D> detector = cv::xfeatures2d::SIFT::create(nfeatures,
                                                                        nOctaveLayers,
                                                                        contrastThreshold,
                                                                        edgeThreshold,
                                                                        sigma);

        std::vector<KeyPoint> keypoints_1;

        detector->detect( in, keypoints_1 );

        for(auto keypoint : keypoints_1){
            result_obj.add_result(keypoint);
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool sift_descriptor_extraction( Mat & in, Mat & out,
                                 const algo_pipeline_params & params,
                                 computed_objects & result_obj )
{
    int nfeatures = 0;
    int nOctaveLayers = 3;
    double contrastThreshold = 0.04;
    double edgeThreshold = 10;
    double sigma = 1.6;

    try
    {
        read_int_param("sift_descriptor_extraction", nfeatures);
        read_int_param("sift_descriptor_extraction", nOctaveLayers);
        read_double_param("sift_descriptor_extraction", contrastThreshold);
        read_double_param("sift_descriptor_extraction", edgeThreshold);
        read_double_param("sift_descriptor_extraction", sigma);

        auto keypoints_1 = result_obj.get_keypoints();

        cv::Ptr<cv::Feature2D> extractor = cv::xfeatures2d::SIFT::create(nfeatures,
                                                                         nOctaveLayers,
                                                                         contrastThreshold,
                                                                         edgeThreshold,
                                                                         sigma);


        cv::Mat descriptors_object;
        extractor->compute( in, keypoints_1, descriptors_object );

        result_obj.add_result(descriptors_object);
    }
    catch(...)
    {
        return false;
    }

    return true;
}


bool surf_feature_detection( Mat & in, Mat & out,
                             const algo_pipeline_params & params,
                             computed_objects & result_obj )
{
    int nOctaves = 4;
    int nOctaveLayers = 3;
    double hessianThreshold=100;
    bool extended = false;
    bool upright = false;

    try
    {
        read_int_param("surf_feature_detection", nOctaves);
        read_int_param("surf_feature_detection", nOctaveLayers);
        read_double_param("surf_feature_detection", hessianThreshold);
        read_bool_param("surf_feature_detection", extended);
        read_bool_param("surf_feature_detection", upright);

        cv::Ptr<cv::Feature2D> detector = cv::xfeatures2d::SURF::create(hessianThreshold,
                                                                        nOctaves,
                                                                        nOctaveLayers,
                                                                        extended,
                                                                        upright);

        std::vector<KeyPoint> keypoints_1;

        detector->detect( in, keypoints_1 );

        for(auto keypoint : keypoints_1){

            result_obj.add_result(keypoint);
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool surf_descriptor_extraction( Mat & in, Mat & out,
                                 const algo_pipeline_params & params,
                                 computed_objects & result_obj )
{
    int nOctaves = 4;
    int nOctaveLayers = 3;
    double hessianThreshold=100;
    bool extended = false;
    bool upright = false;

    try
    {
        read_int_param("surf_descriptor_extraction", nOctaves);
        read_int_param("surf_descriptor_extraction", nOctaveLayers);
        read_double_param("surf_descriptor_extraction", hessianThreshold);
        read_bool_param("surf_descriptor_extraction", extended);
        read_bool_param("surf_descriptor_extraction", upright);

        auto keypoints_1 = result_obj.get_keypoints();

        cv::Ptr<cv::Feature2D> extractor = cv::xfeatures2d::SURF::create(hessianThreshold,
                                                                         nOctaves,
                                                                         nOctaveLayers,
                                                                         extended,
                                                                         upright);

        cv::Mat descriptors_object;
        extractor->compute( in, keypoints_1, descriptors_object );

        result_obj.add_result(descriptors_object);
    }
    catch(...)
    {
        return false;
    }

    return true;
}


bool fast_feature_detection( Mat & in, Mat & out,
                             const algo_pipeline_params & params,
                             computed_objects & result_obj )
{
    int threshold=10;
    int type=FastFeatureDetector::TYPE_9_16;
    bool nonmaxSuppression=true;

    try
    {
        read_int_param("fast_feature_detection", threshold);
        read_int_param("fast_feature_detection", type);
        read_bool_param("fast_feature_detection", nonmaxSuppression);

        Ptr<FastFeatureDetector> detector = FastFeatureDetector::create(threshold, nonmaxSuppression, type);

        std::vector<KeyPoint> keypoints_1;

        detector->detect( in, keypoints_1 );

        for(auto keypoint : keypoints_1){
            result_obj.add_result(keypoint);
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool orb_feature_detection( Mat & in, Mat & out,
                            const algo_pipeline_params & params,
                            computed_objects & result_obj )
{
    int nfeatures=500;
    int nlevels=8;
    int edgeThreshold=31;
    int firstLevel=0;
    int wta_k=2;
    int scoreType=ORB::HARRIS_SCORE;
    int patchSize=31;
    int fastThreshold=20;
    double scaleFactor=1.2f;

    try
    {
        read_int_param("orb_feature_detection", nfeatures);
        read_int_param("orb_feature_detection", nlevels);
        read_int_param("orb_feature_detection", edgeThreshold);
        read_int_param("orb_feature_detection", firstLevel);
        read_int_param("orb_feature_detection", wta_k);
        read_int_param("orb_feature_detection", scoreType);
        read_int_param("orb_feature_detection", patchSize);
        read_int_param("orb_feature_detection", fastThreshold);
        read_double_param("orb_feature_detection", scaleFactor);

        cv::Ptr<cv::FeatureDetector> ORB = cv::ORB::create(nfeatures,
                                                           scaleFactor,
                                                           nlevels,
                                                           edgeThreshold,
                                                           firstLevel,
                                                           wta_k,
                                                           scoreType,
                                                           patchSize,
                                                           fastThreshold);
        std::vector<cv::KeyPoint> keypoints_1;

        ORB->detect( in, keypoints_1 );

        for(auto keypoint : keypoints_1){
            result_obj.add_result(keypoint);
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool orb_descriptor_extraction( Mat & in, Mat & out,
                                const algo_pipeline_params & params,
                                computed_objects & result_obj )
{
    int nfeatures=500;
    int nlevels=8;
    int edgeThreshold=31;
    int firstLevel=0;
    int WTA_K=2;
    int scoreType=ORB::HARRIS_SCORE;
    int patchSize=31;
    int fastThreshold=20;
    double scaleFactor=1.2f;
    try
    {
        read_int_param("orb_descriptor_extraction", nfeatures);
        read_int_param("orb_descriptor_extraction", nlevels);
        read_int_param("orb_descriptor_extraction", edgeThreshold);
        read_int_param("orb_descriptor_extraction", firstLevel);
        read_int_param("orb_descriptor_extraction", WTA_K);
        read_int_param("orb_descriptor_extraction", scoreType);
        read_int_param("orb_descriptor_extraction", patchSize);
        read_int_param("orb_descriptor_extraction", fastThreshold);
        read_double_param("orb_descriptor_extraction", scaleFactor);

        auto keypoints_1 = result_obj.get_keypoints();

        cv::Ptr<cv::FeatureDetector> extractor = cv::ORB::create(nfeatures,
                                                                 scaleFactor,
                                                                 nlevels,
                                                                 edgeThreshold,
                                                                 firstLevel,
                                                                 WTA_K,
                                                                 scoreType,
                                                                 patchSize,
                                                                 fastThreshold);
        cv::Mat descriptors_object;
        extractor->compute( in, keypoints_1, descriptors_object );

        result_obj.add_result(descriptors_object);
    }
    catch(...)
    {
        return false;
    }

    return true;
}


bool brief_descriptor_extraction( Mat & in, Mat & out,
                                  const algo_pipeline_params & params,
                                  computed_objects & result_obj )
{
    int bytes = 32;
    bool use_orientation = false;

    try
    {
        read_int_param("brief_descriptor_extraction", bytes);
        read_bool_param("brief_descriptor_extraction", use_orientation);

        auto keypoints_1 = result_obj.get_keypoints();

        cv::Ptr<cv::Feature2D> extractor;
        extractor = cv::xfeatures2d::BriefDescriptorExtractor::create(bytes, use_orientation);

        cv::Mat descriptors_object;
        extractor->compute( in, keypoints_1, descriptors_object );

        result_obj.add_result(descriptors_object);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool harris_corner_detection( Mat & in, Mat & out,
                              const algo_pipeline_params & params,
                              computed_objects & result_obj )
{
    int blockSize=2;
    int ksize = 3;
    int borderType = BORDER_DEFAULT;
    int norm_type = NORM_MINMAX;
    int dtype = CV_32FC1;
    int alpha = 0;
    int beta = 255;
    int thresh = 200;
    double k = 0.04;

    try
    {
        read_int_param("harris_corner_detection", blockSize);
        read_int_param("harris_corner_detection", ksize);
        read_int_param("harris_corner_detection", borderType);
        read_int_param("harris_corner_detection", norm_type);
        read_int_param("harris_corner_detection", dtype);
        read_int_param("harris_corner_detection", alpha);
        read_int_param("harris_corner_detection", beta)
        read_int_param("harris_corner_detection", thresh);
        read_double_param("harris_corner_detection", k);

        Mat dst, dst_norm, dst_norm_scaled;

        dst = Mat::zeros( in.size(), CV_32FC1 );

        cornerHarris( in, dst, blockSize, ksize, k, borderType );

        normalize( dst, dst_norm, alpha, beta, norm_type, dtype, Mat() );

        convertScaleAbs( dst_norm, dst_norm_scaled );

        for( int j = 0; j < dst_norm.rows ; j++ )
        { for( int i = 0; i < dst_norm.cols; i++ )
            {
                if( dst_norm.at<float>(j,i) > thresh )
                {
                    result_obj.add_result(Point2f(i,j));
                }
            }
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool shi_tomasi_corner_detection( Mat & in, Mat & out,
                                  const algo_pipeline_params & params,
                                  computed_objects & result_obj )
{
    int maxCorners = 23;
    int blockSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;
    double qualityLevel = 0.01;
    double minDistance = 10;

    try
    {
        read_int_param("shi_tomasi_corner_detection", maxCorners);
        read_int_param("shi_tomasi_corner_detection", blockSize);
        read_bool_param("shi_tomasi_corner_detection", useHarrisDetector);
        read_double_param("shi_tomasi_corner_detection", k);
        read_double_param("shi_tomasi_corner_detection", qualityLevel);
        read_double_param("shi_tomasi_corner_detection", minDistance);

        vector<Point2f> corners;

        goodFeaturesToTrack( in,
                             corners,
                             maxCorners,
                             qualityLevel,
                             minDistance,
                             Mat(),
                             blockSize,
                             useHarrisDetector,
                             k );

        for(auto corner : corners){
            result_obj.add_result(corner);
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
