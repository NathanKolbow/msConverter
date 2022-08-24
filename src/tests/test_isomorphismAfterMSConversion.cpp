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


#define BOOST_TEST_MODULE test_isomorphismAfetrMSConversion
#include <boost/test/unit_test.hpp>
#include <boost/test/debug.hpp>

#include "../SimSuite.hpp"
#include "test_helpers.hpp"

#include <vector>
#include <string>
#include <stdexcept>

namespace ss = SimSuite;

BOOST_AUTO_TEST_CASE(test1) {
    std::vector<std::string> newicks = loadNewickStrings();

    // convert the newicks to ms, then back to newick
    for(std::string newick : newicks) {
        std::string msArgs = ss::newickToMS(newick, 1, false);
        argsOnly(msArgs);
        
        std::string convNewick = ss::msToNewick(msArgs);
        
        std::cerr << "Testing " << newick << " vs. " << convNewick << std::endl;

        BOOST_CHECK(isomorphicNewick(newick, convNewick));
    }
}