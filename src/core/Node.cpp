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


#include "Node.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

Node::Node(void) {
    left = right = majorAncestor = minorAncestor = NULL;
    index = 0;
    name = std::string("<blank_name>");
    majorBranchLength = 0;
    minorBranchLength = 0;
    gamma = gammaLft = gammaRht = 0;
    bootSupport = -1;
    time = -1;
}

std::string Node::getNewickFormattedName(bool fromMinorEdge, double gamma) {
    std::stringstream ss;
    ss << name << ":" << (fromMinorEdge ? minorBranchLength : majorBranchLength) << 
                  ":" << (bootSupport != -1 ? std::to_string(bootSupport) : "") <<
                  ":" << (gamma != -1 ? std::to_string(gamma) : "");
    return ss.str();
}

void Node::printInfo(void) {
    if(time != -1)
        std::cout << "\tTime: " << time << std::endl;

    if(getLft() != NULL) {
        std::cout << "\tLeft: " << left->name << " (" << left << ")";
        if(gammaLft != 0)
            std::cout << ", gamma=" << gammaLft << std::endl;
        else
            std::cout << std::endl;
    }


    if(getRht() != NULL) {
        std::cout << "\tRight: " << right->name << " (" << right << ")";
        if(gammaRht != 0)
            std::cout << ", gamma=" << gammaRht << std::endl;
        else
            std::cout << std::endl;
    }
        
    if(getMajorAnc() != NULL)
        std::cout << "\tMajor anc: " << majorAncestor->name << ":" << majorBranchLength << std::endl;
    if(getMinorAnc() != NULL)
        std::cout << "\tMinor anc: " << minorAncestor->name << ":" << minorBranchLength << std::endl;
    
    if(bootSupport != -1)
        std::cout << "\tBoot supp: " << bootSupport << std::endl;
}