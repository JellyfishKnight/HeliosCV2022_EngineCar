project(Serials)

aux_source_directory(. DIR_SERIALS)

add_library(Serials ${DIR_SERIALS})


# 寻找OpenCV库   #这里的opencv文件路径为我自己的，需要修改时则修改
set(OpenCV_DIR /home/wjy/OPENCV/MINGW_BUILD/install)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(OpenCV 4 REQUIRED)

#添加头文件
include_directories(${OpenCV_INCLUDE_DIRS})
message("OPENCV INC DIR: ${OpenCV_INCLUDE_DIRS}")

set(OPENCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_calib3d)

#链接opencv库
target_link_libraries( Serials ${OPENCV_LIBS})