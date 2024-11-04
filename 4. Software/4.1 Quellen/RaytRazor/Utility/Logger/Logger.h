#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

using namespace std;

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
         * @brief Methode um den Logger zu initialisieren.
         */
        static void initialize();

        /**
         * @brief Methode zur Überprüfung, ob der Logger initialisiert ist.
         * @returns @code true@endcode, wenn Logger initialisiert ist. Anderenfalls, @code false@endcode.
         */
        static bool isInitialized();

        /**
         * @brief Methode um Nachrichten zu loggen.
         * @param msgType Art der Nachricht die geloggt werden soll.
         * @param logMessage Nachricht die geloggt werden soll.
         */
        static void log(MessageType msgType, const string& logMessage);

        /**
         * //TODO Write method comment.
         */
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
         * @brief Methode um Werte aus der Config-Datei zu laden.
         */
        static void loadConfigValues();

        /**
         * @brief Methode um Einträge in das Log zu schreiben.
         * @param logEntry Log-Eintrag (Prefix + Nachricht) als String.
         */
        static void writeLog(const string& logEntry);

};

#endif
