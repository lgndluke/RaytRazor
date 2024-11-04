#include "Logger.h"
#include <ctime>
#include <iostream>
#include <filesystem>
#include <thread>

//TODO
// - Add: Implement thread safety measures.
// - Add: Make logger fetch static variables 'logDirectory' and 'logFileName' from program 'config.json' file.

using namespace std;

// Attributes.

string Logger::configFileDirectory = "./config/"; //For now.
string Logger::configFileName = "config.json"; //For now.
string Logger::configFile = configFileDirectory + configFileName; //For now.
bool Logger::initialized = false; //For now.
int Logger::logFileSize = 10 * 1024 * 1024; //For now.
int Logger::logFilesToSave = 3; //For now.
string Logger::logDirectory = "./logs/"; //For now.
string Logger::logFileName = "log0.log"; //For now.
string Logger::logFile = logDirectory + logFileName; //For now.
ofstream Logger::logFileStream;

// Private Methods:

/**
 * //TODO Write method comment.
 */
string Logger::getDateTimeString(const string& format)
{

  const time_t now = time(nullptr);
  tm buffer = {};
  char timeBuffer[20];
  string result = {};

  localtime_s(&buffer, &now);
  strftime(timeBuffer, sizeof(timeBuffer), format.c_str(), &buffer);

  result.append(string(timeBuffer));
  return result;

}

/**
 * //TODO Write method comment.
 */
string Logger::getThreadID()
{

  stringstream result = {};
  result << this_thread::get_id();
  return result.str();

}

/**
 * //TODO Write method comment.
 */
void Logger::loadConfigValues()
{

    //TODO

}

/**
 * //TODO Write method comment.
 */
void Logger::writeLog(const string& logEntry)
{

  if (!logFileStream.is_open())
  {
    filesystem::create_directories(logDirectory);
    logFileStream.open(logFile, ios_base::out | ios_base::app);
  }

  if (logFileStream.is_open())
  {
    logFileStream << logEntry;
    logFileStream.flush();
  }
  else
  {
    cerr << "Logger::writeLog: Unable to open log file: " << logFile << endl;
  }

}

// Public Methods:

/**
 * //TODO Write method comment.
 */
void Logger::initialize()
{
  //TODO implement reading values from config.json file.
  if (!initialized)
  {
    loadConfigValues();
    initialized = true;
  }

}

/**
 * //TODO Write method comment.
 */
bool Logger::isInitialized()
{
  return initialized;
}

/**
 * //TODO Write method comment.
 */
void Logger::log(const MessageType logType, const string& logMessage)
{

  string logEntry;
  logEntry.append("[" + getDateTimeString("%H:%M:%S") + "]\t");

  switch (logType)
  {
    case MessageType::DEBUG:    logEntry.append("[Thread: " + getThreadID() + "/DEBUG]:\t");  break;
    case MessageType::INFO:     logEntry.append("[Thread: " + getThreadID() + "/INFO]:\t");   break;
    case MessageType::WARN:     logEntry.append("[Thread: " + getThreadID() + "/WARN]:\t");   break;
    case MessageType::SEVERE:   logEntry.append("[Thread: " + getThreadID() + "/ERROR]:\t");  break;
  }

  logEntry.append(logMessage + "\n");
  writeLog(logEntry);

}

/**
 * //TODO Write method comment.
 */
void Logger::rotateLogs()
{

  //TODO Rework completely?

  if (logFileStream.is_open())
  {
    logFileStream.close();
  }

  string rotatedLogFileName = {};
  rotatedLogFileName.append(logDirectory);
  rotatedLogFileName.append("log_" + getDateTimeString("%Y_%m_%d") + ".log");

  cout << rotatedLogFileName << endl;

  if (rename(logFile.c_str(), rotatedLogFileName.data()) != 0)
  {
    cerr << "Logger::rotateLogs: Unable to rotate log file: " << logFile << endl;
  }

}
