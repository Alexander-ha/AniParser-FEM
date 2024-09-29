# AniParser-FEM
## Implementation of a parser with a tree-like architecture for working on the construction of mathematical models.

This repository allows developers to create their own flexible model configurations and provides an implementation of the parser architecture for these purposes.
Special types realization by discriminated union (Doxygen check).
This library provides both usage of classic compilation arguments input or interpretation.

## Classic parser tree workflow: 
```c++
#include <iostream>
#include <vector>
#include "parser_addon.h"
#include "parser_tree.h"
#include "example_models/sample1_model.h"

int main(int argc, char* argv[]){

    //Creating your model here
    Model m;
    //Instantinating parser
    ParserTree p;

    int scenario = 0;

    //Setting parser nodes
    p.set("model", &m, makeModelGetArgDescr(), setArg, "Coupled model structure"); 
    p.set("scenario", &scenario, ParamType::Int, "Set computation scenario, 0 - slab contraction, 1 - venticular cycle");
    p.set("scenario1", "0", ParamType::Int, "Set computation scenario1, store data in ParserTree only");

    //Printing your parser structure
    p.print();

    //Reading parameters and storing them in your parser
    p.readParamsFromCmdLine(argc, argv, false);

    return 0;
}
```
### Example output:
```
root
  model
    param
    dval
    ss
      x
  scenario
  scenario1
```

## Interpreter module connection example: 
To connect interpreter module:
```c++
#include "interpeter.h"
```
Add module to your code:
```c++
#include <iostream>
#include <vector>
#include "parser_addon.h"
#include <regex>
#include "parser_tree.h"
#include "example_models/sample1_model.h"
#include "interpreter.h"

using namespace std;

int main(int argc, char* argv[]){

    //Creating your model here
    Model m;
    //Instantinating parser
    ParserTree p;

    std::string scenario = "Start value";

    //Setting parser nodes
    p.set("model", &m, makeModelGetArgDescr(), setArg, "Coupled model structure"); 
    p.set("scenario", &scenario, ParamType::String, "Set computation scenario, 0 - slab contraction, 1 - venticular cycle");
    p.set("scenario1", "0", ParamType::Int, "Set computation scenario1, store data in ParserTree only");

    //Printing your parser structure
    p.print();

    Interpreter intp;
    //Reading parameters and storing them in your parser
    
    p.readParamsFromCmdLine(intp.getCommands(), false);


    return 0;
}
```

### Example output:
```bash
root
  model
    param
    dval
    ec
      x
      string_data
      path_to_data
    ef
      one_dim_flow
      is_flow
      efl
        flow
  scenario
  scenario1
Interpreter is connected, input cli(on) to use interpreter mode

```
### cli(on) to use interpreter mode:

```bash
cli(on)
Entering CLI mode. Type 'cli(off)' to exit:
```
### cli(off) to stop interpreter mode:

```bash
cli(off)
Exiting CLI mode.
```
[!NOTE]
Current version of parser provides a few service commands such as:
```bash
--help|-h - help command, displays type and description
```
```bash
--version|-v - parser information and current programm version
```
### Example of help command display:
```bash
Argument name: ss
 - Descripton: Sub Parameter
 - ParamType: Int
Argument name: x
 - Descripton: Some demo parameter x
 - ParamType: Int
Argument name: scenario
 - Descripton: Set computation scenario, 0 - slab contraction, 1 - venticular cycle
 - ParamType: String
Argument name: scenario1
 - Descripton: Set computation scenario1, store data in ParserTree only
 - ParamType: Int
For additional help use --help|-h. 
 To use the interpreter mode, enter: cli(on), 
 if you have it connected, 
 cli(off) to turn it off. 
  --version|-v to get current version
```
_________________________________________________________________________________
## Additional documentation:
🟦: Doxygen docs: docs/html|latex
🟥: How to create model tutorial: docs/HowToCreateModel.md
_________________________________________________________________________________
Contact me here or via Telegram|Gmail:
- alexang2300@gmail.com
- tg: @altergan1
