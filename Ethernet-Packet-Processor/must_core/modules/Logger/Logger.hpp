#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <sstream>
#include <sys/sysinfo.h> // For sysinfo struct
#include <stdexcept>
#include <iomanip>
#include <memory>

enum class LogLevel {
    INFO,
    ERROR
};

constexpr const char* levelStr[] = {
    "INFO",
    "ERROR"
};

constexpr unsigned long SECONDS_PER_HOUR = 3600;
constexpr unsigned long SECOND_PER_MINUTE = 60;
constexpr unsigned int DIGITS_FOR_FORMAT = 2;

class Logger {
public:
    // Delete copy constructor and assignment operator to prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance(const std::string& logPath);
    static Logger& getInstance();

    void Log(LogLevel level, const std::string& message);

private:
    Logger(const std::string& logPath);
    ~Logger();

    std::string getSystemUptime() const;

    std::ofstream logFile_;
    std::string logPath_;
    mutable std::mutex logMutex_;

    static inline std::unique_ptr<Logger> instance_ = nullptr;
    static inline std::once_flag onceFlag_;

    friend std::default_delete<Logger>;
};
