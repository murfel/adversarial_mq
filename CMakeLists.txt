cmake_minimum_required(VERSION 3.13)
project(adversarial_mq)

set(CMAKE_CXX_STANDARD 14)

set(BOOST_INCLUDEDIR "/nfs/scistore14/alistgrp/vaksenov/murfel/multiqueue/boost_1_75_0")

find_package(Boost COMPONENTS thread REQUIRED)

add_executable(adversarial_mq main.cpp)

target_link_libraries(adversarial_mq PRIVATE Boost::thread)
