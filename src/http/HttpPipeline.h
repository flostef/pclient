#pragma once

#include <atomic>
#include <workflow/WFTaskFactory.h>

class HttpPipeline {

public:
    static bool process(const std::string &proxyUrl, const std::string &url, const std::string &ua);


private:

    static void requestCallback(WFHttpTask *task);
    static void seriesCallback(const SeriesWork* series_work);
    static WFHttpTask *createHttpTask(const std::string &proxyUrl, const std::string &url, const std::string &ua);
    static std::string toLower(const std::string & str);
    static void redirectIfNeeded(const std::string &url, SeriesWork* series);
    static bool isUrlMmp(const std::string &url);

    static std::atomic_int runningTasks;
    const static int RETRY_MAX;
    const static int MAX_TASK_CONCURRENCY;
};
