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
#include <iterator>
#include "parser_addon.h"
#include "parser_tree.h"
#include <regex>
#include "sample_model.h"



bool setArgSample(void* obj, int id, const ParamValue& pv) {
    Sample* sample = static_cast<Sample*>(obj);
    assert(dynamic_cast<Sample*>(sample) != nullptr);
    auto& s = *reinterpret_cast<Sample*>(obj);
    switch (id){
        case 0: return pv.assign_if_available<long>(s.param);
        case 1: return pv.get_and_assign(s.dval);
        default: return false;
    }
};

bool setArgSubSample (void* obj, int id, const ParamValue& pv){
    (void) id;
    assert(id == 0 && "Wrong id");
    SubSample* subSample = static_cast<SubSample*>(obj);
    assert(dynamic_cast<SubSample*>(subSample) != nullptr);
    auto& s = *reinterpret_cast<SubSample*>(obj);
    return pv.get_and_assign(s.x);
};


GetArgDescrFunc makeSubSampleGetArgDescr(){
    return [](void* self)->std::vector<ArgDescr>{
        SubSample* subSample = static_cast<SubSample*>(self);
        assert(dynamic_cast<SubSample*>(subSample) != nullptr);
        auto& s = *reinterpret_cast<SubSample*>(self);
        std::vector<ArgDescr> res;
        ArgDescr x_descr{0, "x", ParamValue(s.x), "Some demo parameter x", nullptr, nullptr};
        res.push_back(x_descr);
        return res;
    };
} 

GetArgDescrFunc makeSampleGetArgDescr(){
    return [](void* self)->std::vector<ArgDescr>{
        Sample* sample = static_cast<Sample*>(self);
        assert(dynamic_cast<Sample*>(sample) != nullptr);
        auto& s = *reinterpret_cast<Sample*>(self);
        std::vector<ArgDescr> res;
        res.emplace_back(0, "param", ParamValue(long(s.param)), "Int parameter", nullptr, nullptr);
        res.emplace_back(1, "dval", ParamValue(s.dval), "Double parameter", nullptr, nullptr);
        res.emplace_back(2, "ss", ParamValue(), "Sub Parameter", &s.ss, create_get_arg_descr<decltype(s.ss)>(), setArgSubSample);
        return res;
    };
} 
//Setting args for models
SetArgSampleFunc setArg = setArgSample;

