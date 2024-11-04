#include "Logger.h"
#include <iostream>
#include <ctime>
#include <filesystem>
#include <thread>

// TODO
// - Fix: Logger not writing to logfile.
// - Add: Implement thread safety measures.
// - Add: Make logger fetch static variables 'logDirectory' and 'logFileName' from program 'config.json' file.

// Static Attributes.

std::string Logger::logDirectory = "./logs/";
std::string Logger::logFileName = "log.log";
std::string Logger::logFile = logDirectory + logFileName;
std::ofstream Logger::logFileStream;

// Static Methods

/**
 * //TODO Write method comment.
 */
void Logger::writeLog(const std::string& logEntry)
{

  if (!logFileStream.is_open())
  {
    std::filesystem::create_directories(logDirectory);
    logFileStream.open(logFile, std::ios_base::app);
  }

  if (logFileStream.is_open())
  {
    logFileStream << logEntry;
    logFileStream.flush();
  }
  else
  {
    std::cerr << "Logger::writeLog: Unable to open log file: " << logFile << std::endl;
  }

}

/**
 * //TODO Write method comment.
 */
void Logger::log(LoggerType logType, const std::string& logMessage)
{

  std::string logEntryPrefix;

  switch (logType)
  {
    case LoggerType::DEBUG:       logEntryPrefix = "[DEBUG] ";      break;
    case LoggerType::INFO:        logEntryPrefix = "[INFO] ";       break;
    case LoggerType::WARN:     logEntryPrefix = "[WARNING] ";       break;
    case LoggerType::SEVERE:       logEntryPrefix = "[ERROR] ";     break;
  }

  // TODO implement a local thread storage for allocating thread names to threads?
  std::stringstream logExecutingThread;
  logExecutingThread << "[Thread " << std::this_thread::get_id() << "] ";
  logEntryPrefix += logExecutingThread.str();

  std::time_t logEntryTime = std::time(nullptr);
  char logEntryTimeBuffer[20];
  std::strftime(logEntryTimeBuffer, sizeof(logEntryTimeBuffer), "%F %T", std::localtime(&logEntryTime));
  logEntryPrefix += std::string(logEntryTimeBuffer);

  std::string logEntry = logEntryPrefix + logMessage + "\n";
  writeLog(logEntry);
}

/**
 * //TODO Write method comment.
 */
void Logger::rotateLogs()
{

  if (logFileStream.is_open())
  {
    logFileStream.close();
  }

  std::time_t logFileTimeStamp = std::time(nullptr);
  char backupFileName[50];
  std::strftime(backupFileName, sizeof(backupFileName), "log_%Y%m%d_%H%m%s.log", std::localtime(&logFileTimeStamp));

  if (std::rename(logFile.c_str(), backupFileName) != 0)
  {
    std::cerr << "Logger::rotateLogs: Unable to rotate log file: " << logFile << std::endl;
  }

  logFileStream.open(logFile, std::ios_base::out | std::ios_base::trunc);
  if (!logFileStream.is_open())
  {
    std::cerr << "Logger::rotateLogs: Unable to open new log file: " << logFile << std::endl;
  }

}
