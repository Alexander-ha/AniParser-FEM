#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <parser_tree.h>
#include <functional>

//Simple implementation for parser containers to check if certain string is in array.
//
//value - is your string
//
//array - is your container of strings
bool is_string_in_array(const std::string &value, const std::vector<std::string> &array) {
        const std::string prefix = "path:";
        std::string currentArg = value;
        if (currentArg.compare(0, prefix.length(), prefix) == 0) {
                currentArg.erase(0, prefix.length());
            }
       for (const auto &s : array) {
           if (currentArg.find(s) != std::string::npos) {
               return true;
           }
       }
       return false;
   }
//Help command, prints all argument nodes of your model and their description
//You can use this function everywhere in your custom modifications in order to print info
//about agruments.
//graph - is your current ParserTree
void print_help(const ParserTree& graph) {
    std::function<void(const std::shared_ptr<ParserTree::Node>&)> printNodeDescr;
    printNodeDescr = [&printNodeDescr](const std::shared_ptr<ParserTree::Node>& node) {
        if (node->description.empty())  {
            std::cout << "Argument name: " << node->name << "\n" << " - Descripton: " << " (no description)" << "\n" << " - ParamType: " << param_type_to_string(node->type) << std::endl;
        } else {
            std::cout << "Argument name: " << node->name << "\n" << " - Descripton: " << node->description << "\n" << " - ParamType: " << param_type_to_string(node->type) << std::endl;
        }
        if (&(node->children) != nullptr){
        for (const auto& child : node->children) {
            if (child != nullptr) {
                printNodeDescr(child);
            }
          }
        }   
    };

    if (graph.root.get() != nullptr) {
        printNodeDescr(graph.root);
    } else {
        std::cout << "The graph is empty." << std::endl;
    }

}

//INTERPRETER SYSTEM:
/*void cliInterpreter() {
    std::string input;
    std::ostringstream oss;
    std::cout << "Input cli(off) (press 'cli(off)', to stop inerpreting):\n";

    while (true) {
        std::getline(std::cin, input);

        if (input == "cli(off)") {
            std::cout << "Exit CLI.\n";
            break;
        }
        if (!oss.str().empty()) {
            oss << " "; 
        }
        oss << input;
    }
}*/

/*void processInput(const std::string& command) {
    if (command == "cli(on)") {
        cliInterpreter();
    } else {
        std::cout << "Executing argument seting: " << command << std::endl;
    }
}*/








