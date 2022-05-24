cmake_minimum_required(VERSION 3.22)
project(HeliosCV2022_EngineTruck)

set(CMAKE_CXX_STANDARD 14)

#添加头文件目录
include_directories(
        ${PROJECT_SOURCE_DIR}/Detector
        ${PROJECT_SOURCE_DIR}/Serials
        ${PROJECT_SOURCE_DIR}/PreProcess
)

#搜索包含cmakelist.txt和源文件的子目录
add_subdirectory(./Detector)
add_subdirectory(./Serials)
add_subdirectory(./PreProcess)

set(TARGET Detector PreProcess Serials)

add_executable(HeliosCV2022_EngineTruck ${TARGET} main.cpp)
#链接静态库
target_link_libraries(
        ${PROJECT_NAME} ${TARGET}
)

# 寻找OpenCV库   #这里的opencv文件路径为我自己的，需要修改时则修改
set(OpenCV_DIR /home/wjy/OPENCV/MINGW_BUILD/install)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(OpenCV 4 REQUIRED)

#添加头文件
include_directories(${OpenCV_INCLUDE_DIRS})

set(OPENCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_calib3d)

#链接opencv库
target_link_libraries(${PROJECT_NAME} ${OPENCV_LIBS})
