#include <vector>
#include <functional>
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


struct SubSample{
    long x;
};

struct Sample{
    int param;
    double dval;
    SubSample ss;
};


using GetArgDescrFunc = std::function<std::vector<ArgDescr> (void* nested_obj)>;

namespace internals{
    template <>
    struct GenGetArgDescr<SubSample>{
        static GetArgDescrFunc make(){
            return makeSubSampleGetArgDescr();
        }
    };
}

template <typename T>
GetArgDescrFunc create_get_arg_descr(){ return internals::GenGetArgDescr<T>::make(); }

namespace internals{
    template <>
    struct GenGetArgDescr<Sample>{
        static GetArgDescrFunc make(){
            return makeSampleGetArgDescr();
        }
    };
}

