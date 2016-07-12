
cmake_minimum_required(VERSION 2.8)

project( Annotator )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( labeler labeler.cc )
target_link_libraries( labeler ${OpenCV_LIBS} )