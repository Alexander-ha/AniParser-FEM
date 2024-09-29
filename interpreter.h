#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include "parser_tree.h"
/*!
	\brief Interpreter module

    Simply instantinate Interpreter from this header to connect this module to your code
*/
class Interpreter {
    public:
        Interpreter();
        std::vector<std::string> commandBuffer;  
        std::vector<std::string> argv; 
        size_t argc;                      

        /*!
         Getter of commands in vector of strings
        */
        std::vector<std::string> getCommands() const;
        /*!
         Getter of command string
        */
        std::string getCommandString() const;
        /*!
         Getter
        */
        std::vector<std::string> getArgv() const { return argv; }
        /*!
         Getter
        */ 
        std::vector<std::string> getStringArgs() const { return argv; } 
        /*!
         Split string
        \param[in] str const std::string&
        \param[out] vector const std::vector<std::string> 
        */
        std::vector<std::string> splitString(const std::string& str) const;
        /*!
         Join string       
         \param[out] str const std::string&
         \param[in] vector const std::vector<std::string> 
        */
        std::string joinStrings(const std::vector<std::string>& input) const;

        /*Get size of commanbuffer*/
        size_t size() const { return commandBuffer.size(); }
        /*Clear commanbuffer*/
        void clear() { commandBuffer.clear(); }

        ~Interpreter();

    private:
        //Service args
        bool running;
        std::string commandString;
        const std::vector<std::string> constVectorCommands;

        std::vector<std::string> rearrangeVector(const std::vector<std::string>& input) const;

        //Service methods 
        void processInput();
        void cliInterpreter();
        void appendCommandToArgv();
        void copyCharArray(char* dest[], char* src[], int size);
};

#endif
