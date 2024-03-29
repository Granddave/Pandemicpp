#include "logging.h"

#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <iostream>

#include "utils.h"

namespace pandemic
{
void setupLog(const spdlog::level::level_enum level)
{
#ifdef LOG_OFF
    UNUSED(level);
    spdlog::null_logger_mt(LOG_SINK);
#else
    spdlog::stdout_color_mt(LOG_SINK);
    spdlog::set_pattern("[%Y-%m-%dT%T.%f%z] [%l] %v");
    spdlog::set_level(level);
    LOG_INFO("Log level: {}", spdlog::level::to_string_view(level));

#endif
}

void setupLog(const std::string& level)
{
    setupLog(loglevelFromString(level));
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

} // namespace pandemic
