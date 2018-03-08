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

class Log {
    
    static std::string logFilePath;
    
public:
    
    static void i(std::string str);
    static void e(std::string str);
    
    static bool logToFile {false};
    static bool logToConsole {false};
};

void Log::i(std::string str) {
    if (logToConsole) std::cout << str.c_str();
    if (logToFile) 3;
}

#endif /* LOG_H */

