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


#define BOOST_TEST_MODULE test_isomorphicNewick
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
    int nRand;
    for(Network *net : loadNewickNetworks()) {
        nRand = (int)net->getLeaves().size();
        std::vector<std::string> randomNewicks = net->getRandomNewickRepresentations(nRand);

        for(int i=0; i < nRand-1; i++) {
            for(int j=i+1; j<nRand; j++) {
                BOOST_CHECK(isomorphicNewick(randomNewicks[i], randomNewicks[j]));
            }
        }
        delete net;
    }
}

// isomorphicNewick tests where the networks are obvioiusly not isomorphic
BOOST_AUTO_TEST_CASE(test2) {
    // Obvious falses, comparing totally different networks
    int nRand = 10;
    std::vector<std::string> newicks = loadNewickStrings();
    for(unsigned int n=0; n < newicks.size()-1; n++) {
        std::vector<std::string> randoms1 = Network(newicks[n], "newick").getRandomNewickRepresentations(nRand);
        std::vector<std::string> randoms2 = Network(newicks[n+1], "newick").getRandomNewickRepresentations(nRand);

        for(int i=0; i<nRand; i++) {
            for(int j=0; j<nRand; j++) {
                BOOST_CHECK(!isomorphicNewick(randoms1[i], randoms2[j]));
            }
        }
    }
}

// isomorphicNewick tests comparing pairs of newick strings that ARE isomorphic but have
// previously failed to be seen as isomorphic
BOOST_AUTO_TEST_CASE(test3) {
    std::vector<std::pair<std::string, std::string>> pairs = getNewickStringPairs();
    for(std::pair<std::string, std::string> pair : pairs) {
        std::cerr << pair.first << " vs. " << pair.second << std::endl;
        BOOST_CHECK(isomorphicNewick(pair.first, pair.second));
    }
}