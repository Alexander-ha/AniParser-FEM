#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <array>
#include <cstring>
#include <set>
#include <fstream>
#include <memory>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <functional>
#include <cassert>
#include <iterator>
#include "parser_addon.h"
#include <regex>
#include "parser_tree.h"



//Добавлены все типы ParamType
std::string param_type_to_string(ParamType t){
    switch (t){
        case ParamType::Int: return "Int";
        case ParamType::Double: return "Real";
        case ParamType::String: return "String";
        case ParamType::Bool: return "Bool";
        case ParamType::IntArray: return "IntArray";
        case ParamType::DoubleArray: return "DoubleArray";
        case ParamType::StringArray: return "StringArray";
        case ParamType::BoolArray: return "BoolArray";
        default:
            throw std::runtime_error("Faced unknown ParamType");
    }
};

ParamType param_type_from_string(const std::string& s){
    static const std::map<std::string, ParamType> name_map{
      {"Int", ParamType::Int},
      {"Real", ParamType::Double},
      {"String", ParamType::String},
      {"Bool", ParamType::Bool},
      {"IntArray", ParamType::IntArray},
      {"DoubleArray", ParamType::DoubleArray},
      {"StringArray", ParamType::StringArray},
      {"BoolArray", ParamType::BoolArray},
    };
    auto it = name_map.find(s);
    if (it == name_map.end()) 
        throw std::runtime_error("String \"" + s + "\" is not associated with known ParamType");
    return it->second;
};

template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& v) {
    std::copy(std::istream_iterator<T>(is), std::istream_iterator<T>(),
              std::back_inserter(v));
    return is;
};

template<class T>
std::ostream& operator<<(std::ostream& os, std::vector<T>& v)
{
    copy(begin(v), end(v), std::ostream_iterator<T>(os, " ") );
    return os;
};


// Структура значений
//>x Конструкторы
ParamValue::ParamValue(ParamType type, const void* _value) : type(type){ 
    switch (type){
        case ParamType::Int: new(&value.intValue) long(*reinterpret_cast<const long*>(_value)); break;
        case ParamType::Double: new(&value.doubleValue) double(*reinterpret_cast<const double*>(_value)); break;
        case ParamType::String: new(&value.stringValue) std::string(*reinterpret_cast<const std::string*>(_value)); break;
        case ParamType::Bool: new(&value.boolValue) bool(*reinterpret_cast<const bool*>(_value)); break;
        case ParamType::IntArray: value.intArrayValue = *static_cast<const std::vector<long>*>(_value); break;
        case ParamType::StringArray: new(&value.stringArrayValue) std::vector<std::string>(*reinterpret_cast< const std::vector<std::string>*>(_value)); break;
        case ParamType::BoolArray: new(&value.boolArrayValue) std::vector<bool>(*reinterpret_cast< const std::vector<bool>*>(_value)); break;
        case ParamType::DoubleArray: new(&value.doubleArrayValue) std::vector<double>(*reinterpret_cast<const std::vector<double>*>(_value)); break;
        }
    }
ParamValue::ParamValue(ParamType type) : type(type){ 
    switch (type){
        case ParamType::Int: new(&value.intValue) long(); break;
        case ParamType::Double: new(&value.doubleValue) double(); break;
        case ParamType::String: new(&value.stringValue) std::string(); break;
        case ParamType::Bool: new(&value.boolValue) bool(); break;
        case ParamType::IntArray: new(&value.intArrayValue) std::vector<long>; break;
        case ParamType::StringArray: new(&value.stringArrayValue) std::vector<std::string>; break;
        case ParamType::BoolArray: new(&value.boolArrayValue) std::vector<bool>; break;
        case ParamType::DoubleArray: new(&value.doubleArrayValue) std::vector<double>; break;
        }
    }
ParamValue::ParamValue(long _value) { set(_value); }
ParamValue::ParamValue(double _value) { set(_value); }
ParamValue::ParamValue(const std::string& _value) { set(_value); }
ParamValue::ParamValue(bool _value) { set(_value); }
ParamValue::ParamValue(std::vector<long> _value) { set(_value); }
ParamValue::ParamValue(std::vector<std::string> _value) { set(_value); }
ParamValue::ParamValue(std::vector<bool> _value) { set(_value); }
ParamValue::ParamValue(std::vector<double> _value) { set(_value); }
ParamValue::ParamValue(const ParamValue& other) { type = other.type; copy_multivalue(other.value); }
ParamValue::ParamValue(ParamValue&& other) { type = other.type; move_copy_multivalue(std::move(other.value)); other.destroy_multivalue(); other.type = ParamType::Int; }
//>x Опреатор присваивания
ParamValue& ParamValue::operator=(const ParamValue& other) { if (this == &other) return *this; type = other.type; copy_multivalue(other.value); return *this; }
ParamValue& ParamValue::operator=(ParamValue&& other) { 
        if (this == &other) return *this; 
        type = other.type; move_copy_multivalue(std::move(other.value)); 
        other.destroy_multivalue(); other.type = ParamType::Int; 
        return *this; 
    }

//>x Сеттеры
ParamValue& ParamValue::set(long _value){ type = ParamType::Int; new(&value.intValue) long(_value); return *this; }
ParamValue& ParamValue::set(double _value){ type = ParamType::Double; new(&value.doubleValue) double(_value); return *this; }
ParamValue& ParamValue::set(const std::string& _value){ type = ParamType::String; new(&value.stringValue) std::string(_value); return *this; }
ParamValue& ParamValue::set(bool _value){ type = ParamType::Bool; new(&value.boolValue) bool(_value); return *this; }
ParamValue& ParamValue::set(std::vector<long> _value){ type = ParamType::IntArray; new(&value.intArrayValue) std::vector<long>(_value); return *this; }
ParamValue& ParamValue::set(std::vector<std::string> _value){ type = ParamType::StringArray; new(&value.stringArrayValue) std::vector<std::string>(_value); return *this;}
ParamValue& ParamValue::set(std::vector<bool> _value){ type = ParamType::BoolArray; new(&value.boolArrayValue) std::vector<bool>(_value); return *this;}
ParamValue& ParamValue::set(std::vector<double> _value){type = ParamType::DoubleArray; new(&value.doubleArrayValue) std::vector<double>(_value); return *this;}
//>x Геттеры
long& ParamValue::Integer() { 
        if (type != ParamType::Int)
            throw std::runtime_error("Typed access to ParamValue with wrong type");
        return value.intValue;
    }
double& ParamValue::Real() { 
        if (type != ParamType::Double)
            throw std::runtime_error("Typed access to ParamValue with wrong type");
        return value.doubleValue;
    }
std::string& ParamValue::String() { 
        if (type != ParamType::String)
            throw std::runtime_error("Typed access to ParamValue with wrong type");
        return value.stringValue;
    }
bool& ParamValue::Bool() { 
        if (type != ParamType::Bool)
            throw std::runtime_error("Typed access to ParamValue with wrong type");
        return value.boolValue;
    }
std::vector<long>& ParamValue::IntArray() {
        if (type != ParamType::IntArray)
            throw std::runtime_error("Typed access to ParamValue with wrong type");
        return value.intArrayValue;
    }
std::vector<std::string>& ParamValue::StringArray() {
        if (type != ParamType::StringArray)
            throw std::runtime_error("Typed access to ParamValue with wrong type");
        return value.stringArrayValue;
    }
std::vector<bool>& ParamValue::BoolArray() {
        if (type != ParamType::BoolArray)
            throw std::runtime_error("Typed access to ParamValue with wrong type");
        return value.boolArrayValue;
    }    
std::vector<double>& ParamValue::DoubleArray() {
        if (type != ParamType::DoubleArray)
            throw std::runtime_error("Typed access to ParamValue with wrong type");
        return value.doubleArrayValue;
    }


long ParamValue::Integer() const { return const_cast<ParamValue*>(this)->Integer(); }
double ParamValue::Real() const { return const_cast<ParamValue*>(this)->Real(); }
std::string ParamValue::String() const { return const_cast<ParamValue*>(this)->String(); }
bool ParamValue::Bool() const { return const_cast<ParamValue*>(this)->Bool(); }
std::vector<long>& ParamValue::IntArray() const {return const_cast<ParamValue*>(this) -> IntArray();}
std::vector<std::string>& ParamValue::StringArray() const {return const_cast<ParamValue*>(this) -> StringArray();}
std::vector<bool>& ParamValue::BoolArray() const {return const_cast<ParamValue*>(this) -> BoolArray();}
std::vector<double>& ParamValue::DoubleArray() const {return const_cast<ParamValue*>(this) -> DoubleArray();}
    /// @warning The function assume that actual type of the @param value coincide with ParamValue storage
    void ParamValue::copy_value(const ParamValue& from, void* out){
        switch (from.type){
            case ParamType::Int: *reinterpret_cast<long*>(out) = from.value.intValue; break;
            case ParamType::Double: *reinterpret_cast<double*>(out) = from.value.doubleValue; break;
            case ParamType::String: *reinterpret_cast<std::string*>(out) = from.value.stringValue; break;
            case ParamType::Bool: *reinterpret_cast<bool*>(out) = from.value.boolValue; break;
            //case ParamType::Path: *reinterpret_cast<stringpath*>(out) = from.value.pathValue; break;
            case ParamType::IntArray: *reinterpret_cast<std::vector<long>*>(out) = from.value.intArrayValue; break;
            case ParamType::StringArray: *reinterpret_cast<std::vector<std::string>*>(out) = from.value.stringArrayValue; break;
            //case ParamType::PathArray: *reinterpret_cast<pathsarray*>(out) = from.value.pathArrayValue; break;
            case ParamType::BoolArray: *reinterpret_cast<std::vector<bool>*>(out) = from.value.boolArrayValue; break;
            case ParamType::DoubleArray: *reinterpret_cast<std::vector<double>*>(out) = from.value.doubleArrayValue; break;
        }
    }

    // Деструктор 
ParamValue::~ParamValue() {
        switch (type){
            case ParamType::String: value.stringValue.~basic_string(); break;
            case ParamType::IntArray: value.intArrayValue.~vector<long>(); break;
            case ParamType::StringArray: value.stringArrayValue.~vector<std::string>(); break;
            case ParamType::BoolArray: value.boolArrayValue.~vector<bool>(); break;
            case ParamType::DoubleArray: value.doubleArrayValue.~vector<double>(); break;
            default: break;
        }
    }

std::pair<bool, ParamValue> ParamValue::make_ParamValue_from_string(ParamType type, std::string& str){
        std::stringstream ss(str);
        ParamValue pv(type);

        std::string sval;   
        
        switch(type){
            case ParamType::Int: ss >> pv.value.intValue; break;
            case ParamType::Double: ss >> pv.value.doubleValue; break;
            case ParamType::String: ss >> pv.value.stringValue; break;
            case ParamType::Bool: ss >> std::boolalpha >> pv.value.boolValue; break;            
            case ParamType::IntArray:
                long val;
                while (ss >> val) {
                    pv.value.intArrayValue.push_back(val);
                }
                break;
            case ParamType::StringArray:
                while (ss >> sval) {
                    pv.value.stringArrayValue.push_back(sval);
                }
                break;
            case ParamType::BoolArray:
                bool bval;
                while (ss >> bval) {
                    pv.value.boolArrayValue.push_back(bval);
                }
                break;
            case ParamType::DoubleArray:
                double dval;
                while (ss >> dval) {
                    pv.value.boolArrayValue.push_back(dval);
                }
                break;
            default: return {false, pv};
            }
        return {ss.peek() == std::stringstream::traits_type::eof(), pv};
    }

std::ostream& ParamValue::print_paramval(std::ostream& out){
        switch (type){
            case ParamType::Int: return out << value.intValue;
            case ParamType::Double: return out << value.doubleValue;
            case ParamType::String: return out << value.stringValue;
            case ParamType::Bool: return out << (value.boolValue ? "true" : "false");
            case ParamType::IntArray:
                out << "[";
            for (const auto& val : value.intArrayValue) out << val << " ";
                out << "]";
                return out;
            case ParamType::StringArray: 
                out << "[";
                for (const auto& val : value.stringArrayValue) out << val << " ";
                out << "]";
                return out;
            case ParamType::BoolArray: 
                out << "[";
                for (const auto& val : value.boolArrayValue) out << val << " ";
                out << "]";
                return out;
            case ParamType::DoubleArray: 
                out << "[";
                for (const auto& val : value.doubleArrayValue) out << val << " ";
                out << "]";
                return out;
            default: 
                return out;
        }
    }

void ParamValue::destroy_multivalue(){
        switch (type){
            case ParamType::String: value.stringValue.~basic_string(); break;
            case ParamType::IntArray: value.intArrayValue.~vector(); break;
            case ParamType::StringArray: value.stringArrayValue.~vector(); break;
            case ParamType::BoolArray: value.boolArrayValue.~vector(); break;
            case ParamType::DoubleArray: value.intArrayValue.~vector(); break;
            default: break; 
        }
    }
void ParamValue::copy_multivalue(const MultiValue& other){
        if (&value == &other) return;
        switch (type){
            case ParamType::Int: new(&value.intValue) long(other.intValue); break;
            case ParamType::Double: new(&value.doubleValue) double(other.doubleValue); break;
            case ParamType::String: new(&value.stringValue) std::string(other.stringValue); break;
            case ParamType::Bool: new(&value.boolValue) bool(other.boolValue); break;
            case ParamType::IntArray: new(&value.intArrayValue) std::vector<long>(other.intArrayValue); break;
            case ParamType::StringArray: new(&value.stringArrayValue) std::vector<std::string>(other.stringArrayValue); break;
            case ParamType::BoolArray: new(&value.boolArrayValue) std::vector<bool>(other.boolArrayValue); break;
            case ParamType::DoubleArray: new(&value.intArrayValue) std::vector<long>(other.intArrayValue); break;
        }
    }
void ParamValue::move_copy_multivalue(MultiValue&& other){
        if (&value == &other) return;
        switch (type){
            case ParamType::Int: new(&value.intValue) long(other.intValue); break;
            case ParamType::Double: new(&value.doubleValue) double(other.doubleValue); break;
            case ParamType::String: new(&value.stringValue) std::string(std::move(other.stringValue)); break;
            case ParamType::Bool: new(&value.boolValue) bool(other.boolValue); break;
            case ParamType::IntArray: new(&value.intArrayValue) std::vector<long>(std::move(other.intArrayValue)); break;
            case ParamType::StringArray: new(&value.stringArrayValue) std::vector<std::string>(std::move(other.stringArrayValue)); break;
            case ParamType::BoolArray: new(&value.boolArrayValue) std::vector<bool>(std::move(other.boolArrayValue)); break;
            case ParamType::DoubleArray: new(&value.doubleArrayValue) std::vector<double>(std::move(other.doubleArrayValue)); break;
        }
    }
std::ostream& ParamValue::print_multivalue(std::ostream& out){
        switch (type){
            case ParamType::Int: return out << value.intValue;
            case ParamType::Double: return out << value.doubleValue;
            case ParamType::String: return out << value.stringValue;
            case ParamType::Bool: return out << value.boolValue;
            case ParamType::IntArray:
            for (const auto& val : value.intArrayValue) out << val << " ";
                return out;
            case ParamType::StringArray: 
                for (const auto& val : value.stringArrayValue) out << val << " ";
                return out;
            case ParamType::BoolArray: 
                for (const auto& val : value.boolArrayValue) out << val << " ";
                return out;
            case ParamType::DoubleArray: 
                for (const auto& val : value.doubleArrayValue) out << val << " ";
                return out;
            default: 
                return out;
        }
    }


//Структура создания узла
struct Node {
        using SetFunc = std::function<bool(const ParamValue& value)>;

        std::string name;
        ParamType type;
        ParamValue stored_object;
        std::string description;
        void* object = nullptr; 
        SetFunc set_func = nullptr;
        std::string init_str;

        std::vector<std::shared_ptr<Node>> children;
        Node* parent;
        
        bool isLeaf() const {return children.empty(); };
        bool isValue() const {return isLeaf() && set_func != nullptr; };
        bool haveRefObject() const {return object != nullptr; }
    
        Node() = default;

        //Далее идут перегрузки конструктора для различных методов
        Node(const std::string& fieldName, ParamType paramType, void* obj, SetFunc set_Func = nullptr)
            : name(fieldName), type(paramType), object(obj), set_func(set_Func) {}

        Node(const std::string& fieldName, void* obj)
            : name(fieldName), object(obj) {}

        Node(const std::string& fieldName, ParamType paramType, std::string default_value)
            : name(fieldName), type(paramType), init_str(default_value) {
                auto val = ParamValue::make_ParamValue_from_string(paramType, default_value);
                if (!val.first){ throw std::runtime_error("Non-convertible default_value = \"" + default_value + "\" while type = " + param_type_to_string(paramType)); };
                stored_object = std::move(val.second);
            }

        Node(const std::string& fieldName, void* obj, ParamType paramType)
            : name(fieldName), object(obj), type(paramType){}
    };  

//Rus: Считывание аргументов с командой строки, вывод сообщений по усмотрению
//Ввод аргументов осуществляется в виде key value, либо key=value
//Разделение уровня вложенности определяется через '.'
//Пример:  model.ss.x = 2 | model.ss.x 2
//
//Eng: Reading arguments from cmd line. Message logging is at the discretion of the user
//
void ParserTree::readParamsFromCmdLine(int argc, char* argv[], bool print_messages) {
        std::vector<std::shared_ptr<Node>> checkable_nodes = root->children;
        std::vector<std::string> checkable_names;
        for (const auto &s : checkable_nodes) {
                checkable_names.push_back(s->name);
        };

        //Временный контейнер для хранения пар ключ-значение
        //Изначально думал использовать хеш-таблицы, но мне кажется это гораздо затратнее 
        std::vector<std::pair<std::string, std::string>> keyvalue_data;

        for (int i = 1; i < argc; ++i) {
            const std::string prefix = "path:";
            std::string currentArg = argv[i];
            std::cout << "The model is model" << argv[i] << std::endl;
            if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0){
                print_help(*this);
                std::cout<<"For additional help use --help|-h. \n To use the interpreter mode, enter: cli(on), \n if you have it connected, \n cli(off) to turn it off. \n  --version|-v to get current version"<<std::endl;
                return;
            }
            if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0){
                std::cout << "\nParser version: 0.0.1 \n" << std::endl;
                if (!root->name.empty()){
                    std::cout << "Current program name is: " << root->name <<std::endl;
                }
                else{
                    std::cout << "Current program has no name. \n" <<std::endl;
                }
                return;
            }
            if (currentArg.compare(0, prefix.length(), prefix) == 0) {
                currentArg.erase(0, prefix.length());
            }
            if (std::string(argv[i]).find('=') != std::string::npos){
                std::string parameterName = currentArg.substr(0, currentArg.find('='));
                std::string paramString = currentArg;
                if (is_string_in_array(parameterName, checkable_names)) {
                    for (int j = i + 1; j < argc; ++j) {
                        std::string nextArg = argv[j];
                        if (is_string_in_array(nextArg, checkable_names)) {
                            break;
                        }
                        paramString += " " + nextArg;
                        i++;
                    }
                parseParameter(root.get(), paramString);
                }
            }
            else{
                if (is_string_in_array(currentArg, checkable_names)) {
                    std::string key = currentArg;
                    int pos = i + 1;
                    std::string val;
                    while(pos < argc && !is_string_in_array(argv[pos], checkable_names)) {
                        val.append(argv[pos]);
                        val.append(" ");
                        pos++;
                    }
                    i = pos - 1;
                    val.pop_back();
                    keyvalue_data.push_back(make_pair(key, val));
                }
            }
        };
}
void ParserTree::readParamsFromCmdLine(const std::vector<std::string>& argv, bool print_messages) {
        int argc = argv.size();
        //std::cout<<argv[0]<<std::endl;
        std::vector<std::shared_ptr<Node>> checkable_nodes = root->children;
        std::vector<std::string> checkable_names;
        for (const auto &s : checkable_nodes) {
                checkable_names.push_back(s->name);
        };

        //Временный контейнер для хранения пар ключ-значение
        //Изначально думал использовать хеш-таблицы, но мне кажется это гораздо затратнее 
        std::vector<std::pair<std::string, std::string>> keyvalue_data;

        for (int i = 0; i < argc; ++i) {
            const std::string prefix = "path:";
            std::string currentArg = argv[i];
            //std::cout << currentArg << std::endl;
            if ((argv[i] == "--help") || (argv[i] == "-h")){
                print_help(*this);
                std::cout<<"For additional help use --help|-h. \n To use the interpreter mode, enter: cli(on), \n if you have it connected, \n cli(off) to turn it off. \n  --version|-v to get current version"<<std::endl;
                return;
            }
            if ((argv[i] == "--version") || (argv[i] == "-v")){
                std::cout << "\nParser version: 0.0.1 \n" << std::endl;
                if (!root->name.empty()){
                    std::cout << "Current program name is: " << root->name <<std::endl;
                }
                else{
                    std::cout << "Current program has no name. \n" <<std::endl;
                }
                return;
            }
            if (currentArg.compare(0, prefix.length(), prefix) == 0) {
                currentArg.erase(0, prefix.length());
            }
            if (std::string(argv[i]).find('=') != std::string::npos){
                std::string parameterName = currentArg.substr(0, currentArg.find('='));
                std::string paramString = currentArg;
                if (is_string_in_array(parameterName, checkable_names)) {
                    for (int j = i + 1; j < argc; ++j) {
                        std::string nextArg = argv[j];
                        if (is_string_in_array(nextArg, checkable_names)) {
                            break;
                        }
                        paramString += " " + nextArg;
                        i++;
                    }
                parseParameter(root.get(), paramString);
                }
            }
            else{
                if (is_string_in_array(currentArg, checkable_names)) {
                    std::string key = currentArg;
                    int pos = i + 1;
                    std::string val;
                    while(pos < argc && !is_string_in_array(argv[pos], checkable_names)) {
                        val.append(argv[pos]);
                        val.append(" ");
                        pos++;
                    }
                    i = pos - 1;
                    val.pop_back();
                    keyvalue_data.push_back(make_pair(key, val));
                }
            }
        };

//Расскоментируйте метод для отображения пар ключ-значений разделенных уровнем вложенности
        for (const auto& pair : keyvalue_data) { 
            /* std::cout << "(" << pair.first << ", "
                   << pair.second << ")" << std::endl; */
            parseParameterByPair(pair);
        }
}

void ParserTree::parseParameterByPair(const std::pair<std::string, std::string>& input_pair){
        std::string path = input_pair.first;
        std::string arg = input_pair.second;
        descentDefine(root.get(), path, arg);
    }

void ParserTree::descentDefine(Node* parent, const std::string& path, const std::string& arg){

        size_t equalDot = path.find('.');
        std::string path_way = path;
        std::string parent_name;
        std::string children_name;
        size_t start = 0;
        bool endl = 0;
        Node* current_parent = parent;
        if (equalDot == std::string::npos){
            children_name = path_way;
            std::string dest_arg = arg;
            for (int i = 0; i < current_parent->children.size(); i++) {
                if (current_parent->children[i]->name == children_name) {
                    current_parent = current_parent->children[i].get();
                }
            }
            auto val = ParamValue::make_ParamValue_from_string(current_parent->type, dest_arg);
            if (val.first && current_parent->set_func != nullptr) {
                std::cout<<"Setting stored value in"<<" "<<current_parent->name<<": "<<std::endl;
                current_parent->set_func(val.second);
                current_parent->stored_object = val.second;
                current_parent->stored_object.print_paramval(std::cout);
                std::cout << std::endl;
                }
            else if(val.first){
                std::cout<<"Setting stored value in"<<" "<<current_parent->name<<": "<<std::endl;
                current_parent->stored_object = val.second;
                current_parent->stored_object.print_paramval(std::cout);
                std::cout << std::endl;
            }
        }

        if (equalDot != std::string::npos){
            while (equalDot != std::string::npos){
                parent_name = path_way.substr(0, equalDot);
                children_name = path_way.substr(equalDot + 1);
                for (int i = 0; i < current_parent->children.size(); i++) {
                    if (current_parent->children[i]->name == parent_name) {
                        current_parent = current_parent->children[i].get();
                    }
                equalDot = children_name.find('.');
                path_way = children_name;
                }
            
            for (int i = 0; i < current_parent->children.size(); i++) {
                if (current_parent->children[i]->name == children_name) {
                    current_parent = current_parent->children[i].get();
                }
            }

            std::string dest_arg = arg;
            auto val = ParamValue::make_ParamValue_from_string(current_parent->type, dest_arg);
            if (val.first && current_parent->set_func != nullptr) {
                std::cout<<"Setting stored value in"<<" "<<current_parent->name<<": "<<std::endl;
                current_parent->set_func(val.second);
                current_parent->stored_object = val.second;
                current_parent->stored_object.print_paramval(std::cout);
                std::cout << std::endl;
                }
            }
        }
    }

//Рекурсивная функция разбора строки(спуск вглубь), парсинг параметров и установка значений '='/' '
//Имеется два режима считывания (endl) - флаг, установлен в значение 0 по мере 
//спуска по дереву через разделение '.'
//Установка значения 1 - после нахождения финального листа дерева с нужным именем и присвоения
//ему значения ParamValue 
//На данный момент тип ParamValue определяется автоматически исходя из заданного типа на листе
//Не знаю насколько это верный подход, я также думал про реализацию определения типа через regexp 
void ParserTree::parseParameter(Node* parent, const std::string& param) {

        size_t equalDot = param.find('.');
        bool endl = 0;
        if (equalDot == std::string::npos){
            equalDot = param.find('=');
            endl = 1;
        }
        
        std::string key, value;
        if (equalDot != std::string::npos) {
            key = param.substr(0, equalDot);
            value = param.substr(equalDot + 1);
            if (endl == 0){
                for (int i = 0; i < parent->children.size(); i++) {
                    if (parent->children[i]->name == key){
                        parseParameter(parent->children[i].get(), value);
                    }
                }
            }
            if (endl == 1){
                for (int i = 0; i < parent->children.size(); i++) {
                    if (parent->children[i]->name == key){
                        auto val = ParamValue::make_ParamValue_from_string(parent->children[i]->type, value);
                        if (val.first && parent->children[i]->set_func != nullptr){
                            std::cout<<"Setting stored value in"<<" "<<parent->children[i]->name<<": "<<std::endl;
                            parent->children[i]->set_func(val.second);
                            parent->children[i]->stored_object = val.second;
                            parent->children[i]->stored_object.print_paramval(std::cout);
                            std::cout << std::endl;
                        }
                    else if(val.first){
                            std::cout<<"Setting stored value in"<<" "<<parent->children[i]->name<<": "<<std::endl;
                            parent->children[i]->stored_object = val.second;
                            parent->children[i]->stored_object.print_paramval(std::cout);
                            std::cout << std::endl;
                    }
                    }
                }
            }
        }
    }

    //Перегрузка метода установки значения
void ParserTree::set(const std::string& fieldName, void* obj, GetArgDescrFunc getFunc, ArgDescr::SetValueFunc setFunc, const std::string& description) {
        if (obj == nullptr) {
            throw std::invalid_argument("Incorrect setting of object.");
        }
        internal_AddProxyNode(root.get(), fieldName, obj, getFunc, setFunc, description);
    };

    //Перегрузка метода установки значения
void ParserTree::set(const std::string& fieldName, void* obj, ParamType t, const std::string& description) {
            if (obj == nullptr) {
                throw std::invalid_argument("Incorrect setting of object.");            
            }
        internal_AddSoloLeaf(root.get(), fieldName, obj, t, description);
    };

    //Перегрузка метода установки значения
void ParserTree::set(const std::string& fieldName, const std::string& default_value, ParamType t, const std::string& description) {
            internal_AddInactiveValue(root.get(), fieldName, default_value, t, description);
    };

    // Метод для вывода дерева на экран
void ParserTree::printTree(const std::shared_ptr<Node>& node, const std::string& prefix) {
        if (!node) return;
        std::cout << prefix << node->name << std::endl; 
        //рекурсивный спуск по узлам
        for (const auto& child : node->children) {
            printTree(child, prefix + "  "); // Добавление отступов для вложенных узлов
        }
    };

    // Сам принтинг
void ParserTree::print() {
        printTree(root);
    }

    //Добавление вложенного узла
void ParserTree::addNestedNode(const std::string& key, std::string& value) {
        std::stringstream ss(key);
        std::string segment;
        std::shared_ptr<Node> currentNode = root;
        //сегментируем подаваемую строку через '.', если будет найден соответствующий сегмент среди дочерних узлов - обращаемся к нему/либо пушим новый
        while (std::getline(ss, segment, '.')) {
            auto it = std::find_if(currentNode->children.begin(), currentNode->children.end(),
                 [&segment](const std::shared_ptr<Node>& child) {
                    return child->name == segment;
                });

            if (it != currentNode->children.end()) {
                currentNode = *it; 
            } else {
                auto newNode = std::make_shared<Node>(segment, ParamType::Int, nullptr); //ParamType::Int будет заменен
                currentNode->children.push_back(newNode);
                currentNode = newNode; 
            }
        }
        // Установка объекта в текущий узел
        void* obj = &value;
        currentNode->object = obj;
    }


    //Перегрузка для добавления узла
void ParserTree::addNode(const std::string& fieldName, void* obj, GetArgDescrFunc getFunc, ArgDescr::SetValueFunc setFunc) {
        auto node = std::make_shared<Node>(fieldName, obj);
        root->children.push_back(node);
        auto fields = getFunc(&obj);
        for (const ArgDescr& field : fields) {
            std::string childFieldName = field.paramName;
            addNode(childFieldName, field.nested_object, field.getArgDescr, field.setValue);
        }
    }
    //Добавление финального листа при изначальном конструировании дерева для классического режима с функциями геттерами/сеттерами
void ParserTree::internal_AddLeaf(Node* parent_node, const std::string& fieldName, ParamType type, GetArgDescrFunc getFunc, Node::SetFunc setFunc, const std::string& description){
        auto node = std::make_shared<Node>(fieldName, type, nullptr, setFunc);
        node->description = description;
        node->parent = parent_node;
        parent_node->children.push_back(node);
    } 
    //Добавление промежуточного узла для ветвления дерева
void ParserTree::internal_AddProxyNode(Node* parent_node, const std::string& fieldName, void* obj, GetArgDescrFunc getFunc, ArgDescr::SetValueFunc setFunc, const std::string& description){
        auto node = std::make_shared<Node>(fieldName, obj);
        node->description = description;
        node->parent = parent_node;
        try {
            parent_node->children.push_back(node);
        } catch (const std::exception& e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }
        auto fields = getFunc(obj);
        for (const ArgDescr& field : fields) {
            std::string childFieldName = field.paramName;
            if (field.isValue()){
                auto setVal = [set = setFunc, obj, id = field.id](const ParamValue& value) -> bool{
                    return set(obj, id, value);
                };
                internal_AddLeaf(node.get(), field.paramName, field.paramValue.type, field.getArgDescr, setVal, field.description);
            }
            else{
                internal_AddProxyNode(node.get(), field.paramName, field.nested_object, field.getArgDescr, field.setValue, field.description);
            }
        }
    }
    //Добавление единичного листа с объектом для моментального присоединения листа к корню
void ParserTree::internal_AddSoloLeaf(Node* parent_node, const std::string& fieldName, void* obj, ParamType type, const std::string& desrciption){
        auto node = std::make_shared<Node>(fieldName, obj, type);
        node->parent = parent_node;
        node->description = desrciption;
        auto val = ParamValue(type, obj);
        parent_node->children.push_back(node);
    }
    //Добавление единичного узла ждя ветвления без геттеров/сеттеров при необходимости хранения значения
void ParserTree::internal_AddSoloProxyNode(Node* parent_node, const std::string& fieldName, void* obj, ParamType type, const std::string& description){
        auto node = std::make_shared<Node>(fieldName, obj);
        node->parent = parent_node;
        node->description = description;
        parent_node->children.push_back(node);
        internal_AddSoloLeaf(node.get(), fieldName, obj, type, description);
    }
    //Устанока значения для хранения и не более
void ParserTree::internal_AddInactiveValue(Node* parent_node, const std::string& fieldName, const std::string& val, ParamType type, const std::string& descriprtion){
        auto node = std::make_shared<Node>(fieldName, type, val);
        node->parent = parent_node;
        node->description = descriprtion;
        parent_node->children.push_back(node);
    }

    //Для данной перегрузки тип объекта известен, Template не юзаем
void ParserTree::addNode(const std::string& fieldName, const char& obj, ParamType type) {
        std::string str = std::string(1, obj);
        auto node = std::make_shared<Node>(fieldName, type, str);
        root->children.push_back(node);
    }



