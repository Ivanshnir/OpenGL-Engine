#include "debug.hpp"
#include <filesystem>
std::fstream file;
std::string buffer;

#include <format>
int flushTrigger = 0;
bool isOpen = false;
bool logger::init(std::string fileName, int bufferSize, const bool CMD){
    std::filesystem::create_directories("LOGS");
    file.open("LOGS\\"+fileName, std::fstream::out | std::fstream::trunc);
    if(!file.is_open())
        return false;
    flushTrigger = bufferSize;
    file << "|-----------------------------------------|" << "\n" << "|------------------INIT-------------------|" << "\n|-----------------------------------------|" << std::endl;
    buffer.clear();
    isOpen = true;
    file.flush();

    return true;
}
void logger::writeFile(){
    file.write(buffer.data(), buffer.length());
    file.flush();
    buffer.clear();
}
void logger::closeFile(){
    writeFile();
    file.close();
}
void logger::log(States state, std::string context,int line, std::string funcName, std::string fileName){
    if (!isOpen)
        return;
    
    char buff[80];
    time_t time_raw = time(nullptr);
    tm * current_time = std::localtime(&time_raw);
    strftime(buff,sizeof(buff),"[%H:%M:%S]",current_time);
    auto bufWrite = [=](const std::string format){
        buffer += format;
        if(buffer.length() >= flushTrigger){
            writeFile();
        }
    };
    auto func = [=](){bufWrite(std::string(buff)+"["+fileName+"]"+"["+funcName+":"+std::to_string(line)+"]");};
    switch (state)
    {
        case WARN:
            func();
            bufWrite(("[WARN] " + context + "\n"));
            break;
        case ERROR:
            func();
            bufWrite(("[ERROR] " + context + "\n"));
            writeFile();
            break;
        case FATAL:
            func();
            bufWrite(("[FATAL] " + context + "\n"));
            writeFile();
        case INFO:
            func();
            bufWrite(("[INFO] " + context + "\n"));
            break;
        case TRACE:
            func();
            bufWrite(("[TRACE] " + context + "\n"));
            break;
        case DEBUG:
            func();
            bufWrite(("[DEBUG] " + context + "\n"));
            writeFile();
            break;

        default:
            break;
    }
}

logger::Tracer::Tracer(std::string msg_, int line, std::string funcName, std::string fileName): start_time(std::chrono::steady_clock::now()){
    msg = msg_;
    line_ = line;
    func_name_ = funcName;
    file_name_ = fileName;
} 
logger::Tracer::~Tracer(){
    const std::chrono::duration<double> secs(std::chrono::steady_clock::now() - start_time);
    if(msg != ""){
        msg = "[Add.Info. = "+msg+"]";
        msg += std::format("ENTR. & END FUNC time - {}sec",std::to_string(secs.count()));
    }else{
        
        msg = std::format("ENTR. & END FUNC time - {}sec", std::to_string(secs.count()));

    }
    log(TRACE, msg, line_, func_name_, file_name_);
}