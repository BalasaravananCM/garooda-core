project(ipe_example)

include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(../src)

file(GLOB IPE_CONFIG_FILES  "../../data/config_json/*.json")

add_executable(${PROJECT_NAME} ipe_example.cpp ${IPE_CONFIG_FILES})

target_link_libraries(${PROJECT_NAME} libipe ${OpenCV_LIBS})
