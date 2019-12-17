
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17 -Wall -Wextra -Wshadow -Wnull-dereference -Wno-psabi -Wno-unused-function -pthread -g -D THREADS_STD")

add_library(OkapiLibV5 STATIC

remove all test/ files except implMocks.cpp

in implMocks remove all EXPECT_ and fix << operators