#include "debug.hpp"
std::fstream file;
char * current_buffer = nullptr;
int current_size = 0;
bool isOpen = false;
bool logger::init(std::string fileName, int bufferSize, const bool CMD){
    file.open("src\\core\\Debugger\\"+fileName, std::fstream::out | std::fstream::trunc);
    if(!file.is_open())
        return false;
    current_buffer = new char[bufferSize];
    current_size = bufferSize;
    file << "|-----------------------------------------|" << "\n" << "|------------------INIT-------------------|" << "\n|-----------------------------------------|" << std::endl;

    isOpen = true;
    return true;
}
void logger::log(States state, std::string context,int line, std::string funcName, std::string fileName){
    
    if (LOG_LEVEL == 0)
        return;
    else if (!isOpen)
        return;
    
    char buff[80];
    time_t time_raw = time(nullptr);
    tm * current_time = std::localtime(&time_raw);
    strftime(buff,sizeof(buff),"[%H:%M:%S]",current_time);
    
    auto func = [=](){file << buff << '[' << fileName << ']' << '[' << funcName << ':' << std::to_string(line) << ']'; };
    switch (state)
    {
        case TRACE:
            if(LOG_LEVEL >= 4){
                func();
                file << "[TRACE]" << context << "\n";
            }
            break;
        case INFO:
            if(LOG_LEVEL >= 3){
                func();
                file << "[INFO]" << context << "\n";
            }
            break;
        case WARN:
            if(LOG_LEVEL >= 2){
                func();
                file << "[WARN]" << context << std::endl;
                //Why here std::endl?
                //Std::endl if compare with \n have one neccessery difference
                //std::endl forces stream to flush their buffer thus we use endl only for warns and all critical stuff
            }
            break;
        case FATAL:
            if(LOG_LEVEL >= 1){
                func();
                file << "[FATAL]" << context << std::endl;
            }
            break;


        default:
            break;
    }
}