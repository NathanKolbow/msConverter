#include <boost/program_options.hpp>

#include "SimSuite.hpp"
#include "tests/test_helpers.hpp"

#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>

namespace po = boost::program_options;

inline void argsOnly(std::string&);

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "print ussage message")
        ("file,input", po::value<std::string>(), "input file location")
        ("newick", po::value<std::string>(), "Newick formatted network or tree")
        ("output,o,out", po::value<std::string>(), "output file location")
        ("quiet,no_warn", "do not output warning messages to standard error output")
        ("args_only", "only output the -ej and -es arguments for the ms command, NOT a fully formed ms command")
        ("safe,safe_mode", "converts all ms back to Newick to ensure accuracy before giving the ms to the user")
    ;
    po::positional_options_description p;
    p.add("file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    // (po::parse_command_line(argc, argv, desc), vm);
    
    if(vm.count("help") || argc == 1) {
        std::cout << desc << std::endl;
        return 0;
    }
    
    if(vm.count("file") == vm.count("newick") && vm.count("file") != 0) {
        std::cout << "Only one of --file or --newick may be specified." << std::endl;
        return -1;
    }
    
    // Check for warning disables
    if(vm.count("quiet")) {
        SimSuite::disableWarnings();
    }

    // Read and convert the Newick
    std::vector<std::string> msCmds;
    if(vm.count("file")) {
        if(vm.count("safe"))
            msCmds = SimSuite::newickFileToMSSafe(vm["file"].as<std::string>());
        else
            msCmds = SimSuite::newickFileToMS(vm["file"].as<std::string>());
    }
    if(vm.count("newick")) {
        if(vm.count("safe"))
            msCmds.push_back(SimSuite::newickToMSSafe(vm["newick"].as<std::string>()));
        else
            msCmds.push_back(SimSuite::newickToMS(vm["newick"].as<std::string>()));
    }

    // Write the ms arguments
    if(vm.count("output")) {
        std::ofstream output_file(vm["output"].as<std::string>());
        if(!output_file.is_open()) {
            std::cerr << "Failed to open file " << vm["output"].as<std::string>() << "." << std::endl;
            return -1;
        }

        for(std::string line : msCmds) {
            if(vm.count("args_only")) {
                argsOnly(line);
            }

            output_file << line << std::endl;
            if(output_file.fail()) {
                std::cerr << "An error occurred while writing to " << vm["output"].as<std::string>() << "." << std::endl;
                return -1;
            }
        }
        return 0;
    } else {
        for(std::string str : msCmds) {
            if(vm.count("args_only")) {
                argsOnly(str);
            }

            std::cout << str << std::endl;
        }
        return 0;
    }
}