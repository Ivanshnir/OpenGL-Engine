#include "debugger.hpp"
#include <filesystem>
#include <format>
#include <sstream>
#include <ctime>
#include <thread>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
std::queue<logger::MessageInfo> MsgQueue;
std::string format = "[$h:$m:$s][$F:$f:$l][$S]$N\n";
std::mutex mtx;
std::condition_variable cv;
bool is_init = false;
std::string new_format;
std::fstream file;
std::string buffer;
unsigned int flushTrigger;
static std::thread MyWorker;
bool logger::init(std::string fileName, int bufferSize, const bool CMD){
    std::filesystem::create_directories("LOGS");
    file.open("LOGS\\"+fileName, std::fstream::out | std::fstream::trunc);
    if(!file.is_open())

        return false;
    MyWorker = std::thread(process_log_msg);
    flushTrigger = bufferSize;
    file << "|-----------------------------------------|" << "\n" << "|------------------INIT-------------------|" << "\n|-----------------------------------------|" << std::endl;


    buffer.clear();
    is_init = true;
    file.flush();

    return true;
}
void logger::writeFile(){
    file.write(buffer.data(), buffer.length());
    file.flush();
    buffer.clear();
}
void logger::closeFile(){
    
    is_init = false;
    if(MyWorker.joinable()){
        cv.notify_one();
        MyWorker.join();
        
    }
    file.close();
}
void logger::SetFormat(std::string format_example){
    format = format_example;
}
void logger::log(States state, std::string context,int line, std::string funcName, std::string fileName){
    if (!is_init)
        return;
    MessageInfo msg;
    msg.msg = context;
    msg.line = line;
    msg.fileName = fileName;
    msg.funcName = funcName;
    msg.state = state;
    {
        std::lock_guard<std::mutex>lock(mtx);
        MsgQueue.push(msg);
    }
    
    cv.notify_one();
    //_log_msg(std::ref(context), state, line, funcName, fileName);
}
void logger::process_log_msg(){
    while (true)
    {
        std::unique_lock<std::mutex>lock(mtx);
        cv.wait(lock, [](){ return (is_init == false) || (!MsgQueue.empty()); });
        auto bufWrite = [=](const std::string format){
            buffer += format;
            if(buffer.length() >= flushTrigger){
                writeFile();
            }
        };
        if(is_init == false){
            while (!MsgQueue.empty())
            {
                bufWrite(process_format_msg(MsgQueue.front()));
                MsgQueue.pop();
            }
            writeFile();
            break;
        }

        bufWrite(process_format_msg(MsgQueue.front()));
        States st = MsgQueue.front().state;
        MsgQueue.pop();
        lock.unlock();
        if(st == logger::States::ERROR || st == logger::States::DEBUG || st == logger::States::WARN || st == logger::States::FATAL){
            writeFile();
        }

    }
    
    
}
std::string logger::process_format_msg(MessageInfo & Target){
    bool marker = false;
    std::stringstream formatfillings;
    std::unordered_map<States,std::string> state_strings = 
    {
        {logger::States::FATAL, "FATAL"},
        {logger::States::ERROR, "ERROR"},
        {logger::States::WARN, "WARN"},
        {logger::States::DEBUG, "DEBUG"},
        {logger::States::INFO, "INFO"},
        {logger::States::TRACE, "TRACE"}
    };
    for(size_t i = 0; i < format.length(); i++){
        if(format.at(i) == '$'){
            marker = true;
            continue;
        }
        if (marker)
        {
            auto const now = std::chrono::system_clock::now();
            std::time_t newt = std::chrono::system_clock::to_time_t(now);
            struct tm dateTime = *localtime(&newt);
            switch (format.at(i))
            {
            //basic info
            case 'S':
                formatfillings << state_strings[Target.state];
                break;
            case 'N':
                formatfillings << Target.msg;
                break;
            //Location
            case 'F':
                {
                int lastSlash = 0;
                for(size_t i = 0; i < Target.fileName.length(); i++){
                    if(Target.fileName[i] == '\\'){
                        lastSlash = i;
                    }
                }
                formatfillings << Target.fileName.substr(lastSlash+1, Target.fileName.length());
                }
                break;
            case 'f':
                formatfillings << Target.funcName;
                break;
            case 'l':
                formatfillings << Target.line;
                break;
            //time
            case 'h':
                formatfillings << dateTime.tm_hour;
                break;
            case 's':
                formatfillings << dateTime.tm_sec;
                break;
            case 'm':
                formatfillings << dateTime.tm_min;
                break;
            case 'M':
                formatfillings << dateTime.tm_mon;
                break;  
            case 'D':          
                formatfillings << dateTime.tm_mday;
                break;
            case 'Y':          
                formatfillings << dateTime.tm_year;
                break;
            default:
                break;
            } 
            marker = false;
        }else{
            formatfillings << format.at(i);
        }
        
       
    }
    return formatfillings.str();
}
logger::Tracer::Tracer(std::string _context,int _line, std::string _funcName, std::string _fileName) : line(_line), msg(_context), funcName(_funcName), fileName(_fileName){
    start = std::chrono::steady_clock::now();
    INNER_TRACE_LOG("[start]"<<msg, line, funcName, fileName);
}

logger::Tracer::~Tracer(){
    auto dur = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
    INNER_TRACE_LOG("[finish]Execution takes - " << dur, line, funcName, fileName);
}