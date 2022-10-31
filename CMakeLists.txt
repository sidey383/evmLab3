cmake_minimum_required(VERSION 3.22)
project(Lab3)

set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV REQUIRED )

add_executable( Camera camera.cpp)
add_executable( LibUSB libUSB.cpp)

target_link_libraries( Camera ${OpenCV_LIBS})
target_link_libraries( LibUSB "C:/Users/sidey/Documents/libusb-1.0.26-binaries/libusb-MinGW-x64/lib/libusb-1.0.a"  )
