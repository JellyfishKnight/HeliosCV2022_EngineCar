cmake_minimum_required(VERSION 3.22)
project(HeliosCV2022_EngineTruck)

set(CMAKE_CXX_STANDARD 14)

set(HELIOS_HPPS Detector/Detector.h Serials/SerialPort.h PreProcess/PreProcess.cpp PreProcess/PreProcess.h)
set(HELIOS_CPPS Detector/Detector.cpp Serials/SerialPort.cpp PreProcess/PreProcess.cpp PreProcess/PreProcess.h)

# 寻找OpenCV库   #这里的opencv文件路径为我自己的，需要修改时则修改
set(OpenCV_DIR /home/wjy/OPENCV/MINGW_BUILD/install)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package( OpenCV 4 REQUIRED )

#添加头文件
include_directories( ${OpenCV_INCLUDE_DIRS} )
message("OPENCV INC DIR: ${OpenCV_INCLUDE_DIRS}")

set(OPENCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

add_executable(HeliosCV2022_EngineTruck main.cpp ${HELIOS_HPPS} ${HELIOS_CPPS} )

#链接opencv库
target_link_libraries(HeliosCV2022_EngineTruck ${OPENCV_LIBS})
