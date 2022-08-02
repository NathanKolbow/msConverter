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
    std::string newickToMS(std::string newickStr) {
        Network net(newickStr, "newick");
        std::string msString = net.getMSString();
        return msString;
    }

    std::string msToNewick(std::string msStr) {
        Network net(msStr, "ms");
        std::string newick = net.getNewickRepresentation();
        return newick;
    }

    std::vector<std::string> newickFileToMS(std::string location) {
        std::ifstream ifs(location);
        if(!ifs.is_open()) {
            std::cerr << "ERROR: Failed to open file " << location << "; quitting." << std::endl;
            exit(-1);
        }

        std::vector<std::string> msStrs;
        std::string line;

        while(std::getline(ifs, line))
            msStrs.push_back(newickToMS(line));
        
        return msStrs;
    }

    std::string newickToMSSafe(std::string newickStr) {
        std::string msCmd = newickToMS(newickStr);
        std::string msArgs = std::string(msCmd);
        argsOnly(msArgs);
        Network newickNet(newickStr, "newick");
        Network msNet(msArgs, "ms");

        if(!isomorphic(newickNet, msNet))
            throw std::runtime_error("safety check failed: ms did not match Newick after conversion [newickToMSSafe]");
        
        return msCmd;
    }

    std::vector<std::string> newickFileToMSSafe(std::string location) {
        std::ifstream ifs(location);
        if(!ifs.is_open()) {
            std::cerr << "ERROR: Failed to open file " << location << "; quitting." << std::endl;
            exit(-1);
        }

        std::vector<std::string> msStrs;
        std::string line;

        while(std::getline(ifs, line)) {
            std::string msCmd = newickToMS(line);
            std::string msArgs = std::string(msCmd);
            argsOnly(msArgs);

            Network newickNet(line, "newick");
            Network msNet(msArgs, "ms");

            if(!isomorphic(newickNet, msNet))
                throw std::runtime_error("safety check failed: ms did not match Newick after conversion [newickFileToMSSafe]");
            msStrs.push_back(msCmd);
        }
        return msStrs;
    }

    std::string msFromUserInputNewick(void) {
        std::string newick;
        std::cin >> newick;
        Network net(newick, "newick");
        return net.getMSString();
    }

    void disableWarnings(void) {
        disableNetworkWarnings();
    }
}
