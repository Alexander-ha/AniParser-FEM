#ifndef PARSER_ADDON_H
#define PARSER_ADDON_H

#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <functional>
#include "parser_tree.h"


/*!
	\brief Simple addon for parser workaround
*/
/*! 
Check if string is in array 
 \param[in] value initial value
 \param[in] array array of checkable values
 \param[out] out true or false
 */
bool is_string_in_array(const std::string &value, const std::vector<std::string> &array);
/*! 
Print help command wherever you want to do it
 \param[in] graph you Parser tree or node or branch
 */
void print_help(const ParserTree& graph);

//INTERPRETER SYSTEM
//void processInput(const std::string& command);
//void cliInterpreter();

#endif // PARSER_ADDON_H
