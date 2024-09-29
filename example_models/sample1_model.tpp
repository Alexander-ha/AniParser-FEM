#include <vector>
#include <functional>
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
#include <cassert>
#include "parser_tree.h"

struct EngineVerticalFlow{
    long flow;
};

struct EngineCycle{
    long x;
    std::vector<std::string> string_data;
    std::string path_to_data;
};

struct EngineFlow{
    std::vector<double> one_dim_flow;
    bool is_flow;
    EngineVerticalFlow efl;
};

struct Model{
    int param;
    double dval;
    EngineCycle ec;
    EngineFlow ef;
};


using GetArgDescrFunc = std::function<std::vector<ArgDescr> (void* nested_obj)>;

namespace internals{
    template <>
    struct GenGetArgDescr<EngineVerticalFlow>{
        static GetArgDescrFunc make(){
            return makeEngineVerticalFlowGetArgDescr();
        }
    };
}


namespace internals{
    template <>
    struct GenGetArgDescr<EngineCycle>{
        static GetArgDescrFunc make(){
            return makeEngineCycleGetArgDescr();
        }
    };
}

namespace internals{
    template <>
    struct GenGetArgDescr<EngineFlow>{
        static GetArgDescrFunc make(){
            return makeEngineFlowGetArgDescr();
        }
    };
}

namespace internals{
    template <>
    struct GenGetArgDescr<Model>{
        static GetArgDescrFunc make(){
            return makeModelGetArgDescr();
        }
    };
}


template <typename T>
GetArgDescrFunc create_get_arg_descr(){ return internals::GenGetArgDescr<T>::make(); }



