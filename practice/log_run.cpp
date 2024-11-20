#include "../util/log.h"

void run() {
    LOG_INFO("this is log info");
    int64_t a = 10;
    LOG_DEBUG("this is print log debug info %d", a);
}

/**
 *  g++ --std=c++11 log_run.cpp ../lib/loglib.a -lpthread
 * */
int main() {
    Log::Instance()->init(0, "./log", "_main.log");
    run();
    Log::Instance()->stop();
    return 0;
}