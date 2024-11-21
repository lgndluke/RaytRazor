#include "Logger.h"
#include <ctime>
#include <filesystem>
#include <iostream>
#include <thread>
#include <unordered_map>

//TODO
// - Add: Implement thread safety measures.
// - Add: Make logger fetch static variables 'logDirectory' and 'logFileName' from program 'config.json' file.
// - Improve: Error Handling.

using namespace std;
using namespace std::filesystem;

// Attributes.

string Logger::configFileDirectory = "./config/"; //For now.
string Logger::configFileName = "config.json"; //For now.
string Logger::configFile = configFileDirectory + configFileName; //For now.
bool Logger::initialized = false; //For now.
int Logger::logFileSize = 25 * 1024; //25 KB for now.
int Logger::logFilesToSave = 3; //For now.
string Logger::logDirectory = "./logs/"; //For now.
string Logger::logFileName = "log_0.log"; //For now.
string Logger::logFile = logDirectory + logFileName; //For now.
ofstream Logger::logFileStream;

// Private Methods:

void Logger::deleteOldestLogFile(const unordered_map<string, file_time_type>& oldLogFiles)
{

  string oldestFile = {};
  file_time_type oldestTime = file_time_type::max();

  for (const auto& [fileName, modificationTime] : oldLogFiles)
  {
    if (modificationTime < oldestTime)
    {
      oldestTime = modificationTime;
      oldestFile = fileName;
    }
  }

  try
  {
    const path removeLogFile = logDirectory + oldestFile;
    remove(removeLogFile);
  }
  catch (const filesystem_error& e)
  {
    std::cerr << "Logger::rotateLogs: Löschen fehlgeschlagen! \n" << "Grund:" << e.what() << std::endl;
  }

}

string Logger::getDateTimeString(const string& format)
{

  const time_t now = time(nullptr);
  tm buffer = {};
  char timeBuffer[20];
  string result = {};

  #ifdef _WIN32
    localtime_s(&buffer, &now);
  #else
    localtime_r(&now, &buffer);
  #endif

  strftime(timeBuffer, sizeof(timeBuffer), format.c_str(), &buffer);

  result.append(string(timeBuffer));
  return result;

}

string Logger::getThreadID()
{

  stringstream result = {};
  result << this_thread::get_id();
  return result.str();

}

void Logger::loadConfigValues()
{

    //TODO

}

void Logger::rotateLogFile(const path& fileToRename = logFile, int counter = 0)
{

  string rotatedLogFileName = "log_" + getDateTimeString("%Y_%m_%d");

  if (counter > 0)
  {
    rotatedLogFileName.append("_" + to_string(counter));
  }

  rotatedLogFileName.append(".log");

  const path rotatedLogFile = logDirectory + rotatedLogFileName;

  if (exists(rotatedLogFile))
  {
    rotateLogFile(rotatedLogFile, ++counter);
  }

  try
  {
    rename(fileToRename, rotatedLogFile);
  }
  catch (const filesystem_error& e)
  {
    std::cerr << "Logger::rotateLogs: Umbenennen fehlgeschlagen! \n" << "Grund:" << e.what() << std::endl;
  }

}

void Logger::rotateLogs()
{

  const string logFile_NamePrefix = "log";
  unordered_map<string, file_time_type> oldLogFiles = {};

  if (logFileStream.is_open())
  {
    logFileStream.close();
  }

  try
  {
    for (const auto& entry : directory_iterator(logDirectory))
    {
      if (entry.is_regular_file() && entry.path().filename().string().substr(0, logFile_NamePrefix.length()) == logFile_NamePrefix)
      {
        oldLogFiles[entry.path().filename().string()] = last_write_time(entry.path());
      }
    }
  }
  catch (const filesystem_error& e)
  {
    std::cerr << "FileSystem Error: " << e.what() << std::endl;
  }

  if (oldLogFiles.size() <= logFilesToSave)
  {
    rotateLogFile();
  }
  else
  {
    deleteOldestLogFile(oldLogFiles);
    rotateLogFile();
  }

}

void Logger::writeLog(const string& logEntry)
{

  if (!logFileStream.is_open())
  {
    create_directories(logDirectory);
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

  if (file_size(logFile) > logFileSize)
  {
    rotateLogs();
  }

}

// Public Methods:

void Logger::initialize()
{
  //TODO implement reading values from config.json file.
  if (!initialized)
  {
    loadConfigValues();
    initialized = true;
  }

}

bool Logger::isInitialized()
{
  return initialized;
}

void Logger::log(const MessageType msgType, const string& logMessage)
{

  string logEntry;
  logEntry.append("[" + getDateTimeString("%H:%M:%S") + "]\t");

  switch (msgType)
  {
    case MessageType::DEBUG:    logEntry.append("[Thread: " + getThreadID() + "/DEBUG]:\t");  break;
    case MessageType::INFO:     logEntry.append("[Thread: " + getThreadID() + "/INFO]:\t");   break;
    case MessageType::WARN:     logEntry.append("[Thread: " + getThreadID() + "/WARN]:\t");   break;
    case MessageType::SEVERE:   logEntry.append("[Thread: " + getThreadID() + "/ERROR]:\t");  break;
  }

  logEntry.append(logMessage + "\n");
  writeLog(logEntry);

}
