#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;
using namespace std::filesystem;

/**
 * @brief Enum das die unterschiedlichen Nachrichten Typen definiert.
 *
 * @code DEBUG@endcode - um 'DEBUG' Nachrichten zu loggen.
 * @code INFO@endcode - um 'INFO' Nachrichten zu loggen.
 * @code WARN@endcode - um 'WARN' Nachrichten zu loggen.
 * @code SEVERE@endcode - um 'ERROR' Nachrichten zu loggen.
 *
 * @author Lukas Jeckle
 */
enum class MessageType
{
    DEBUG,
    INFO,
    WARN,
    SEVERE
};

/**
 * @brief Logger-Implementation.
 * @author Lukas Jeckle
 */
class Logger
{
    public:

        Logger() = delete;
        ~Logger() { logFileStream.close(); }

        /**
         * @brief Methode um Nachrichten zu loggen.
         * @param msgType Art der Nachricht die geloggt werden soll.
         * @param logMessage Nachricht die geloggt werden soll.
         */
        static void log(MessageType msgType, const string& logMessage);

    private:

        static int logFileSize;
        static int logFilesToSave;
        static string logDirectory;
        static string logFileName;
        static string logFile;
        static ofstream logFileStream;

        /**
         * @brief Methode, welche die älteste log-Datei löscht.
         */
        static void deleteOldestLogFile(const unordered_map<string, file_time_type>& oldLogFiles);

        /**
         * @brief Methode um eine einzige Log-Datei zu rotieren.
         */
        static void rotateLogFile(const path& fileToRename, int counter);

        /**
         * @brief Methode um die erstellen Log-Dateien zu rotieren.
         */
        static void rotateLogs();

        /**
         * @brief Methode um aktuellen DateTime-Wert als formatierten String-Wert zu holen.
         * @param format String-Wert um die Formatierung zu definieren. z.B.: @code "%H:%M:%S"@endcode, @code "%Y_%m_%d"@endcode.
         * @returns DateTime als String-Wert formatiert anhand @code format@endcode.
         */
        static string getDateTimeString(const string& format);

        /**
         * @brief Methode um die ID des ausführenden Threads als String-Wert zu holen.
         * @returns Thread-ID als String-Wert.
         */
        static string getThreadID();

        /**
         * @brief Methode um Einträge in das Log zu schreiben.
         * @param logEntry Log-Eintrag (Prefix + Nachricht) als String.
         */
        static void writeLog(const string& logEntry);

};

#endif
