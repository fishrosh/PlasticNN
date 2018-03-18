/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Log.h
 * Author: kamil
 *
 * Created on March 7, 2018, 9:31 PM
 */

#ifndef LOG_H
#define LOG_H

#include <string>
#include <iostream>
#include <fstream>
#include <memory>

class Log {
    
    std::string logFilePath = "Log";
    
    std::ostream& out;
    std::ofstream outf;
    
    static std::shared_ptr<Log> current_instance;
    
public:
    
    Log();
    
    static void i(std::string str);
    static void ln(std::string str);
    static void breakln();
    
    static void BindInstance (std::weak_ptr<Log> log_ptr);
    
    bool logToFile {true};
    bool logToConsole {true};

protected:

    //virtual void LogToOuput (std::string& str) const;
    //virtual void LogToFile (std::string& str) const;
};

std::shared_ptr<Log> Log::current_instance = std::shared_ptr<Log> ();

Log::Log()
    : out {std::cout}, outf {logFilePath.c_str(), std::ios::out}
{

}

void Log::i(std::string str) {
    if (current_instance) {
        if (current_instance->logToConsole) current_instance->out << str.c_str();
        if (current_instance->logToFile) current_instance->outf << str.c_str();
    }
}

void Log::ln(std::string str) {
    if (current_instance) {
        if (current_instance->logToConsole) current_instance->out << str.c_str() << std::endl;
        if (current_instance->logToFile) current_instance->outf << str.c_str() << std::endl;
    }
}

void Log::breakln() {
    if (current_instance) {
        if (current_instance->logToConsole) current_instance->out << std::endl;
        if (current_instance->logToFile) current_instance->outf << std::endl;
    }
}

void Log::BindInstance(std::weak_ptr<Log> log_ptr) {
    current_instance = log_ptr.lock();
}

#endif /* LOG_H */

