cmake_minimum_required(VERSION 3.13)
project(adversarial_mq)

set(CMAKE_CXX_STANDARD 14)

add_executable(adversarial_mq main.cpp priority_queue.h multiqueue.h)
