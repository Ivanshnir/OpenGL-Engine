#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include <sstream>
#include <chrono>
#include <queue>
#define LOG_LEVEL 4
namespace logger{

    enum class States{
        WARN,
        INFO,
        ERROR,
        DEBUG,
        FATAL,
        TRACE
    };
    struct MessageInfo{
            std::string msg;
            States state;
            int line;
            std::string funcName; 
            std::string fileName;
    };
    
    void closeFile();
    bool init(std::string fileName, int bufferSize = 512, const bool CMD = false);
    void log(States state, std::string context,int line, std::string funcName, std::string fileName);
    void SetFormat(std::string);
    
    void process_log_msg();
    std::string process_format_msg(MessageInfo &);

    void writeFile();
    struct Tracer{
        std::chrono::steady_clock::time_point start;
        std::string msg;
        int line;
        std::string funcName;
        std::string fileName;
        Tracer(std::string,int, std::string, std::string);
        ~Tracer();
    };

}

#define LOG_FATAL(msg, state) {std::ostringstream oss{}; oss << msg; logger::log(logger::States::FATAL, oss.str(), __LINE__, __FUNCTION__ , __FILE__);}
#define LOG_ERROR(msg, state) {std::ostringstream oss{}; oss << msg; logger::log(logger::States::ERROR, oss.str(), __LINE__, __FUNCTION__ , __FILE__);}
#if LOG_LEVEL >= 1
    #define LOG_WARN(msg, state) {std::ostringstream oss{}; oss << msg; logger::log(logger::States::WARN, oss.str(), __LINE__, __FUNCTION__ , __FILE__);}
#else 
    #define LOG_WARN(msg, state) {}
#endif
#if LOG_LEVEL >= 3
    #define LOG_INFO(msg, state) {std::ostringstream oss{}; oss << msg; logger::log(logger::States::INFO, oss.str(), __LINE__, __FUNCTION__ , __FILE__);}
#else
    #define LOG_INFO(msg, state) {}
#endif
#if LOG_LEVEL >= 4
    #define LOG_TRACE(msg, state) {std::ostringstream oss{}; oss << msg; logger::Tracer(oss.str(), __LINE__, __FUNCTION__ , __FILE__);}
#else
    #define LOG_TRACE(msg, state) {}
#endif

#if LOG_LEVEL >= 2
    #define LOG_DEBUG(msg, state) {std::ostringstream oss{}; oss << msg; logger::log(logger::States::DEBUG,oss.str(), __LINE__, __FUNCTION__ , __FILE__);}
#else
    #define LOG_DEBUG(msg, state) {}
#endif

#define LOG(msg, state) LOG_##state(msg, state);
#define INNER_TRACE_LOG(msg, line, func, filename) {std::ostringstream oss{}; oss << msg; logger::log(logger::States::TRACE,oss.str(), line, func, filename);}