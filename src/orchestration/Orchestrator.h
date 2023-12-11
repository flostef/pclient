#pragma once

#include <vector>

class Orchestrator {

public:
    static void startBatch(const std::vector<std::string> &proxyUrls, const std::string &trackingUrl, int budget,
                           int hours, bool withSession);

private:
    static std::vector<std::string> UAS;
};
