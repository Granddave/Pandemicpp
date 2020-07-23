#pragma once

#include <spdlog/spdlog.h>
#include <string>

#define LOG_SINK "log"

#define LOG_TRACE(...) spdlog::get(LOG_SINK)->trace(__VA_ARGS__)
#define LOG_DEBUG(...) spdlog::get(LOG_SINK)->debug(__VA_ARGS__)
#define LOG_INFO(...) spdlog::get(LOG_SINK)->info(__VA_ARGS__)
#define LOG_WARN(...) spdlog::get(LOG_SINK)->warn(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::get(LOG_SINK)->error(__VA_ARGS__)
#define LOG_CRIT(...) spdlog::get(LOG_SINK)->critical(__VA_ARGS__)

namespace pandemic
{

void setupLog(const spdlog::level::level_enum level);
void setupLog(const std::string& level = "info");
spdlog::level::level_enum loglevelFromString(const std::string& levelString);

} // namespace pandemic
