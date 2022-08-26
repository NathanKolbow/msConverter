#include <boost/program_options.hpp>

#include "SimSuite.hpp"
#include "tests/test_helpers.hpp"

#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <cstdlib>

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
        ("n,ntrees", po::value<unsigned int>(), "number of trees generated in the ms command")
        ("4N,4n", "tells the converter to use ms's default 4N generation coalescent model instead of the standard 2N generation model")
        
        #ifndef _WIN32
        #ifndef WIN32
            ("run","after converting from Newick to ms, runs the ms command and sends the output to the temrinal (Unix/Linux ONLY)")
            ("dirty","--run must be specified to use --dirty. provides output *as is* from ms; not cleaned (Unix/Linux ONLY)")
            ("ms_path",po::value<std::string>(),"path to the ms exectuable. this need not be used if the directory is already in the user's path. must be used with --run (Unix/Linux ONLY)")
        #endif
        #endif
    ;
    po::positional_options_description p;
    p.add("file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    // (po::parse_command_line(argc, argv, desc), vm);
    
    if(vm.count("clean") && !vm.count("run")) {
        std::cout << desc << std::endl;
        std::cout << std::endl << "--run must be specified in order for --clean to be used." << std::endl;
        return 0;
    }

    if(vm.count("args_only") && vm.count("run")) {
        std::cout << "--run and --args_only cannot be used together." << std::endl;
        return 0;
    }

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

    // specify the number of trees generated per ms command
    unsigned int ntrees = 1;
    if(vm.count("n"))
        ntrees = vm["n"].as<unsigned int>();

    // specify whether 2N coalescent units or 4N should be used (defaults to 2N b/c that is the common use case, but `ms` defaults to 4N)
    bool coalescent2N = true;
    if(vm.count("4N"))
        coalescent2N = false;


    // Read and convert the Newick
    std::vector<std::string> msCmds;
    if(vm.count("file")) {
        if(vm.count("safe"))
            msCmds = SimSuite::newickFileToMSSafe(vm["file"].as<std::string>(), ntrees, coalescent2N);
        else
            msCmds = SimSuite::newickFileToMS(vm["file"].as<std::string>(), ntrees, coalescent2N);
    }
    if(vm.count("newick")) {
        if(vm.count("safe"))
            msCmds.push_back(SimSuite::newickToMSSafe(vm["newick"].as<std::string>(), ntrees, coalescent2N));
        else
            msCmds.push_back(SimSuite::newickToMS(vm["newick"].as<std::string>(), ntrees, coalescent2N));
    }

    std::vector<std::string> msResults;
    #ifndef _WIN32
    #ifndef WIN32
    // run the ms commands
    if(vm.count("run")) {
        FILE *fpipe;
        
        std::string cmdAppend = (vm.count("ms_path") ? vm["ms_path"].as<std::string>() : "");
        for(std::string msCmd : msCmds) {
            std::string command = cmdAppend + std::string(msCmd);
            if(0 == (fpipe = (FILE*)popen(command.c_str(), "r"))) {
                perror("Failed to execute ms command.");
                std::cerr << "popen() failed." << std::endl;
                return -1;
            }

            std::string result;
            char line[8192];
            while(fgets(line, 8192, fpipe))
                result += line;
            msResults.push_back(result);
        }
            
    }
    #endif
    #endif

    std::vector<std::string> outputs;
    if(msResults.empty())
        outputs = msCmds;
    else {
        for(std::string result : msResults) {
            unsigned int _start = 0;
            unsigned int _end = 0;
            int newLineCount = 0;

            while(result[_end] != '\0') {
                if(result[_end] == '\n') {
                    newLineCount += 1;
                    
                    // if we want dirty output, push everything; otherwise, clean it
                    if(vm.count("dirty")) {
                        outputs.push_back(result.substr(_start, _end-_start));
                    } else if( newLineCount != 1 &&      // the command itself is always first; skip it
                        newLineCount != 2 &&      // the seed always comes second; skip it
                        _start != _end &&         // skip blank lines
                        result[_start] != '/'     // skip the "//" lines that are included in the output for...some...reason...
                      ) 
                        outputs.push_back(result.substr(_start, _end-_start));

                    _start = _end + 1;
                }

                _end += 1;
            }
        }
    }

    // Write the ms arguments
    if(vm.count("output")) {
        std::ofstream output_file(vm["output"].as<std::string>());
        if(!output_file.is_open()) {
            std::cerr << "Failed to open file " << vm["output"].as<std::string>() << "." << std::endl;
            return -1;
        }

        for(std::string line : outputs) {
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
        // print outputs to terminal
        for(std::string line : outputs) {
            if(vm.count("args_only")) {
                argsOnly(line);
            }

            std::cout << line << std::endl;
        }
        return 0;
    }
}
