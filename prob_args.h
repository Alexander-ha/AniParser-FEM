#include <sys/stat.h>
#include <numeric>
#include <iostream>

// Class for parsing command line args
struct InputArgs{
    unsigned axis_sizes = 8;
    std::string lin_sol_db = "";
    std::string save_dir = "./", save_prefix = "problem_out";
    std::string lin_sol_nm = "inner_mptiluc", lin_sol_prefix = "";
    uint max_quad_order = 2;

    virtual uint parseArg(int argc, char* argv[], bool print_messages = true);
    virtual void print(std::ostream& out = std::cout, const std::string& prefix = "") const;

    friend std::ostream& operator<<(std::ostream& out, const InputArgs& p);
    void parseArgs(int argc, char* argv[], bool print_messages = true);

protected:
    virtual void printArgsDescr(std::ostream& out = std::cout, const std::string& prefix = "");
    virtual void setup_args();
    void printArgsHelpMessage(std::ostream& out = std::cout, const std::string& prefix = "");
};

inline std::ostream& operator<<(std::ostream& out, const InputArgs& p){ return p.print(out), out; }