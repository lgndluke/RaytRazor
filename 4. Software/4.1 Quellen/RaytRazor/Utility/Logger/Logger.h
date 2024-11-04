#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

/**
 * //TODO
 * @author Lukas Jeckle
 */
enum class LoggerType
{
    DEBUG,      //Used for logging 'DEBUG' messages.
    INFO,       //Used for logging 'INFORMATION' messages.
    WARN,       //Used for logging 'WARNING' messages.
    SEVERE      //Used for logging 'ERROR' messages.
};

/**
 * //TODO
 * @author Lukas Jeckle
 */
class Logger
{
    public:

        Logger() = delete;
        static void log(LoggerType logType, const std::string& logMessage);
        static void rotateLogs();

    private:

        static std::string logDirectory;
        static std::string logFileName;
        static std::string logFile;
        static std::ofstream logFileStream;
        static void writeLog(const std::string& logEntry);

};

#endif
