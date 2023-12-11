#include "workflow/HttpMessage.h"
#include "workflow/WFFacilities.h"
#include "orchestration/Orchestrator.h"

static WFFacilities::WaitGroup wait_group(1);

void sigHandler(int sigNumber)
{
    if (sigNumber == SIGINT || sigNumber == SIGTERM) {
        wait_group.done();
    }
}


int main()
{
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);

//    std::vector<std::string> proxyUrls = {
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-pa-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-mi-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-nj-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-ia-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-il-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-va-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-ms-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-md-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-oh-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-ma-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-wv-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-tn-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-la-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-ks-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/",
//            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-us-state-wy-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/"
//    };

    std::vector<std::string> proxyUrls = {
            "http://brd-customer-hl_c0e7b66d-zone-alex_test-country-gb-session-$session:4j3h4fww0cnl@brd.superproxy.io:22225/"
    };
    std::string trackingUrl = "http://track.adformant.swaarm-clients.com/click?offer_id=1104&pub_id=338";
//    std::this_thread::sleep_for(std::chrono::hours(4));
// local start time 14:45
    Orchestrator::startBatch(proxyUrls, trackingUrl, 200000, 2, true);

    wait_group.wait();
    return 0;
}

