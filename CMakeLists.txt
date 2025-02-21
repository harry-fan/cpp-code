cmake_minimum_required(VERSION 3.14)
project(cpp-code)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(base/util)
include_directories(algorithm)
include_directories(cplus_module)
include_directories(game_module)
include_directories(linux_module)
include_directories(base/third)

add_executable(MyProject
        # algorithm
        algorithm/quick_sort.cpp

        # cplus_module
        cplus_module/epoll_client.cpp
        cplus_module/epoll_server.cpp
        cplus_module/mutex_shm.cpp
        cplus_module/select_client.c
        cplus_module/select_server1.c
        cplus_module/select_server2.c
        cplus_module/shm_info.cpp
        cplus_module/tree_node1.cpp
        cplus_module/thread_pool.cpp
        cplus_module/consistent_hash.cpp

        # game_module
        game_module/king_honor_match.cpp
        game_module/mobilize.cpp
        game_module/up_or_down.cpp
        game_module/send_appsflyer_event.cpp

        # linux_module
        linux_module/kernel_thread.cpp
        linux_module/linux_malloc_memory.cpp
        linux_module/signal_handle.cpp
        linux_module/user_thread.cpp
        linux_module/virtual_table.cpp
        linux_module/atom_queue.cpp
        linux_module/linux_kernel2.cpp
        linux_module/linux_kernel.h
        linux_module/linux_kernel.cpp
        linux_module/asm_learn.c

        # util
        base/util/connection.h
        base/util/helpers.h
        base/util/json.hpp
        base/util/restclient.h
        base/util/time_util.h
        base/util/version.h
        game_module/must_hit.cpp
        game_module/c_array.c
        game_module/no_inherit.cpp
        algorithm/mid_search.cpp
        redis_module/adlist.cpp
        game_module/oasis_match.cpp
        game_module/oasis_match.h
        base/util/buffer.h
        base/util/buffer.cpp
        base/util/blockqueue.h
        base/util/log.cpp
        practice/log_run.cpp
        game_module/parse_config.cpp

        base/third/nlohmann_json/json.hpp
        game_module/oasis_jsonmatch.cpp
        game_module/league_duel_match.cpp
        game_module/get_ip_info.cpp
        game_module/get_ip_info.cpp
        algorithm/list_reverse.cpp
)
