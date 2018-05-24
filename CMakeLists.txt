cmake_minimum_required(VERSION 2.8)
project( FloodFill )
find_package( OpenCV REQUIRED )
add_executable( FloodFill GSPriorityFlood.h ppmb_io.cpp main.cpp )
target_link_libraries( FloodFill ${OpenCV_LIBS} )

