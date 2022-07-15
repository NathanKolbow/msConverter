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


#include "Network.hpp"
#include "MSEvents.hpp"
#include <iostream>

int main(int narg, char *argv[]) {
    // Check for Newick string equivalence
    std::string n1 = std::string("((A:0.6,(B:0.5,#H1:0.1::0.6):0.1):0.2,((C:0.4)#H1:0.3::0.4,D:0.7):0.1);");
    std::string n2 = std::string("((A:0.6,(B:0.5,(C:0.4)#H1:0.1::0.6):0.1):0.2,(#H1:0.3::0.4,D:0.7):0.1);");
    std::string n3 = std::string("((1:0.1,((2:0.2,(3:0.3,(4:0.4)Y#H1:0.5)g:0.6)e:0.7,(((Y#H1:0.8,5:0.9)h:1.0,6:1.1)f:1.2)X#H2:1.3)c:1.4)a:1.5,((X#H2:1.6,7:1.7)d:1.8,8:1.9)b:2.0)r;");

    std::cout << "Building net1" << std::endl;
    Network net1(n1, "newick");
    std::cout << "Building net2" << std::endl;
    Network net2(n2, "newick");

    // std::vector<std::string> newicks = net3.getRandomNewickRepresentations(5);
    // std::cout << "\n\nRandom equivalent Newick representations:\n";
    // for(std::string str : newicks)
    //     std::cout << str << std::endl;

    return 0;
}