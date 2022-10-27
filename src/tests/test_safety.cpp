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


#define BOOST_TEST_MODULE test_safety
#include <boost/test/unit_test.hpp>
#include <boost/test/debug.hpp>


#define SIMSUITE_NETWORK_SUPRESS_WARNINGS
#include "../core/Network.hpp"
#include "../SimSuite.hpp"
#include "test_helpers.hpp"

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>


struct GlobalFixture {
    GlobalFixture() {
        boost::debug::detect_memory_leaks(true);
        disableNetworkWarnings();
    }
    ~GlobalFixture() { }
};
BOOST_GLOBAL_FIXTURE(GlobalFixture);

// isomorphicNewick tests where the networks are truly isomorphic
BOOST_AUTO_TEST_CASE(test1) {
    for(std::string newick : loadNewickStrings()) {
        SimSuite::newickToMSSafe(newick, 1, false);
    }
}