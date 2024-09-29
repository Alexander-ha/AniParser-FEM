#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include "interpreter.h"

Interpreter::Interpreter() : argc(0), running(false) {
    std::string input;
    std::cout << "Interpreter is connected, input cli(on) to use interpreter mode\n";
    std::getline(std::cin, input);
    std::istringstream iss(input);
    std::string command;

    while (iss >> command) {
            argv.push_back(command); 
            commandBuffer.push_back(command);
    }
    argc = static_cast<int>(argv.size());
    Interpreter::processInput();
}

std::vector<std::string> Interpreter::getCommands() const {
    const std::vector<std::string> constVectorCommands = commandBuffer;
    return splitString(joinStrings(constVectorCommands)); 
};

void Interpreter::processInput() {
    for (const auto& command : argv) {
        if (command == "cli(on)") {
            running = true;
            cliInterpreter();
        } else {
            std::cout << "Executing argument: " << command << std::endl;
        }
    }
}

std::string Interpreter::joinStrings(const std::vector<std::string>& input) const {
    std::ostringstream oss;  
    for (size_t i = 0; i < input.size(); ++i) {
        oss << input[i];  
        if (i != input.size() - 1) {
            oss << " ";  
        }
    }
    return oss.str();  
}

std::vector<std::string> Interpreter::splitString(const std::string& str) const {
    std::istringstream iss(str);  
    std::vector<std::string> result;
    std::string temp;

    while (iss >> temp) {  
        result.push_back(temp); 
    }

    return result;  
}

std::vector<std::string> Interpreter::rearrangeVector(const std::vector<std::string>& input) const {
    std::vector<std::string> withoutSpaces;
    std::vector<std::string> withSpaces;

    for (const auto& str : input) {
        if (str.find(' ') != std::string::npos) {
            withSpaces.push_back(str);
        } else {
            withoutSpaces.push_back(str);
        }
    }

    withoutSpaces.insert(withoutSpaces.end(), withSpaces.begin(), withSpaces.end());
    return withoutSpaces;
}

void Interpreter::cliInterpreter() {
    std::string input;
    std::ostringstream oss;
    running = true;

    std::cout << "Entering CLI mode. Type 'cli(off)' to exit:\n";
    
    while (running) {
        std::getline(std::cin, input);
        if (input == "cli(off)") {
            std::cout << "Exiting CLI mode.\n";
            running = false;
            break;
        }
        commandBuffer.push_back(input);
        argv.push_back(input); 
        argc++; 
    }
}

Interpreter::~Interpreter(){}
   


