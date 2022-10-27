// Copyright (c) 2022 Nathan Kolbow
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), to deal in 
// the Software without restriction, including without limitation the rights to use, 
// copy, modify, merge, publish, distribute, sublicense, and*or sell copies of the 
// Software, and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all 
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "core/Network.hpp"
#include "core/MSEvents.hpp"
#include "tests/test_helpers.hpp"

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <stdexcept>

namespace SimSuite {
    std::string newickToMS(std::string newickStr, int ntrees, bool coalescent2N) {
        Network net(newickStr, "newick");
        std::string msString = net.getMSString(ntrees, coalescent2N);
        return msString;
    }

    std::string msToNewick(std::string msStr) {
        Network net(msStr, "ms");
        std::string newick = net.getNewickRepresentation();
        return newick;
    }

    std::vector<std::string> newickFileToMS(std::string location, int ntrees, bool coalescent2N) {
        std::ifstream ifs(location);
        if(!ifs.is_open()) {
            std::cerr << "ERROR: Failed to open file " << location << "; quitting." << std::endl;
            exit(-1);
        }

        std::vector<std::string> msStrs;
        std::string line;

        while(std::getline(ifs, line))
            msStrs.push_back(newickToMS(line, ntrees, coalescent2N));
        
        return msStrs;
    }

    std::string newickToMSSafe(std::string newickStr, int ntrees, bool coalescent2N) {
        std::string msCmd = newickToMS(newickStr, ntrees, coalescent2N);

        std::string msArgs = std::string(msCmd);
        argsOnly(msArgs);

        // The check here depends on coalescent2N b/c coalescent2N either leaves the time untouched
        // or cuts them in half. If the original times are compared to the halved times then every
        // check fails, obviously...so we have to check based on coalescent2N
        if(!coalescent2N) {
            if(!isomorphicNewick(newickStr, msToNewick(msArgs))) {
                std::cerr << "---DEBUG INFO---\n";
                std::cerr << "Input: " << newickStr << "\n\n";
                std::cerr << "Conversion (false): " << msToNewick(msArgs) << "\n\n";
                std::cerr << "---END DEBUG---\n";
                throw std::runtime_error("safety check failed: ms did not match Newick after conversion [newickToMSSafe]");
            }
        } else {
            std::string msCmdCheck = newickToMS(newickStr, ntrees, false);
            std::string msArgsCheck = std::string(msCmdCheck);
            argsOnly(msArgsCheck);
            if(!isomorphicNewick(newickStr, msToNewick(msArgsCheck))) {
                std::cerr << "---DEBUG INFO---\n";
                std::cerr << "Input: " << newickStr << "\n\n";
                std::cerr << "Conversion (true): " << msToNewick(msArgs) << "\n\n";
                std::cerr << "---END DEBUG---\n";
                throw std::runtime_error("safety check failed: ms did not match Newick after conversion [newickToMSSafe]");
            }
        }
        
        
        return msCmd;
    }

    std::vector<std::string> newickFileToMSSafe(std::string location, int ntrees, bool coalescent2N) {
        std::ifstream ifs(location);
        if(!ifs.is_open()) {
            std::cerr << "ERROR: Failed to open file " << location << "; quitting." << std::endl;
            exit(-1);
        }

        std::vector<std::string> msStrs;
        std::string line;

        while(std::getline(ifs, line)) {
            std::string msCmd = newickToMS(line, ntrees, coalescent2N);
            std::string msArgs = std::string(msCmd);
            argsOnly(msArgs);

            if(!isomorphicNewick(line, msToNewick(msArgs)))
                throw std::runtime_error("safety check failed: ms did not match Newick after conversion [newickFileToMSSafe]");
            msStrs.push_back(msCmd);
        }
        return msStrs;
    }

    void disableWarnings(void) {
        disableNetworkWarnings();
    }
}
