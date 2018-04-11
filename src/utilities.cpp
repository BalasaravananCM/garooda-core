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

#include "utilities.hpp"


std::string cm::extract_function_name(std::string in_string)
{
    return in_string.substr(3);
}


cm::circle_i cm::get_circle(cv::Vec3f circle_vec)
{
    circle_i circle;
    circle.center.x = cvRound(circle_vec[0]);
    circle.center.y = cvRound(circle_vec[1]);
    circle.radius = cvRound(circle_vec[2]);
    return circle;
}


cm::point_i cm::get_cm_point(cv::Point in_pt)
{
    cm::point_i out_pt;
    out_pt.x = in_pt.x;
    out_pt.y = in_pt.y;
    return out_pt;
}


cv::Point cm::get_cv_point(cm::point_i in_pt)
{
    return cv::Point(in_pt.x, in_pt.y);
}

// Draw only the 2D points
void draw2DPoints(cv::Mat image, std::vector<cv::Point2f> &list_points, cv::Scalar color)
{
  for( size_t i = 0; i < list_points.size(); i++)
  {
    cv::Point2f point_2d = list_points[i];

    // Draw Selected points
    cv::circle(image, point_2d, 3, color, -1, 0 );
  }
}


std::vector<cm::point_i> cm::get_cm_contour(std::vector<cv::Point> contour_points)
{
    std::vector<cm::point_i> cm_contour;
    for(auto &point : contour_points)
    {
        cm_contour.push_back(get_cm_point(point));
    }
    return cm_contour;
}


std::vector<cv::Point> cm::get_cv_contour(std::vector<cm::point_i> contour_points)
{
    std::vector<cv::Point> cv_contour;
    for(auto &point : contour_points)
    {
        cv::Point pt = cm::get_cv_point(point);
        cv_contour.push_back(pt);
    }
    return cv_contour;
}

cv::Point cm::get_centroid(std::vector<cv::Point> contour)
{
    cv::Point centroid;
    cv::Moments rect_moments = cv::moments(contour);
    centroid.x = (int) ( rect_moments.m10 / rect_moments.m00);
    centroid.y = (int) ( rect_moments.m01 / rect_moments.m00);
    return centroid;
}

cm::rectangle_i cm::get_bounding_rect(cm::circle_i in)
{
    cm::rectangle_i out;

    out.origin.x = in.center.x - in.radius;
    out.origin.y = in.center.y - in.radius;
    out.width = out.height = in.radius * 2;

    return out;
}

double cm::get_length(line_i l)
{
    double dx1 = l.p1.x - l.p2.x;
    double dy1 = l.p1.y - l.p2.y;

    return std::sqrt( ( dx1 * dx1 ) + ( dy1 + dy1) );
}


cm::rectangle_i  cm::rect_2_bounding_rect(Rect2d v)
{
    cm::rectangle_i v_rectangle_i;

    v_rectangle_i.origin.x = v.x;
    v_rectangle_i.origin.y = v.y;
    v_rectangle_i.width = v.width;
    v_rectangle_i.height = v.height;

    return v_rectangle_i;
}

cm::rectangle_i cm::polygon_i_2_bounding_rect(polygon_i rect_contour)
{

    std::vector<cm::point_i> a = rect_contour.contour;
    vector<cv::Point2d> v1d=cm::vector_Point_i_2_vector_Point2d(a);
    vector<cv::Point> v1(v1d.begin(), v1d.end());
    Rect2d rect = boundingRect(v1);
    cm::rectangle_i  v = rect_2_bounding_rect(rect);

    return v;
}

cv::Rect cm::get_cv_rect(const cm::rectangle_i & cm_rect)
{
    cv::Rect cv_rect;
    cv_rect.x = cm_rect.origin.x;
    cv_rect.y = cm_rect.origin.y;
    cv_rect.width = cm_rect.width;
    cv_rect.height = cm_rect.height;

    return cv_rect;
}

vector<cm::rectangle_i> cm::vector_polygon_i_2_vector_bounding_rect(std::vector<polygon_i> found_rectangles)
{
    vector<cm::rectangle_i> rectangle_v;

    for(int rct = 0; rct<found_rectangles.size(); rct++){
        cm::rectangle_i  v = polygon_i_2_bounding_rect(found_rectangles[rct]);
        rectangle_v.push_back(v);
    }

    return rectangle_v;
}

vector<cm::rectangle_i> cm::get_vector_bounding_rect(vector<cm::circle_i> in)
{
    vector<cm::rectangle_i> out;
    cm::rectangle_i out1;

    for(auto & c : in){

        out1= cm::get_bounding_rect(c);
        out.push_back(out1);
    }
    return out;
}

cm::rectangle_i cm::bounding_rect_shifting(cm::rectangle_i v, int shift, int shift_value)
{
    cm::rectangle_i rc;

    /* 1 for "UP" , 2 for "DOWN" , 3 for "LEFT" , 4 for "RIGHT" ,*/
    const int str_up = 1;
    const int str_down = 2;
    const int str_left = 3;
    const int str_right = 4;

    switch(shift) {
    case str_up  :
        rc.origin.x = v.origin.x;
        rc.origin.y = v.origin.y-shift_value;
        rc.width = v.width;
        rc.height = v.height;
        CM_LOG<<"   UP shift"<<std::endl;
        break; //optional
    case str_down  :
        rc.origin.x = v.origin.x;
        rc.origin.y = v.origin.y+shift_value;
        rc.width = v.width;
        rc.height = v.height;
        CM_LOG<<"   DOWN shift"<<std::endl;
        break; //optional
    case str_left  :
        rc.origin.x = v.origin.x-shift_value;
        rc.origin.y = v.origin.y;
        rc.width = v.width;
        rc.height = v.height;
        CM_LOG<<"   LEFT shift"<<std::endl;
        break; //optional
    case str_right  :
        rc.origin.x = v.origin.x+shift_value;
        rc.origin.y = v.origin.y;
        rc.width = v.width;
        rc.height = v.height;
        CM_LOG<<"   RIGTH shift"<<std::endl;
        break; //optional
    default :
        CM_LOG<<"   shift is not posible"<<std::endl;
    }
    return rc;
}

cm::polygon_i cm::vector_point_2_polygon_i(vector<cv::Point> vd, double shape_appoximator_ratio)
{
    cm::polygon_i rect_contour;
    double contour_perimeter = arcLength(vd, true);
    vector<cv::Point> approx_shape;
    approxPolyDP(vd,
                 approx_shape,
                 shape_appoximator_ratio * contour_perimeter,
                 true);
    rect_contour.perimeter = contour_perimeter;
    rect_contour.contour = cm::get_cm_contour(approx_shape);
    rect_contour.centroid = cm::get_cm_point(cm::get_centroid(approx_shape));
    return rect_contour;
}

vector<Point2d> cm::vector_Point_i_2_vector_Point2d(std::vector<cm::point_i> a){

    vector<Point2d> vd;

    vd = { {double(a[0].x), double(a[0].y)},
           {double(a[1].x), double(a[1].y)},
           {double(a[2].x), double(a[2].y)},
           {double(a[3].x), double(a[3].y)} };
    return vd;
}

std::vector<cv::Point> cm::bounding_rect_2_vector_point(cm::rectangle_i rc){

    std::vector<cv::Point> vd;

    vd = { { int(rc.origin.x), int(rc.origin.y) },
           { int(rc.origin.x+rc.width), int(rc.origin.y)},
           { int(rc.origin.x+rc.width), int(rc.origin.y+rc.height)},
           { int(rc.origin.x), int(rc.origin.y+rc.height) } };
    return vd;
}

std::pair<int, int> cm::find_histogram(int bins, vector<cm::rectangle_i> check)
{
    std::pair<int, int> bin_threshold;
    Mat group_radius_;
    vector<float> group_radius;

    CM_LOG<<"check size "<<check.size()<<std::endl;
    for(cm::rectangle_i & c : check)
    {
        group_radius_.push_back(float(c.width*c.height));
        group_radius.push_back(float(c.width*c.height));
    }
    CM_LOG<<group_radius.size()<<"group_radius.size()"<<group_radius_.size()<<std::endl;


    //min and max area
    float min_value = *std::min_element(group_radius.begin(),group_radius.end());
    float max_value = *std::max_element(group_radius.begin(),group_radius.end());
    CM_LOG<<"min area "<<min_value <<" max area "<<max_value<<std::endl;

    int bin_size = int((max_value - min_value)/bins);
    CM_LOG<<"bin size :"<<bin_size<<std::endl;

    int histSize[] = {bins};

    // Set ranges for histogram bins
    float lranges[] = {min_value, max_value};
    const float* ranges[] = {lranges};

    // create matrix for histogram
    cv::Mat hist;
    int channels[] = {0};

    //calculate hist
    cv::calcHist(&group_radius_, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);

    CM_LOG<<hist<<std::endl;

    double min_val, max_val;
    cv::minMaxLoc(hist, &min_val, &max_val);
    CM_LOG<<"max_hist_value "<<max_val<<std::endl;


    CM_LOG<<hist.size()<<std::endl;

    int minArea, maxArea;
    for (int j=0; j < hist.rows; j++){
        if(max_val==hist.at<float>(0,j)){

            minArea =  min_value+(j*bin_size);
            maxArea =  minArea+bin_size;
        }
    }

    bin_threshold = std::make_pair(minArea, maxArea);

    return bin_threshold;
}

vector<std::pair<int, int>> cm::find_histogram_based_area_groups(int bins, vector<cm::rectangle_i> check)
{
    vector<std::pair<int, int>> bins_threshold;
    Mat group_radius_;
    vector<float> group_radius;

    CM_LOG<<"check size "<<check.size()<<std::endl;
    for(cm::rectangle_i & c : check)
    {
        group_radius_.push_back(float(c.width*c.height));
        group_radius.push_back(float(c.width*c.height));
    }
    CM_LOG<<group_radius.size()<<"group_radius.size()"<<group_radius_.size()<<std::endl;


    //min and max area
    float min_value = *std::min_element(group_radius.begin(),group_radius.end());
    float max_value = *std::max_element(group_radius.begin(),group_radius.end());
    CM_LOG<<"min area "<<min_value <<" max area "<<max_value<<std::endl;

    int bin_size = int((max_value - min_value)/bins);
    CM_LOG<<"bin size :"<<bin_size<<std::endl;

    int histSize[] = {bins};

    // Set ranges for histogram bins
    float lranges[] = {min_value, max_value};
    const float* ranges[] = {lranges};

    // create matrix for histogram
    cv::Mat hist;
    int channels[] = {0};

    cv::calcHist(&group_radius_, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);

    CM_LOG<<hist<<std::endl;

    double min_val, max_val;
    cv::minMaxLoc(hist, &min_val, &max_val);
    CM_LOG<<"max_hist_value "<<max_val<<std::endl;

    CM_LOG<<hist.size()<<std::endl;

    int minArea, maxArea;
    for (int j=0; j < hist.rows; j++){

        minArea =  min_value+(j*bin_size);
        maxArea =  minArea+bin_size;

        std::pair<int, int> bin_threshold;
        bin_threshold=std::make_pair(minArea, maxArea);

        bins_threshold.push_back(bin_threshold);


    }

    CM_LOG<<"Number of bins : "<<bins_threshold.size()<<std::endl;



    return bins_threshold;
}

int cm::overlapping_area(cm::rectangle_i v, cm::rectangle_i vv){

    int xa1, ya1, xa2, ya2;
    int xb1, yb1, xb2, yb2;

    xa1 = v.origin.x;
    ya1 = v.origin.y;
    xa2 = v.origin.x+v.width;
    ya2 =v.origin.y+v.height;

    xb1 = vv.origin.x;
    yb1 = vv.origin.y;
    xb2 = vv.origin.x+vv.width;
    yb2 =vv.origin.y+vv.height;

    int iLeft = max(xa1, xb1);
    int iRight = min(xa2, xb2);
    int iTop = max(ya1, yb1);
    int iBottom = min(ya2, yb2);

    int area =max(0, iRight - iLeft) * max(0, iBottom - iTop);

    return area;

}

float cm::get_eucliedean_distance(Point2f a, Point2f b)
{
    float sq_dist = std::pow( ( a.x - b.x ) , 2.0) + std::pow( ( a.y - b.y ) , 2.0);
    return std::sqrt(sq_dist);
}


double cm::otsu_threshold(Mat in)
{
    Mat k;
    double otsu_thresh;

    if(in.type()==0)
    {
        otsu_thresh = cv::threshold(in, k, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        CM_LOG<<"otsu threshold : "<<otsu_thresh<<std::endl;
    }
    else
    {
        CM_LOG<<"image type is not CV_8U "<<std::endl;
        otsu_thresh = 0.0;
    }

    return otsu_thresh;
}

float cm::distance_btw_point_i(cm::point_i p, cm::point_i q)
{
    Point diff;
    diff.x= abs(p.x - q.x);
    diff.y= abs(p.y - q.y);
    return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}



Mat cm::translate_image(Mat img, int offset_x, int offset_y)
{
    Mat dest = img.clone();
    Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offset_x, 0, 1, offset_y);
    cv::warpAffine(img, dest, trans_mat, img.size());
    return dest;
}

Mat cm::rotate_image(Mat img, int rotation_angle)
{
    Mat dest = img.clone();
    Mat rotn_mat = cv::getRotationMatrix2D(cv::Point2f(img.rows / 2, img.cols / 2) ,
                                           rotation_angle, 1);
    cv::warpAffine(img, dest, rotn_mat, img.size());
    return dest;
}

Mat cm::scale_image(Mat img, int increase_percentage)
{
    Mat dest = img.clone();
    double dt = ( 100 + increase_percentage ) / 100.0;
    int resize_type = INTER_LINEAR;
    if(increase_percentage < 0)
        resize_type = INTER_AREA;
    cv::resize(img, dest, Size(), dt, dt, resize_type);
    return dest;
}

vector<Mat> cm::get_geometric_tranforms(Mat in)
{
    vector<Mat> images;
    images.push_back(in);

    images.push_back(rotate_image(in, 3));
    images.push_back(rotate_image(in, 7));
    images.push_back(rotate_image(in, -3));
    images.push_back(rotate_image(in, -7));

    images.push_back(scale_image(in, 5));
    images.push_back(scale_image(in, -5));

    return images;
}

vector<Rect> cm::extract_faces(Mat in, string cascade_file){
    vector<Rect> faces;
    CascadeClassifier cascade_classifier = CascadeClassifier(cascade_file);
    Mat gray;
    cvtColor(in, gray, COLOR_BGR2GRAY);
    cascade_classifier.detectMultiScale(gray, faces);
    return faces;

}
