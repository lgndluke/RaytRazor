#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

using namespace std;

/**
 * @brief This method does xyz
 * @author Lukas Jeckle
 */
enum class MessageType
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
        ~Logger() { logFileStream.close(); }
        static void initialize();
        static bool isInitialized();
        static void log(MessageType logType, const string& logMessage);
        static void rotateLogs();

    private:

        static string configFileDirectory;
        static string configFileName;
        static string configFile;
        static bool initialized;
        static int logFileSize;
        static int logFilesToSave;
        static string logDirectory;
        static string logFileName;
        static string logFile;
        static ofstream logFileStream;
        static string getDateTimeString(const string& format);
        static string getThreadID();
        static void loadConfigValues();
        static void writeLog(const string& logEntry);

};

#endif
