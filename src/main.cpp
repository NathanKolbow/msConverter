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


#include "SimSuite.hpp"
#include "core/Network.hpp"
#include <iostream>

int main(int narg, char **argv) {
    std::vector<std::string> msStrs = SimSuite::newickFileToMS("newick-strings.txt");
    std::cout << "Getting ms from user input Newick." << std::endl << SimSuite::msFromUserInputNewick() << std::endl;

    /*
    // -----------------------------------------------------------------------
    // - Example content; should be moved to a different file at some point. -
    // -----------------------------------------------------------------------

    // Convert extended Newick to MS
    std::string msString = SimSuite::newickToMS("(15:11.0,(1:10.0,((14:8.0,(((7:2.8,(10:1.6,(9:0.4,8:0.4):1.2):1.2):0.8,(11:2.8,(13:0.4,12:0.4):2.4):0.8):3.4,#H1:0.4::0.3):1.0):1.2,(((2:0.4,3:0.4):5.2,((4:3.6,5:3.6):1.2,6:4.8):0.8):1.0)#H1:2.6::0.7):0.8):1.0);");
    std::cout << msString << std::endl;

    // Some tests by GAB
    // Not sure if this should fail because it lacks the root name h.
    // Also, I've only seen it in use in hybrid-Lambda... do we need it at all?
    std::cout << "GAB: No brlens, no h for root" << std::endl;
    std::string netNoBrlensNoh = SimSuite::newickToMS("((A,B),C);");
    std::cout << netNoBrlensNoh << std::endl << std::endl;

    // This should probably fail because it is not ultrametric and coalescent trees should be
    std::cout << "GAB: No brlens" << std::endl;
    std::string netNoBrlens = SimSuite::newickToMS("((A,B),C)h;");
    std::cout << netNoBrlens << std::endl << std::endl;    

    // Positive simplest test
    std::cout << "GAB: Tinyest example possible, two branches of the same length" << std::endl;
    std::string tinyNet = SimSuite::newickToMS("(A:0.5,B:0.5)h;");
    std::cout << tinyNet << std::endl << std::endl;    

    // A bit larger tree.
    std::cout << "GAB: Three-leaf example, ultrametric" << std::endl;
    std::string threeTaxTree = SimSuite::newickToMS("((A:0.5,B:0.5):0.5,C:1.0)h;");
    std::cout << threeTaxTree << std::endl << std::endl;      

    // Non-ultrametric network
    std::cout << "NAT: Simple three-leaf example, NOT ultrametric" << std::endl;
    std::string threeNonUltra = SimSuite::newickToMS("((1:1.0,#H1:0.2::0.7):1.0,((2:0.8)#H1:0.7::0.3,3:1.0):0.5);");
    std::cout << threeNonUltra << std::endl << std::endl;

    // A true, small network.
    // This one fails
    std::cout << "GAB: Three-leaf network example, ultrametric" << std::endl;
    std::string threeTaxNet = SimSuite::newickToMS("((((A:0.25)#H1:0.25,B:0.5):0.5,#H1:::0.1):0.5,C:1.0);");
    std::cout << threeTaxNet << std::endl << std::endl;  
    */
}
