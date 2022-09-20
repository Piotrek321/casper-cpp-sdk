#include <atomic>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "librpc/common/algorithm/join.hpp"
#include "librpc/common/utils/log_configurator.hpp"

namespace librpc::common::utils
{

namespace details
{
static auto logger = LogConfigurator::initDefault("CommonRpc");
}

LogConfigurator LogConfigurator::initDefault(std::string const& name)
{
    librpc::common::utils::LogConfigurator log{name};
    log.configure();
    return log;
}

LogConfigurator::LogConfigurator(std::string const& log_name) noexcept:
    log_name_{log_name}
{}

std::shared_ptr<spdlog::logger> LogConfigurator::configure()
{
    auto logger = configureConsoleLogger();
    spdlog::set_level(spdlog::level::level_enum::trace);
    spdlog::set_pattern("[%b %m, %Y][%H:%M:%S:%e][%t][%^%l%$][%n][%!:%#] %v");
    spdlog::set_default_logger(logger);
    return logger;
}

std::shared_ptr<spdlog::logger> LogConfigurator::configureRotatingLogger()
{
    auto const max_size = 1048576 * 5;
    auto const max_files = 10;
    return spdlog::rotating_logger_mt(log_name_, algorithm::join("logs/rotating_", log_name_, ".txt"), max_size, max_files);
}

std::shared_ptr<spdlog::logger> LogConfigurator::configureConsoleLogger()
{
    return spdlog::stdout_color_mt(log_name_);
}
}
