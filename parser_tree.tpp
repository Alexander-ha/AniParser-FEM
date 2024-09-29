#include <iostream>
#include <vector>
//#include "prob_args.h"
#include <map>
#include <sstream>
#include <array>
#include <cstring>
//#include <toml++/toml.hpp>
#include <set>
#include <fstream>
#include <memory>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <functional>
#include <cassert>
#include "parser_tree.h"

namespace internals{
    template <typename T>
    struct GenGetArgDescr{
        //x> Я закомментировал функцию по умолчанию, чтобы возникала ошибка компиляции, 
        //x> если производится вызов для типа без шаблонной перегрузки
        // static void make(){
        //     //static_assert(false);
        // }
    };
}

template <typename T>
void ParserTree::addNode(const std::string& fieldName, T* obj, ParamType type){
    auto node = std::make_shared<Node>(fieldName, type, obj);
    root->children.push_back(node);
}

