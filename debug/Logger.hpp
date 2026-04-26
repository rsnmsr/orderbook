#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>

namespace Logger {

inline std::ofstream logFile;
inline std::mutex logMutex;
inline bool logToConsole = true;

inline bool initLog(const std::string &filename, bool toConsole = true) {
    std::lock_guard<std::mutex> lk(logMutex);
    if (logFile.is_open()) logFile.close();
    logFile.open(filename, std::ios::out | std::ios::app);
    logToConsole = toConsole;
    return logFile.is_open();
}

inline void shutdownLog() {
    std::lock_guard<std::mutex> lk(logMutex);
    if (logFile.is_open()) logFile.close();
}

inline void writeLine(const std::string &level, const std::string &msg) {
    std::lock_guard<std::mutex> lk(logMutex);
    std::string line = "[" + level + "] " + msg + '\n';
    if (logToConsole) std::cout << line;
    if (logFile.is_open()) logFile << line;
}

inline void input(const std::string& msg) { writeLine("INPUT", msg); }
inline void engine(const std::string& msg) { writeLine("ENGINE", msg); }
inline void trade(const std::string& msg)  { writeLine("TRADE",  msg); }
inline void book(const std::string& msg)   { writeLine("BOOK",   msg); }
inline void check(const std::string& msg)  { writeLine("CHECK",  msg); }
inline void stats(const std::string& msg)  { writeLine("STATS",  msg); }

}

#endif