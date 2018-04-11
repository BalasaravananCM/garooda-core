project (libipe)

include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB IPE_SOURCE_FILES  "opencv_bridge/*.hpp"
                            "opencv_bridge/*.cpp"
                            "dlib_bridge/*.hpp"
                            "dlib_bridge/*.cpp"
                            "*.hpp" "*.cpp")

add_library(${PROJECT_NAME} ${IPE_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${dlib_LIBRARIES})


