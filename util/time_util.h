#ifndef _LF_TIME_MS_H_
#define _LF_TIME_MS_H_

#include <chrono>
#include <ctime>
#include <string>

// get current local time stamp

namespace ltime {
    static const int64_t SEC_MINUTE = 60;
    static const int64_t SEC_HOUR = SEC_MINUTE * 60;
    static const int64_t SEC_DAY = SEC_HOUR * 24;
    static const int64_t SEC_WEEK = SEC_DAY * 7;

    int64_t getTimeMs()
    {
        // 当前时间点 系统时钟
        auto now = std::chrono::system_clock::now();

        // 时间转换为毫秒
        auto nowMs = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

        // 获取自本纪元开始时间
        auto duration = nowMs.time_since_epoch();

        // 返回毫秒数
        return duration.count();
    }

    int64_t getTime() {
        return getTimeMs() / 1000;
    }

    // 获取相隔天数
    int64_t interval(int64_t beginTm, int64_t endTm) {
        return (endTm - beginTm) / SEC_DAY;
    }

    int64_t getDayBegin(int64_t ts = 0) {
        if (!ts) {
            ts = getTime();
        }
        std::tm* timeInfo = std::localtime(&ts);
        timeInfo->tm_hour = 0;
        timeInfo->tm_min = 0;
        timeInfo->tm_sec = 0;
        return std::mktime(timeInfo);
    }

    int64_t getWeekBegin(int64_t ts = 0) {
        if (!ts) {
            ts = getTime();
        }
        std::tm* timeInfo = std::localtime(&ts);
        int currentDayOfWeek = timeInfo->tm_wday;
        int daysUntilMonday = (currentDayOfWeek + 6) % 7;
        timeInfo->tm_hour = 0;
        timeInfo->tm_min = 0;
        timeInfo->tm_sec = 0;
        timeInfo->tm_mday -= daysUntilMonday;
        return std::mktime(timeInfo);
    }

    int64_t getNextWeekBegin(int64_t ts = 0) {
        return getWeekBegin(ts) + SEC_WEEK;
    }

    int64_t getWeekDay(int64_t ts = 0) {
        if (!ts) {
            ts = getTime();
        }
        std::tm* now = std::localtime(&ts);
        return now->tm_wday;
    }

    std::string tm2str(int64_t ts, const std::string &format = "%Y-%m-%d %H:%M:%S") {
        char buffer[20];
        std::tm* timeInfo = std::localtime(&ts);
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
        return buffer;
    }
}

#endif // _LF_TIME_MS_H_