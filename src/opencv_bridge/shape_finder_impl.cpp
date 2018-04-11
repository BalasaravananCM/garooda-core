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

#include "shape_finder.hpp"
#include "../helper_macros.hpp"
#include  "../utilities.hpp"
#include "geometric_transform.hpp"

using namespace std;
using namespace cv;

namespace cm {

namespace lib_bridge {

bool circle_finder_impl(Mat& in,
                        double dp,
                        double min_dist,
                        double canny_param_1,
                        double canny_param_2,
                        int min_radius,
                        int max_radius,
                        bool clear_and_add,
                        computed_objects & objects_found)
{

    try
    {
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(in, circles, CV_HOUGH_GRADIENT,
                         dp, min_dist, canny_param_1, canny_param_2,
                         min_radius, max_radius);

        if(clear_and_add == true)
        {
            objects_found.clear();
        }

        for(auto &c : circles)
        {
            auto cm_circle = cm::get_circle(c);
            auto cm_circle_bbox = cm::get_bounding_rect(cm_circle);
            objects_found.add_result(cm_circle_bbox);
        }

    }
    catch(...)
    {
        return false;
    }

    return true;
}

double angle_between_points(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

bool rectangle_finder_impl(Mat &in,
                           int threshold_step,
                           double shape_appoximator_ratio,
                           double min_contour_area,
                           double max_contour_area,
                           double cosine_angle,
                           bool clear_and_add,
                           computed_objects &rectangles_found)
{
    try
    {
        if(clear_and_add == true)
        {
            rectangles_found.clear();
        }


        int image_area = in.rows * in.cols;
        int min_rect_area = image_area * min_contour_area;
        int max_rect_area = image_area * max_contour_area;

        for( int threshold_itr = k_threshold_min;
             threshold_itr < k_threshold_max ;
             threshold_itr += threshold_step)
        {
            Mat gray_thres;
            threshold(in, gray_thres, threshold_itr, k_threshold_max, THRESH_BINARY);

            vector<vector<cv::Point> > contours;
            findContours(gray_thres, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

            for(int contour_index = 0;
                contour_index != contours.size() ;
                contour_index++)
            {
                double contour_perimeter = arcLength(contours[contour_index], true);

                vector<cv::Point> approx_shape;
                approxPolyDP(contours[contour_index],
                             approx_shape,
                             shape_appoximator_ratio * contour_perimeter,
                             true);

                int shape_size = approx_shape.size();
                if (shape_size != 4)
                    continue;

                double contour_area = contourArea(approx_shape);
                if ( contour_area < min_rect_area || contour_area > max_rect_area )
                    continue;

                vector<double> cosine_angles;
                for (int j = 2; j < shape_size + 1; j++)
                {
                    double cos_angle = angle_between_points (
                                approx_shape [j%shape_size],
                            approx_shape[j-2],
                            approx_shape[j-1]);
                    cosine_angles.push_back(cos_angle);
                }

                sort(cosine_angles.begin(), cosine_angles.end());
                double max_cosine_angle = cosine_angles.back();
                if(max_cosine_angle < cosine_angle)
                {
                    polygon_i rect_contour;
                    rect_contour.perimeter = contour_perimeter;
                    rect_contour.contour = cm::get_cm_contour(approx_shape);
                    rect_contour.centroid = cm::get_cm_point(cm::get_centroid(approx_shape));

                    rectangle_i rct = cm::polygon_i_2_bounding_rect(rect_contour);
                    CM_LOG << "Rectangle = " <<
                                 "( " << rct.origin.x << "," <<
                                 rct.origin.y << "," <<
                                 rct.width << "," <<
                                 rct.height << " )"<< std::endl;

                    rectangles_found.add_result(rct);
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

bool line_finder_impl(Mat &in,
                      double rho,
                      double theta,
                      double threshold,
                      double length_factor,
                      double max_seperation,
                      bool clear_and_add,
                      computed_objects &lines_found)
{
    try
    {
        if(clear_and_add == true)
        {
            lines_found.clear();
        }


        double avg_image_dimension = (in.rows + in.cols) / 2.0;

        std::vector<cv::Vec4i> lines;

        HoughLinesP(in, lines, rho, theta,
                    threshold,
                    avg_image_dimension * length_factor,
                    max_seperation);

        for( size_t i = 0; i < lines.size(); i++ )
        {
            line_i line_item;
            line_item.p1.x = lines[i][0];
            line_item.p1.y = lines[i][1];
            line_item.p2.x = lines[i][2];
            line_item.p2.y = lines[i][3];

            lines_found.add_result(line_item);
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}


bool get_tracker(Mat & image1, Mat & image2, cm::rectangle_i &tracker, int min_good_features, int nfeatures)
{
    try
    {
        tracker.origin.x = 0;
        tracker.origin.y = 0;
        tracker.width = 0;
        tracker.height = 0;
        cv::Ptr<cv::FeatureDetector> ORB = cv::ORB::create(nfeatures);
        std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;
        ORB->detect( image1, keypoints_object );
        ORB->detect( image2, keypoints_scene );
        cv::Mat descriptors_object, descriptors_scene;
        ORB->compute( image1, keypoints_object, descriptors_object );
        ORB->compute( image2, keypoints_scene, descriptors_scene );
        cv::BFMatcher matcher(cv::NORM_HAMMING, true);
        std::vector< cv::DMatch > good_matches;
        matcher.match( descriptors_object, descriptors_scene, good_matches );
        std::vector<cv::Point2f> obj1;
        std::vector<cv::Point2f> obj2;
        if(good_matches.size() >= min_good_features)
        {
            for( int i = 0; i < int(good_matches.size()); i++ )
            {
                obj1.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
                obj2.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
            }
            if ( obj1.size() != 0 && obj2.size() != 0 )
            {
                cv::Mat H = cv::findHomography( obj1, obj2, CV_RANSAC );
                std::vector<cv::Point2f> obj_corners(4);
                obj_corners[0] = cvPoint(0,0);
                obj_corners[1] = cvPoint( image2.cols, 0 );
                obj_corners[2] = cvPoint( image2.cols, image2.rows );
                obj_corners[3] = cvPoint( 0, image2.rows );
                std::vector<cv::Point2f> scene_corners(4);
                cv::perspectiveTransform( obj_corners, scene_corners, H);
                cv::Mat img_matches;
                cv::warpPerspective(image1,
                                    img_matches,
                                    H,
                                    cv::Size(image2.cols,
                                             image2.rows));
                vector<Point2f> dstPoints, srcPoints;
                dstPoints.push_back(Point2f(0,0));
                dstPoints.push_back(Point2f(0,float(image2.rows)));
                dstPoints.push_back(Point2f(float(image2.cols),float(image2.rows)));
                dstPoints.push_back(Point2f(float(image2.cols),0));
                cv::perspectiveTransform(dstPoints,srcPoints,H.inv());
                RotatedRect minRect = minAreaRect(srcPoints);
                cv::Rect rect  = minRect.boundingRect();
                tracker.origin.x = rect.x;
                tracker.origin.y = rect.y;
                tracker.width = rect.width;
                tracker.height = rect.height;
                Rect RectangleToDraw3(tracker.origin.x,tracker.origin.y,tracker.width,tracker.height);
                cv::rectangle(image1, RectangleToDraw3, Scalar(0, 250, 0), 2, 8, 0);
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    catch(...)
    {
        return false;
    }
}

void drawBoundingBox(Mat image, vector<Point2f> bb)
{
    for(unsigned i = 0; i < bb.size() - 1; i++) {
        cv::line(image, bb[i], bb[i + 1], Scalar(0, 0, 255), 2);
    }
    cv::line(image, bb[bb.size() - 1], bb[0], Scalar(0, 0, 255), 2);
}

}

}
