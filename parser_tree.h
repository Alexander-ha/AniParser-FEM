#ifndef PARSER_TREE_H
#define PARSER_TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
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
#include <regex>

class ParamValue;
class Node;

/*!
	\brief Enumerating discriminated union types, ParamType
	\version 0.1
	\date Sept. 2024

	ParamType is an implementation of usable types in our parser
*/
enum class ParamType {
    Int, Double, String, Bool, IntArray, DoubleArray, StringArray, BoolArray
};

/*!
Transfrom ParamType to std::string
 \param[in] t initial ParamType value
 \return std::string value
*/
std::string param_type_to_string(ParamType t);

/*!
Transfrom ParamType from std::string
 \param[in] s initial string value
\return ParamType value
*/
ParamType param_type_from_string(const std::string& s);

/*!
 istream operator
*/
template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& v);

/*!
 ostream operator
*/
template<class T>
std::ostream& operator<<(std::ostream& os, std::vector<T>& v);

/*!
	\brief Implementation of ParamValue and union - Multivalue
*/
struct ParamValue {
    /**
     * @brief Type for constructing your value
     */
    ParamType type;
    union MultiValue{
        /**
        * @brief long value
        */
        long intValue;
        /**
        * @brief double value
        */        
        double doubleValue;
        /**
        * @brief std::string value
        */
        std::string stringValue;
        /**
        * @brief bool value
        */
        bool boolValue;
        /**
        * @brief int array value
        */
        std::vector<long> intArrayValue;
        /**
        * @brief double array value
        */
        std::vector<double> doubleArrayValue;
        /**
        * @brief std::vector<std::string> string array value
        */
        std::vector<std::string> stringArrayValue;
        /**
        * @brief std::vector<bool> bool array value
        */
        std::vector<bool> boolArrayValue;
        /**
        * @brief MutliValue initial constructore
        */
        MultiValue(){ new(&intValue) long(0); }
        /**
        * @brief MultiValue initial destructor
        */
        ~MultiValue(){ }
    };
    MultiValue value;

    /*!
	    \brief ParamValue initial constructor
    */
    ParamValue() : type(ParamType::Int){ value.intValue = 0; }

    /*!
	    \brief ParamValue constructors
    */
    ParamValue(ParamType type, const void* _value);
    ParamValue(ParamType type);
    explicit ParamValue(long _value);
    explicit ParamValue(double _value);
    explicit ParamValue(const std::string& _value);
    explicit ParamValue(bool _value);
    explicit ParamValue(std::vector<long> _value);
    explicit ParamValue(std::vector<std::string> _value);
    explicit ParamValue(std::vector<bool> _value);
    explicit ParamValue(std::vector<double> _value);
    ParamValue(const ParamValue& other);
    ParamValue(ParamValue&& other);



    /*!
     Assignment operator
    */    
    ParamValue& operator=(const ParamValue& other);
    ParamValue& operator=(ParamValue&& other);

    /*!
     Setter example
    */    
    ParamValue& set(long _value);
    ParamValue& set(double _value);
    ParamValue& set(const std::string& _value);
    ParamValue& set(bool _value);
    ParamValue& set(std::vector<long> _value);
    ParamValue& set(std::vector<std::string> _value);
    ParamValue& set(std::vector<bool> _value);
    ParamValue& set(std::vector<double> _value);

    /*!
     Getter example
    */    
    long& Integer();
    double& Real();
    std::string& String();
    bool& Bool();
    std::vector<long>& IntArray();
    std::vector<std::string>& StringArray();
    std::vector<bool>& BoolArray();
    std::vector<double>& DoubleArray();

    long Integer() const;
    double Real() const;
    std::string String() const;
    bool Bool() const;
    std::vector<long>& IntArray() const;
    std::vector<std::string>& StringArray() const;
    std::vector<bool>& BoolArray() const;
    std::vector<double>& DoubleArray() const;
    static std::pair<bool, ParamValue> make_ParamValue_from_string(ParamType type, std::string& str);

    /*!
     Getter by copy
    */    
    template<typename T> T  get() const;
    /*!
     Getter by modifying val state
    */    
    template<typename T> T& get();
    /*!
     Retreive value and assign it, check success
    \param[out] out
    */    
    template<typename T> bool get_and_assign(T& out) const;
    /*!
     Retreive value and check availability
    \param[in] (Type, val)
    */    
    template<typename T> std::pair<T, bool> get_if_available() const;
    /*!
     Get and assign to a different type
    \param[in] BaseType param
    \param[out] ToType param
    */        
    template<typename BaseType, typename ToType> bool assign_if_available(ToType& out) const;
    /*!
     Copy paramvalue
    \param[in] from
    \param[out] out
    */        
    static void copy_value(const ParamValue& from, void* out);
    /*!
     Print paramvalue
     \param[in] from
     \param[out] out
     Example:
    \code
        parent->children[i]->stored_object.print_paramval(std::cout);
        std::cout << std::endl;
    \endcode
    */   
    std::ostream& print_paramval(std::ostream& out);

    /*!
     Destructor
    */         
    ~ParamValue();

private: 
    void destroy_multivalue();
    void copy_multivalue(const MultiValue& other);
    void move_copy_multivalue(MultiValue&& other);
    std::ostream& print_multivalue(std::ostream& out);

    template<typename T = long, bool Dummy = true>    
    struct GetInternals{
        // static void get(const ParamValue&);
    };

    template<bool Dummy>
    struct GetInternals<long, Dummy>{
        static bool is_type_ok(const ParamValue& v) { return v.type == ParamType::Int; }
        static long get(const ParamValue& v) { return v.Integer(); }
        static long& get(ParamValue& v) { return v.Integer(); }
    };
    template<bool Dummy>
    struct GetInternals<bool, Dummy>{
        static bool is_type_ok(const ParamValue& v) { return v.type == ParamType::Bool; }
        static bool get(const ParamValue& v) { return v.Bool(); }
        static bool& get(ParamValue& v) { return v.Bool(); }
    };
    template<bool Dummy>
    struct GetInternals<std::string, Dummy>{
        static bool is_type_ok(const ParamValue& v) { return v.type == ParamType::String; }
        static std::string get(const ParamValue& v) { return v.String(); }
        static std::string& get(ParamValue& v) { return v.String(); }
    };
    template<bool Dummy>
    struct GetInternals<double, Dummy>{
        static bool is_type_ok(const ParamValue& v) { return v.type == ParamType::Double; }
        static double get(const ParamValue& v) { return v.Real(); }
        static double& get(ParamValue& v) { return v.Real(); }
    };
    template<bool Dummy>
    struct GetInternals<std::vector<long>, Dummy>{
        static bool is_type_ok(const ParamValue& v) { return v.type == ParamType::IntArray; }
        static std::vector<long> get(const ParamValue& v) { return v.IntArray(); }
        static std::vector<long>& get(ParamValue& v) { return v.IntArray(); }
    };
    template<bool Dummy>
    struct GetInternals<std::vector<std::string>, Dummy>{
        static bool is_type_ok(const ParamValue& v) { return v.type == ParamType::StringArray; }
        static std::vector<std::string> get(const ParamValue& v) { return v.StringArray(); }
        static std::vector<std::string>& get(ParamValue& v) { return v.StringArray(); }
    };
    template<bool Dummy>
    struct GetInternals<std::vector<bool>, Dummy>{
        static bool is_type_ok(const ParamValue& v) { return v.type == ParamType::BoolArray; }
        static std::vector<bool> get(const ParamValue& v) { return v.BoolArray(); }
        static std::vector<bool>& get(ParamValue& v) { return v.BoolArray(); }
    };
    template<bool Dummy>
    struct GetInternals<std::vector<double>, Dummy>{
        static bool is_type_ok(const ParamValue& v) { return v.type == ParamType::DoubleArray; }
        static std::vector<double> get(const ParamValue& v) { return v.DoubleArray(); }
        static std::vector<double>& get(ParamValue& v) { return v.DoubleArray(); }
    };
};
/*!
	\brief Main struct to describe your arguments
*/
struct ArgDescr{
    /**
    * @brief Initial setter
    */
    using SetValueFunc = std::function<bool(void*, int, const ParamValue&)>;
    /**
    * @brief Parameter id
    */
    int id;
    /**
    * @brief Parameter name
    */
    std::string paramName;
    /**
    * @brief Parameter value. Type - ParamType::
    */
    ParamValue paramValue;
    /**
    * @brief Parameter description.
    */
    std::string description;

    /**
    * @brief Nested group or object pointer
    */
    void* nested_object = nullptr;
    /**
    * @brief Getter arg descr
    */
    std::function<std::vector<ArgDescr> (void* nested_obj)> getArgDescr = nullptr;
    /**
    * @brief Setter arg descr
    */
    std::function<bool(void*, int, const ParamValue&)> setValue = nullptr;
    /**
    * @brief Method to check if its valure or nested object
    */
    bool isValue() const {return nested_object == nullptr; };
    /**
    * @brief Default constructor
    */
    ArgDescr() = default;
    ArgDescr(int id, std::string paramName, ParamValue paramValue, std::string description, void* nested_object = nullptr, std::function<std::vector<ArgDescr> (void* nested_obj)> getArgDescr = nullptr, std::function<bool(void*, int, const ParamValue&)> setValue = nullptr):
        id{id}, paramName{std::move(paramName)}, paramValue{std::move(paramValue)}, description{std::move(description)}, nested_object{nested_object}, getArgDescr{std::move(getArgDescr)}, setValue(std::move(setValue)) {}
};

/*
 Alias getter
*/     
using GetArgDescrFunc = std::function<std::vector<ArgDescr> (void* nested_obj)>;


template<typename T> T  ParamValue::get() const { return GetInternals<T, true>::get(*this); }
template<typename T> T& ParamValue::get()       { return GetInternals<T, true>::get(*this); }
template<typename T> bool ParamValue::get_and_assign(T& out) const { return GetInternals<T, true>::is_type_ok(*this) ? (out = get<T>(), true) : false; }
template<typename T> std::pair<T, bool> ParamValue::get_if_available() const { return GetInternals<T, true>::is_type_ok(*this) ? std::pair<T, bool>{get<T>(), true} : std::pair<T, bool>{T(), false}; }
template<typename BaseType, typename ToType> bool ParamValue::assign_if_available(ToType& out) const { return GetInternals<BaseType, true>::is_type_ok(*this) ? (out = get<BaseType>(), true) : false; }

/*!
	\brief Parser tree class

    Parser tree implementation consists of class Node
*/
class ParserTree
{
public:
    struct Node {
        using SetFunc = std::function<bool(const ParamValue& value)>;
        /**
        * @brief Node name
        */
        std::string name;
        /**
        * @brief Param type if node is leaf of consist stored value
        */
        ParamType type;
        /**
        * @brief Stored paramval
        */
        ParamValue stored_object;
        /**
        * @brief Data description
        */
        std::string description;
        /**
        * @brief Nested object pointer
        */
        void* object = nullptr; 
        /**
        * @brief Setter 
        */
        SetFunc set_func = nullptr;
        std::string init_str;
        /**
        * @brief Children nodes 
        */
        std::vector<std::shared_ptr<Node>> children;
        /**
        * @brief Parent node
        */
        Node* parent;

        /**
        * @brief Available constructor
        */
        Node(const std::string& name, ParamType type, std::nullptr_t obj)
            : name(name), type(type), object(obj) {}
        /**
        * @brief Available constructor
        */
        Node(const std::string& fieldName, ParamType paramType, void* obj, SetFunc set_Func = nullptr)
            : name(fieldName), type(paramType), object(obj), set_func(set_Func) {}
        /**
        * @brief Available constructor
        */
        Node(const std::string& fieldName, void* obj)
            : name(fieldName), object(obj) {}
        /**
        * @brief Available constructor
        */
        Node(const std::string& fieldName, ParamType paramType, std::string default_value)
            : name(fieldName), type(paramType), init_str(default_value) {}
        /**
        * @brief Available constructor
        */
        Node(const std::string& fieldName, void* obj, ParamType paramType)
            : name(fieldName), object(obj), type(paramType){}

    };

    std::map<std::string, Node*> vertexMap;
    std::map<std::pair<std::string, std::string>, std::vector<std::string>> adjList;

public:
    /**
    * @brief Root of the tree
    */    
    std::shared_ptr<Node> root;
    ParserTree() : root(std::make_shared<Node>("root", nullptr)){};

    ParserTree(int argc, char* argv[], bool print_messages);
    /*!
     Read params from command line
    \param[in] arc - size of argv
    \param[in] argv[] - array of char pointers to args
    \param[in] print_messages - bool to display or not messages
    */            
    void readParamsFromCmdLine(int argc, char* argv[], bool print_messages);
    /*!
     @warning This is an Interpreter implementation
     Read params from command line
    \param[in] arv - vector of strings, often - interpreter output
    \param[in] print_messages - bool to display or not messages
    */            
    void readParamsFromCmdLine(const std::vector<std::string>& argv, bool print_messages);
    /*!
     Print tree to the ouput
    \param[in] node - start node
    \param[in] prefix - using prefix for print
    */    
    void printTree(const std::shared_ptr<Node>& node, const std::string& prefix = "");
    /*!
     Call printTree.
    */ 
    void print();
    /*!
     Set branch by model
    \param[in] fieldName - name of model or any other constructions
    \param[in] obj - current object of model or anything else
    \param[in] setFunc - Setter func ArgDescr
    \param[in] getFunc - Getter func ArgDescr
    \param[in] description - initial description
    */    
    void set(const std::string& fieldName, void* obj, GetArgDescrFunc getFunc, ArgDescr::SetValueFunc setFunc, const std::string& description = "");
    /*!
     Set branch by certain paramtype
    \param[in] fieldName - name of model or any other constructions
    \param[in] obj - current object of model or anything else
    \param[in] t - ParamType definition
    \param[in] description - initial description
    */    
    void set(const std::string& fieldName, void* obj, ParamType t, const std::string& description);
    /*!
     Set leaf by value string
    \param[in] fieldName - value name
    \param[in] default_value - value
    \param[in] t - ParamType definition
    \param[in] description - initial description
    */    
    void set(const std::string& fieldName, const std::string& default_value, ParamType t, const std::string& description);

private:
    void internal_AddProxyNode(Node* parent_node, const std::string& fieldName, void* obj, GetArgDescrFunc getFunc, ArgDescr::SetValueFunc setFunc, const std::string& description);
    void internal_AddSoloLeaf(Node* parent_node, const std::string& fieldName, void* obj, ParamType type, const std::string& description);
    void internal_AddSoloProxyNode(Node* parent_node, const std::string& fieldName, void* obj, ParamType type, const std::string& description);
    void internal_AddInactiveValue(Node* parent_node, const std::string& fieldName, const std::string& val, ParamType type, const std::string& descriprtion);
    void internal_AddLeaf(Node* parent_node, const std::string& fieldName, ParamType type, GetArgDescrFunc getFunc, Node::SetFunc setFunc, const std::string& description);

    void internal_AddNode(Node* parent_node, const std::string& fieldName, void* obj, ParamType type, const std::string& description);

    void addNode(const std::string& fieldName, const char& obj, ParamType type);

    template <typename T>
    void addNode(const std::string& fieldName, T* obj, ParamType type);

    void addNode(const std::string& fieldName, void* obj, ParamType type);

    void addNode(const std::string& fieldName, void* obj, GetArgDescrFunc getFunc, ArgDescr::SetValueFunc setFunc);

    void addNestedNode(const std::string& key, std::string& value);

    void internal_AddLeaf(Node* parent_node, const std::string& fieldName, ParamType type, GetArgDescrFunc getFunc, ArgDescr::SetValueFunc setFunc, const std::string& description);

    void internal_AddNode(Node* parent_node, const std::string& fieldName, void* obj, GetArgDescrFunc getFunc, ArgDescr::SetValueFunc setFunc, const std::string& description);

    void parseParameter(Node* parent, const std::string& param);

    void descentDefine(Node* parent, const std::string& path, const std::string& arg);

    void parseParameterByPair(const std::pair<std::string, std::string>& input_pair);
    
};

#include "parser_tree.tpp"
#endif // PARSER_TREE_H
