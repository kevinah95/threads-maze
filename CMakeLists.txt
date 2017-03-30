cmake_minimum_required(VERSION 3.5)
project(threads-maze)

set(CMAKE_C_STANDARD 99)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCE_FILES pthread_examples/dynamic_pthread.c)

add_executable(threads-maze ${SOURCE_FILES})
target_link_libraries(threads-maze Threads::Threads)