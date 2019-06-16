cmake_minimum_required(VERSION 2.8)

project(my_muduo)
add_executable(${PROJECT_NAME} "main.cpp" "poller.h" "poller.cpp" "noncopyable.h" "eventloop.h" "eventloop.cpp" "channel.h" "channel.cpp" "buffer.h" "buffer.cpp" "socket.h" "socket.cpp")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -lpthread -DNDEBUG -DEIGEN_MPL2_ONLY")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -lpthread -DEIGEN_MPL2_ONLY")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3 -lpthread -I/usr/include/freetype2 -DNDEBUG -DEIGEN_MPL2_ONLY")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -lpthread -I/usr/include/freetype2 -DEIGEN_MPL2_ONLY")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -ldl -DNDEBUG -DEIGEN_MPL2_ONLY")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -ldl -DEIGEN_MPL2_ONLY")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3 -ldl -I/usr/include/freetype2 -DNDEBUG -DEIGEN_MPL2_ONLY")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -ldl -I/usr/include/freetype2 -DEIGEN_MPL2_ONLY")
