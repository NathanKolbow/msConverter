#include <boost/program_options.hpp>

#include "SimSuite.hpp"

#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "print ussage message")
        ("file,input", po::value<std::string>(), "input file location")
        ("newick", po::value<std::string>(), "Newick formatted network or tree")
        ("output,o,out", po::value<std::string>(), "output file location")
        ("quiet,no_warn", "do not output warning messages to standard error output")
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
    std::vector<std::string> msArgs;
    if(vm.count("file")) {
        msArgs = SimSuite::newickFileToMS(vm["file"].as<std::string>());
        // std::cout << "File my guy" << std::endl;
    }
    if(vm.count("newick")) {
        msArgs.push_back(SimSuite::newickToMS(vm["newick"].as<std::string>()));
        // std::cout << "Newick my dude" << std::endl;
    }

    // Write the ms arguments
    if(vm.count("output")) {
        std::ofstream output_file(vm["output"].as<std::string>());
        if(!output_file.is_open()) {
            std::cerr << "Failed to open file " << vm["output"].as<std::string>() << "." << std::endl;
            return -1;
        }

        for(std::string line : msArgs) {
            output_file << line << std::endl;
            if(output_file.fail()) {
                std::cerr << "An error occurred while writing to " << vm["output"].as<std::string>() << "." << std::endl;
                return -1;
            }
        }
        return 0;
    } else {
        for(std::string str : msArgs) {
            std::cout << str << std::endl;
        }
        return 0;
    }
}