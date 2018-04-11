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

#ifndef __tracker__block__hpp__
#define __tracker__block__hpp__

#include "process_block.hpp"
#include "cv_image_holder.hpp"

namespace cm {


struct feature_detector_comparator {

    enum image_type
    {
        reference_image,
        test_image
    };

    explicit feature_detector_comparator();

    virtual ~feature_detector_comparator();

    void compute_keypoints(image_type t, const Mat &img);

    void compute_descriptors(image_type t, const Mat &img);

    void compute_matches();

    bool find_matched_bounding_box();

    void set_reference_image(const cv::Mat & ref);

    void set_test_image(const cv::Mat & ref);

    cv::Ptr<cv::FeatureDetector> feature_detector;

    cv::Ptr<cv::DescriptorExtractor> feature_extractor;

    cv::Ptr<cv::DescriptorMatcher> descriptor_matcher;

    float ratio_test_value, ransac_threshold;

    vector<cv::KeyPoint> ref_kp, test_kp, ref_matches, test_matches;

    cv::Mat ref_desc, test_desc;

    vector<vector<cv::DMatch> > ref_test_matches;

    vector<cv::Point2f> ref_bbox, test_bbox;
};


class tracker_pipeline
{

public:

    tracker_pipeline();

    ~tracker_pipeline();

    bool set_input_image(const cv::Mat & in_image);

    bool set_output_image(const cv::Mat & out_image);

    bool process_pipeline();

    bool set_reference_image(const cv::Mat & ref_image);

protected:

    cv_image_holder * input_image, * output_image, * reference_image;

    feature_detector_comparator feature_detector_comparator_obj;
};


}

#endif
