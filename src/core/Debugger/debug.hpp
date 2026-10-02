#define DEBUG_STATE

#ifdef DEBUG_STATE
#ifndef DEBUG_HPP_
#define DEBUG_HPP_
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#define LOG(msg, state) logger::log(state, msg, __LINE__, __FUNCTION__ , __FILE__);

#define LOG_LEVEL 4
// LOG_LEVEL 0 - DISABLE
// LOG_LEVEL 1 - Only FATAL ERRORS
// LOG_LEVEL 2 - FATAL ERRORS, WARNS
// LOG_LEVEL 3 - FATAL ERRORS, WARNS, INFO
// LOG_LEVEL 4 - FATAL ERRORS, WARNS, TRACE
enum States{
    TRACE,
    WARN,
    INFO,
    FATAL
};
namespace logger{
    bool init(std::string fileName, int bufferSize = 4096, const bool CMD = false);
    void log(States state, std::string context,int line, std::string funcName, std::string fileName);
}

#endif
#endif