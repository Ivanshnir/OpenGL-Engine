#define DEBUG_STATE

#ifdef DEBUG_STATE
#ifndef DEBUG_HPP_
#define DEBUG_HPP_
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include <sstream>
#include <chrono>
#define DEBUG_MODE true


enum States{
    WARN,
    INFO,
    ERROR,
    DEBUG,
    FATAL,
    TRACE
};
namespace logger{
    void closeFile();
    void writeFile();
    struct  Tracer
    {

        std::string msg;
        int line_;
        std::string func_name_;
        std::string file_name_;
        Tracer(std::string, int, std::string, std::string);
        ~Tracer();
        std::chrono::steady_clock::time_point start_time;
    };
    
    bool init(std::string fileName, int bufferSize = 512, const bool CMD = false);
    void log(States state, std::string context,int line, std::string funcName, std::string fileName);
}

#define LOG_FATAL(msg, state) {std::ostringstream oss{}; oss << msg; logger::log(state, oss.str(), __LINE__, __FUNCTION__ , __FILE__);}
#define LOG_ERROR(msg, state) {std::ostringstream oss{}; oss << msg; logger::log(state, oss.str(), __LINE__, __FUNCTION__ , __FILE__);}
#define LOG_LEVEL 3
#if LOG_LEVEL >= 1
    #define LOG_WARN(msg, state) {std::ostringstream oss{}; oss << msg; logger::log(state, oss.str(), __LINE__, __FUNCTION__ , __FILE__);}
#else 
    #define LOG_WARN(msg, state) {}
#endif
#if LOG_LEVEL >= 2
    #define LOG_INFO(msg, state) {std::ostringstream oss{}; oss << msg; logger::log(state, oss.str(), __LINE__, __FUNCTION__ , __FILE__);}
#else
    #define LOG_INFO(msg, state) {}
#endif
#if LOG_LEVEL >= 3
    #define LOG_TRACE(msg, state) {std::ostringstream oss{}; oss << msg; logger::Tracer(oss.str(), __LINE__, __FUNCTION__ , __FILE__);}
#else
    #define LOG_TRACE(msg, state) {}
#endif

#if DEBUG_MODE  
    #define LOG_DEBUG(msg, state) {std::ostringstream oss{}; oss << msg; logger::log(state, oss.str(), __LINE__, __FUNCTION__ , __FILE__);}
#else
    #define LOG_DEBUG(msg, state) {}
#endif

#define LOG(msg, state) LOG_##state(msg, state);
#endif
#endif