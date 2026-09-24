#pragma once
#include <optional>
#include <fstream>
#include <string>
#include "Message.h"  // so Parser knows what Message is

class Parser { 
    public: 
        Parser(const std::string& filename);
        std::optional<Message> next(); 
    private: 
        std::ifstream file; 
}; 