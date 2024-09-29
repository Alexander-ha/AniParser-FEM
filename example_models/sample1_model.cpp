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
#include "parser_tree.h"
#include <regex>
#include "sample1_model.h"

//Creating setters for our model structure
// assign_if_available - usage for a base types 
// get_and_assign - usage for getting value and assign it to ParamValue and appropriate ParamType

bool setArgModel(void* obj, int id, const ParamValue& pv) {
    Model* model = static_cast<Model*>(obj);
    assert(dynamic_cast<Model*>(model) != nullptr);
    auto& s = *reinterpret_cast<Model*>(obj);
    switch (id){
        case 0: return pv.assign_if_available<long>(s.param);
        case 1: return pv.get_and_assign(s.dval);
        default: return false;
    }
};

bool setArgEngineCycle (void* obj, int id, const ParamValue& pv){
    (void) id;
    assert(id == 0 && "Wrong id");
    EngineCycle* engCycle = static_cast<EngineCycle*>(obj);
    assert(dynamic_cast<EngineCycle*>(engCycle) != nullptr);
    auto& s = *reinterpret_cast<EngineCycle*>(obj);
    switch(id){
        case 0: return pv.assign_if_available<long>(s.x);
        case 1: return pv.get_and_assign(s.string_data);
        case 2: return pv.get_and_assign(s.path_to_data);
        default: return false;
    }
};

bool setArgEngineVerticalFlow (void* obj, int id, const ParamValue& pv){
    (void) id;
    assert(id == 0 && "Wrong id");
    EngineVerticalFlow* engVFlow = static_cast<EngineVerticalFlow*>(obj);
    assert(dynamic_cast<EngineVerticalFlow*>(engVFlow) != nullptr);
    auto& s = *reinterpret_cast<EngineVerticalFlow*>(obj);
    return pv.get_and_assign(s.flow);
};

bool setArgEngineFlow (void* obj, int id, const ParamValue& pv){
    (void) id;
    assert(id == 0 && "Wrong id");
    EngineFlow* engFlow = static_cast<EngineFlow*>(obj);
    assert(dynamic_cast<EngineFlow*>(engFlow) != nullptr);
    auto& s = *reinterpret_cast<EngineFlow*>(obj);
    switch(id){
        case 0: return pv.get_and_assign(s.one_dim_flow);
        case 1: return pv.assign_if_available<bool>(s.is_flow);
        default: return false;
    }
};


//Creating getters for models 

GetArgDescrFunc makeModelGetArgDescr(){
    return [](void* self)->std::vector<ArgDescr>{
        Model* model = static_cast<Model*>(self);
        assert(dynamic_cast<Model*>(model) != nullptr);
        auto& s = *reinterpret_cast<Model*>(self);
        std::vector<ArgDescr> res;
        res.emplace_back(0, "param", ParamValue(long(s.param)), "Int parameter", nullptr, nullptr);
        res.emplace_back(1, "dval", ParamValue(s.dval), "Double parameter", nullptr, nullptr);
        res.emplace_back(2, "ec", ParamValue(), "Engine Cycle mode", &s.ec, create_get_arg_descr<decltype(s.ec)>(), setArgEngineCycle);
        res.emplace_back(3, "ef", ParamValue(), "Engine Flow data info", &s.ef, create_get_arg_descr<decltype(s.ef)>(), setArgEngineFlow);
        return res;
    };
};

GetArgDescrFunc makeEngineFlowGetArgDescr(){
    return [](void* self)->std::vector<ArgDescr>{
        EngineFlow* engineFlow = static_cast<EngineFlow*>(self);
        assert(dynamic_cast<EngineFlow*>(engineFlow) != nullptr);
        auto& s = *reinterpret_cast<EngineFlow*>(self);
        std::vector<ArgDescr> res;
        res.emplace_back(0, "one_dim_flow", ParamValue(s.one_dim_flow), "Int parameter", nullptr, nullptr);
        res.emplace_back(1, "is_flow", ParamValue(s.is_flow), "Double parameter", nullptr, nullptr);
        res.emplace_back(2, "efl", ParamValue(), "Engine Cycle mode", &s.efl, create_get_arg_descr<decltype(s.efl)>(), setArgEngineVerticalFlow);
        return res;
    };
};

GetArgDescrFunc makeEngineCycleGetArgDescr(){
    return [](void* self)->std::vector<ArgDescr>{
        EngineCycle* subSample = static_cast<EngineCycle*>(self);
        assert(dynamic_cast<EngineCycle*>(subSample) != nullptr);
        auto& s = *reinterpret_cast<EngineCycle*>(self);
        std::vector<ArgDescr> res;
        res.emplace_back(0, "x", ParamValue(long(s.x)), "Demo x cycle mode", nullptr, nullptr);
        res.emplace_back(1, "string_data", ParamValue(s.string_data), "String data of cycle", nullptr, nullptr);
        res.emplace_back(2, "path_to_data", ParamValue(s.path_to_data), "Path to cycle mesh", nullptr, nullptr);
        return res;
    };

};

GetArgDescrFunc makeEngineVerticalFlowGetArgDescr(){
    return [](void* self)->std::vector<ArgDescr>{
        EngineVerticalFlow* engVFlow = static_cast<EngineVerticalFlow*>(self);
        assert(dynamic_cast<EngineVerticalFlow*>(engVFlow) != nullptr);
        auto& s = *reinterpret_cast<EngineVerticalFlow*>(self);
        std::vector<ArgDescr> res;
        ArgDescr x_descr{0, "flow", ParamValue(s.flow), "Flow data through engine fuel channel", nullptr, nullptr};
        res.push_back(x_descr);
        return res;
    };
};

// Define setArg
SetArgModelFunc setArg = setArgModel;

