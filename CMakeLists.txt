cmake_minimum_required (VERSION 2.6)

set (CMAKE_CXX_STANDARD 11)

project (ipe_cpp)

set(OpenCV_DIR "/home/bala/cm/01_image_processing/opencv_3_4_1/dist_x64/share/OpenCV")

set(dlib_DIR "/home/bala/cm/03_ml_libs/dlib/dlib_19_8/dist_x64/lib/cmake/dlib")

find_package(OpenCV REQUIRED COMPONENTS core imgproc dnn features2d tracking calib3d xfeatures2d objdetect aruco)

find_package(dlib)

message(STATUS "OpenCV version : ${OpenCV_VERSION}")
message(STATUS "OpenCV includes : ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libs : ${OpenCV_LIBS}")

message(STATUS "Dlib version : ${dlib_VERSION}")
message(STATUS "Dlib includes : ${dlib_INCLUDE_DIRS}")
message(STATUS "Dlib libs : ${dlib_LIBRARIES}")

add_subdirectory(src)

add_subdirectory(examples)

