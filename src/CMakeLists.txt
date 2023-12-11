set(SOURCES main.cc
        http/HttpPipeline.cpp
        http/HttpPipeline.h
        log/LogService.cpp
        log/LogService.h
        orchestration/Orchestrator.cpp
        orchestration/Orchestrator.h)

add_executable(pclient ${SOURCES})
target_link_libraries(pclient ${LIB})