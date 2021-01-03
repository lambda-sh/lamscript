#include "Lamscript/util/Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace lamscript {
namespace util {

std::shared_ptr<spdlog::logger> Log::kLogger;

// @todo Update this so that the log level can be set during either
// compile or runtime.
void Log::Init() {
  spdlog::set_pattern("%^[%T]-[%n]-[%s]: %v%$");
  kLogger = spdlog::stdout_color_mt("Lamscript");
  kLogger->set_level(spdlog::level::trace);
}

}  // namespace util
}  // namespace lamscript
