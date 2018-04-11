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

#include "tracker_pipeline.hpp"
#include "opencv_bridge/shape_finder_impl.hpp"

cm::feature_detector_comparator::feature_detector_comparator() :
    ratio_test_value(0.8f), ransac_threshold(2.5f)
{
    feature_detector = cv::ORB::create(500, 1.5, 4);
    feature_extractor = cv::ORB::create(500, 1.5, 4);
    descriptor_matcher = cv::makePtr<cv::BFMatcher>((int)cv::NORM_HAMMING, false);
}

cm::feature_detector_comparator::~feature_detector_comparator()
{
}

void cm::feature_detector_comparator::compute_keypoints(image_type t, const Mat &img)
{
    if(t == reference_image)
    {
        feature_detector->detect(img, ref_kp);
    }

    if(t == test_image)
    {
        feature_detector->detect(img, test_kp);
    }
}

void cm::feature_detector_comparator::compute_descriptors(image_type t, const Mat &img)
{
    if(t == reference_image)
    {
        feature_extractor->compute(img, ref_kp, ref_desc);
    }

    if(t == test_image)
    {
        feature_extractor->compute(img, test_kp, test_desc);
    }
}

void cm::feature_detector_comparator::compute_matches()
{
    descriptor_matcher->knnMatch(ref_desc, test_desc, ref_test_matches, 2);

    for(unsigned i = 0; i < ref_test_matches.size(); i++)
    {
        if(ref_test_matches[i][0].distance < ratio_test_value * ref_test_matches[i][1].distance)
        {
            ref_matches.push_back(ref_kp[ref_test_matches[i][0].queryIdx]);
            test_matches.push_back(test_kp[ref_test_matches[i][0].trainIdx]);
        }
    }
}

bool cm::feature_detector_comparator::find_matched_bounding_box()
{
    Mat inlier_mask, homography;
    vector<KeyPoint> inliers1, inliers2;
    vector<DMatch> inlier_matches;

    vector<cv::Point2f> ref_match_points, test_match_points;
    KeyPoint::convert(ref_matches, ref_match_points);
    KeyPoint::convert(test_matches, test_match_points);

    if(ref_matches.size() >= 4) {
        homography = findHomography(ref_match_points, test_match_points,
                                    RANSAC, ransac_threshold, inlier_mask);
    }

    if(ref_matches.size() < 4 || homography.empty()) {
        return false;
    }

    for(unsigned i = 0; i < ref_matches.size(); i++) {
        if(inlier_mask.at<uchar>(i)) {
            int new_i = static_cast<int>(inliers1.size());
            inliers1.push_back(ref_matches[i]);
            inliers2.push_back(test_matches[i]);
            inlier_matches.push_back(DMatch(new_i, new_i, 0));
        }
    }

    perspectiveTransform(ref_bbox, test_bbox, homography);
    return true;
}

void cm::feature_detector_comparator::set_reference_image(const cv::Mat & ref)
{
    compute_keypoints(reference_image, ref);

    compute_descriptors(reference_image, ref);

    ref_bbox.push_back(cv::Point(0,0));
    ref_bbox.push_back(cv::Point(ref.cols, 0));
    ref_bbox.push_back(cv::Point(ref.cols, ref.rows));
    ref_bbox.push_back(cv::Point(0,ref.rows));
}

void cm::feature_detector_comparator::set_test_image(const cv::Mat & test)
{
    test_kp.clear();
    ref_matches.clear();
    test_matches.clear();
    ref_test_matches.clear();

    compute_keypoints(test_image, test);

    compute_descriptors(test_image, test);
}

cm::tracker_pipeline::tracker_pipeline()
{
    input_image = output_image = reference_image = nullptr;
}

cm::tracker_pipeline::~tracker_pipeline()
{
    delete input_image;

    delete output_image;
}

bool cm::tracker_pipeline::set_input_image(const cv::Mat & in_image)
{
    if(input_image != nullptr)
        delete input_image;

    input_image = new cm::cv_image_holder(in_image);

    return input_image->is_valid();
}

bool cm::tracker_pipeline::set_output_image(const cv::Mat & out_image)
{
    if(output_image != nullptr)
        delete output_image;

    output_image = new cm::cv_image_holder(out_image);

    return output_image->is_valid();
}

bool cm::tracker_pipeline::set_reference_image(const cv::Mat & ref_image)
{
    if(reference_image != nullptr)
        delete reference_image;

    reference_image = new cm::cv_image_holder(ref_image);

    feature_detector_comparator_obj.set_reference_image(ref_image);

    return true;
}

bool cm::tracker_pipeline::process_pipeline()
{
    if(input_image == nullptr)
        return false;

    cv::Mat input_matrix = input_image->get_cv_matrix();
    cv::Mat output_matrix = output_image->get_cv_matrix();

    if(input_matrix.data == nullptr)
        return false;

    cm::lib_bridge::time_counter tc;
    feature_detector_comparator_obj.set_test_image(input_matrix);
    feature_detector_comparator_obj.compute_matches();
    feature_detector_comparator_obj.find_matched_bounding_box();

    cm::lib_bridge::drawBoundingBox(output_matrix, feature_detector_comparator_obj.test_bbox);

    return true;
}

