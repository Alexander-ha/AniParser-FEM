#ifndef SAMPLE1_MODEL_H
#define SAMPLE1_MODEL_H

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
#include <iterator>
#include "../parser_addon.h"
#include <regex>
#include "../parser_tree.h"

#pragma once

using SetArgModelFunc = std::function<bool(void*, int, const ParamValue&)>;

bool setArgEngineVerticalFlow (void* obj, int id, const ParamValue& pv); 

bool setArgEngineFlow (void* obj, int id, const ParamValue& pv); 

bool setArgEngineCycle (void* obj, int id, const ParamValue& pv);

bool setArgModel(void* obj, int id, const ParamValue& pv); 

GetArgDescrFunc makeModelGetArgDescr();
GetArgDescrFunc makeEngineFlowGetArgDescr();
GetArgDescrFunc makeEngineCycleGetArgDescr();
GetArgDescrFunc makeEngineVerticalFlowGetArgDescr();

extern SetArgModelFunc setArg;
#include "sample1_model.tpp"
#endif // SAMPLE_MODEL_H