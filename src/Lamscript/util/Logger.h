/// @file Logger.h
/// @brief The languages lightweight logging utility.
#ifndef SRC_LAMSCRIPT_UTIL_LOGGER_H_
#define SRC_LAMSCRIPT_UTIL_LOGGER_H_

#include <memory>

#include <spdlog/spdlog.h>

namespace lamscript {
namespace util {

/// @brief The engine wide logging API. Should primarily be used through the
/// macros exposed at the bottom of the API.
class Log {
 public:
  static void Init();
  static std::shared_ptr<spdlog::logger> GetLogger() {
      return kLogger; }

 private:
  static std::shared_ptr<spdlog::logger> kLogger;
};

}  // namespace util
}  // namespace lamscript


#ifdef LAMSCRIPT_LOGGING_ENABLED

/// @def LAMSCRIPT_TRACE
/// @brief Log tracing information within the engine.
#define LAMSCRIPT_TRACE(...) \
    SPDLOG_LOGGER_TRACE( \
        ::lamscript::util::Log::GetLogger(), __VA_ARGS__);

/// @def LAMSCRIPT_INFO
/// @brief Log informational information within the engine.
#define LAMSCRIPT_INFO(...)  \
    SPDLOG_LOGGER_INFO(::lamscript::util::Log::GetLogger(), __VA_ARGS__);

/// @def LAMSCRIPT_WARN
/// @brief Log warning information within the engine.
#define LAMSCRIPT_WARN(...)  \
    SPDLOG_LOGGER_WARN(::lamscript::util::Log::GetLogger(), __VA_ARGS__);

/// @def LAMSCRIPT_ERROR
/// @brief Log error information within the engine.
#define LAMSCRIPT_ERROR(...) \
    SPDLOG_LOGGER_ERROR( \
        ::lamscript::util::Log::GetLogger(), __VA_ARGS__);

/// @def LAMSCRIPT_ERROR
/// @brief Log fatal information within the engine.
#define LAMSCRIPT_FATAL(...) \
    SPDLOG_LOGGER_FATAL( \
        ::lamscript::util::Log::GetLogger(), __VA_ARGS__);

#else

#define LAMSCRIPT_TRACE(...)
#define LAMSCRIPT_INFO(...)
#define LAMSCRIPT_ERROR(...)
#define LAMSCRIPT_FATAL(...)

#endif  // LAMSCRIPT_LOGGING_ENABLED

#endif  // SRC_LAMSCRIPT_UTIL_LOGGER_H_
