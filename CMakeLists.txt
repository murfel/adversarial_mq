cmake_minimum_required(VERSION 3.13)
project(adversarial_mq)

set(CMAKE_CXX_STANDARD 14)

#set(SOURCES src/main.cpp src/multiqueue.cpp src/priority_queue_t.cpp src/priority_queue_with_buffer.cpp src/utils.cpp
#        include/multiqueue.h include/priority_queue_t.h include/priority_queue_with_buffer.h include/utils.h)
#add_executable(adversarial_mq ${SOURCES})

add_executable(adversarial_mq src/main.cpp include/multiqueue.h src/multiqueue.cpp include/priority_queue_t.h src/priority_queue_t.cpp include/priority_queue_with_buffer.h include/utils.h src/utils.cpp src/priority_queue_with_buffer.cpp)

target_include_directories(adversarial_mq PRIVATE include)
