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


// This file is included strictly for the purpose of eliminating memory leaks.
// Tests are not actually performed, but basic functions and con/destructors are
// run to check for memory leaks.

#define BOOST_TEST_MODULE test_makeUltrametric
#include <boost/test/unit_test.hpp>
#include <boost/test/debug.hpp>

#include "../core/Network.hpp"
#include <string>

struct GlobalFixture {
    GlobalFixture() {
        boost::debug::detect_memory_leaks(true);
        disableNetworkWarnings();
    }
    ~GlobalFixture() { }
};
BOOST_GLOBAL_FIXTURE(GlobalFixture);


std::string NEWICK = "(15:11.0,(1:10.0,((14:8.0,(((7:2.8,(10:1.6,(9:0.4,8:0.4):1.2):1.2):0.8,(11:2.8,(13:0.4,12:0.4):2.4):0.8):3.4,#H1:0.4::0.3):1.0):1.2,(((2:0.4,3:0.4):5.2,((4:3.6,5:3.6):1.2,6:4.8):0.8):1.0)#H1:2.6::0.7):0.8):1.0);";

BOOST_AUTO_TEST_CASE(test1) {
    Network net(NEWICK, "newick");

    Network *net2 = new Network(NEWICK, "newick");
    delete net2;
}

BOOST_AUTO_TEST_CASE(test2) {
    isomorphicNewick(NEWICK, NEWICK);
}

BOOST_AUTO_TEST_CASE(test3) {
    Network net(NEWICK, "newick");
    Network *net2 = new Network(NEWICK, "newick");

    net.makeUltrametric();
    net2->makeUltrametric();

    delete net2;
}

BOOST_AUTO_TEST_CASE(test4) {
    Network net(NEWICK, "newick");
    net.getRandomNewickRepresentations(1000);
}