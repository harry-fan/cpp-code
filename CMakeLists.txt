cmake_minimum_required(VERSION 3.14)
project(cpp_test)

set(CMAKE_CXX_STANDARD 11)

add_executable(select_client.c
        select_server1.c
        select_server2.c
        epoll_server.cpp
        epoll_client.cpp
        shm_info.cpp
        thread_pool.cpp tree_node1.cpp test_code.cpp mobilize.cpp upDown.cpp
        quick_sort.cpp)
