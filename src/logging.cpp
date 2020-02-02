#include "logging.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/null_sink.h>
#include <iostream>

namespace Pandemic {

void setupLog(__attribute__((unused))const spdlog::level::level_enum level)
{
#ifdef LOG_OFF
    auto logger = spdlog::null_logger_mt(LOG_SINK);
#else
    auto logger = spdlog::stdout_color_mt(LOG_SINK);
    spdlog::set_pattern("[%Y-%m-%d %T.%f] [%l] %v");
    spdlog::set_level(level);
    LOG_INFO("Log level: {}", spdlog::level::to_string_view(level));
#endif
}

spdlog::level::level_enum loglevelFromString(const std::string& levelString)
{
    auto level = spdlog::level::from_str(levelString);
    if (level == spdlog::level::off && levelString != "off")
    {
        std::cerr << "Failed to parse log level\"" << levelString << "\"" << std::endl;
        level = spdlog::level::info;
    }
    return level;
}

}
