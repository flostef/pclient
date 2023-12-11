#include "LogService.h"
#include <iostream>

void LogService::info(const std::string &what) {
    std::cout << what << std::endl;
}
