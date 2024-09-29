#ifndef SAMPLE_MODEL_H
#define SAMPLE_MODEL_H

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
#include <iterator>
#include "../parser_addon.h"
#include <regex>
#include "../parser_tree.h"

#pragma once
using SetArgSampleFunc = std::function<bool(void*, int, const ParamValue&)>;

bool setArgSubSample (void* obj, int id, const ParamValue& pv);

bool setArgSample(void* obj, int id, const ParamValue& pv); 

GetArgDescrFunc makeSampleGetArgDescr();

GetArgDescrFunc makeSubSampleGetArgDescr();

extern SetArgSampleFunc setArg;
#include "sample_model.tpp"
#endif // SAMPLE_MODEL_H