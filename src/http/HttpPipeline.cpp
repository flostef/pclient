#include "HttpPipeline.h"
#include <workflow/HttpUtil.h>
#include "../log/LogService.h"

struct SeriesContext {
    std::string url;
    const std::string proxyUrl;
    const std::string ua;
    const int maxRedirects;
    int followedRedirects{};
    std::string seriesLog{};

    SeriesContext(std::string url,
                  std::string proxyUrl,
                  std::string ua,
                  int max_redirects)
            : url(std::move(url)),
              proxyUrl(std::move(proxyUrl)),
              ua(std::move(ua)),
              maxRedirects(max_redirects),
              seriesLog("ua: " + this->ua) {}
};

const int HttpPipeline::RETRY_MAX = 2;
const int HttpPipeline::MAX_TASK_CONCURRENCY = 10000;
std::atomic_int HttpPipeline::runningTasks(0);

bool HttpPipeline::process(const std::string &proxyUrl, const std::string &url, const std::string &ua) {
    if (runningTasks >= MAX_TASK_CONCURRENCY) {
        return false;
    }
    const auto &task = createHttpTask(proxyUrl, url, ua);
    const auto &series = Workflow::create_series_work(task, seriesCallback);
    const auto &context = new SeriesContext(url, proxyUrl, ua, 5);
    series->set_context(context);
    runningTasks++;
    series->start();
    return true;
}

WFHttpTask *HttpPipeline::createHttpTask(const std::string &proxyUrl, const std::string &url, const std::string &ua) {
    auto task = WFTaskFactory::create_http_task(
                url,
                proxyUrl,
                0,
                RETRY_MAX,
                requestCallback
            );
    task->get_req()->add_header_pair("User-Agent", ua);
//    task->get_req()->add_header_pair("Accept", "text/html,application/xhtml+xml,application/xml,application/json;q=0.9,*/*;q=0.8");
    return task;
}

void HttpPipeline::requestCallback(WFHttpTask *task) {
    protocol::HttpResponse *resp = task->get_resp();
    int state = task->get_state();
    int error = task->get_error();

    const auto &context = (SeriesContext *) series_of(task)->get_context();
    context->seriesLog += " -> " + context->url + "(" + context->proxyUrl + ")" + " ["
                          + (resp->get_status_code() == nullptr ? "FAILED" : resp->get_status_code()) + "]";

    switch (state) {
        case WFT_STATE_SYS_ERROR:LogService::info("System error: "  + std::string(strerror(error)) + " for " + context->url);
            break;
        case WFT_STATE_DNS_ERROR:LogService::info("DNS error: " + std::string(gai_strerror(error)) + " for " + context->url);
            break;
        case WFT_STATE_SSL_ERROR:LogService::info("SSL error: " + std::to_string(error) + " for " + context->url);
            break;
        case WFT_STATE_TASK_ERROR:LogService::info("Task error: " + std::to_string(error) + " for " + context->url);
            break;
        default:break;
    }

    if (state != WFT_STATE_SUCCESS) {
        return;
    }

//    const void *body;
//    size_t body_len;
//    resp->get_parsed_body(&body, &body_len);
//    fwrite(body, 1, body_len, stdout);
//    fflush(stdout);

    protocol::HttpHeaderCursor resp_cursor(resp);
    std::string headerName;
    std::string headerValue;
    while (resp_cursor.next(headerName, headerValue)) {
        if (toLower(headerName) == "location") {
            redirectIfNeeded(headerValue, series_of(task));
            return;
        }
    }




}

std::string HttpPipeline::toLower(const std::string & str) {
    std::string copy = str;
    std::transform(copy.begin(), copy.end(), copy.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return copy;
}

void HttpPipeline::redirectIfNeeded(const std::string &url, SeriesWork *series) {
    if (!isUrlMmp(url)) {
        ((SeriesContext *) series->get_context())->seriesLog += " -> " + url + "[NOT FOLLOWED (NOT MMP)]";
        return;
    }

    const auto &context = (SeriesContext *) series->get_context();
    if (context->followedRedirects >= context->maxRedirects) {
        ((SeriesContext *) series->get_context())->seriesLog += " -> " + url + "[NOT FOLLOWED (MAX REDIRECTS)]";
        return;
    }
    
    context->url = url;

    context->followedRedirects++;
    const auto &task = createHttpTask(context->proxyUrl, url, context->ua);
    series->push_back(task);
}

bool HttpPipeline::isUrlMmp(const std::string &url) {
    return (toLower(url).find("singular.net") != std::string::npos ||
            toLower(url).find("singular") != std::string::npos ||
            (toLower(url).find("tune") != std::string::npos && toLower(url).find("itunes") == std::string::npos) ||
            toLower(url).find("appsflyer") != std::string::npos ||
            toLower(url).find("sng.link") != std::string::npos ||
            (toLower(url).find("appsflyer") != std::string::npos
             && toLower(url).find("tracking.applift.com") == std::string::npos) ||
            toLower(url).find("tlnk.io") != std::string::npos ||
            toLower(url).find("app.link") != std::string::npos ||
            toLower(url).find("adjust.com") != std::string::npos);
}

void HttpPipeline::seriesCallback(const SeriesWork *series_work) {
    runningTasks--;
    const auto &context = (SeriesContext *) series_work->get_context();
    LogService::info(context->seriesLog);
    delete context;
}
