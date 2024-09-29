#include <iostream>
#include <vector>
#include "parser_addon.h"
#include <regex>
#include "parser_tree.h"
#include "example_models/sample_model.h"
#include "interpreter.h"

using namespace std;

int main(int argc, char* argv[]){

    //Creating your model here
    Sample m;
    //Instantinating parser
    ParserTree p;

    int scenario = 0;

    //Setting parser nodes
    p.set("model", &m, makeSampleGetArgDescr(), setArg, "Coupled model structure"); 
    p.set("scenario", &scenario, ParamType::String, "Set computation scenario, 0 - slab contraction, 1 - venticular cycle");
    p.set("scenario1", "0", ParamType::Int, "Set computation scenario1, store data in ParserTree only");

    //Printing your parser structure
    p.print();

    //Reading parameters and storing them in your parser
    p.readParamsFromCmdLine(argc, argv, false);

    return 0;
}