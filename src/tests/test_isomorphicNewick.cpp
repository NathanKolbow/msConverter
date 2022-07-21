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

std::vector<std::string> ALL_NEWICKS{
    std::string("((1,((2,(3,(4)Y#H1)g)e,(((Y#H1,5)h,6)f)X#H2)c)a,((X#H2,7)d,8)b)r;"),
    std::string("(((((1,#H1),((2)#H1,3)),(4)#H2),(#H2,5)),6);"),
    std::string("((#H2:0.5::0.2,4:2.0):0.5,((1:1.0,(2:0.5)#H1:0.5::0.9):1.0,((#H1:0.5::0.1,3:1.0):0.5)#H2:0.5::0.8):0.5);"),
    std::string("(15,(1,((14,(#H1,(((12,13),(11,#H3)),(7,((10)#H3,(8,9)))))),((((2,3))#H2,(6,(5,(#H2,4)))))#H1)));"),
    std::string("(10,(#H2,(1,(2,(((9)#H1,(3,(4,((5,6),(7,(8,#H1)))))))#H2))));"),
    std::string("((((1,2),((3,4))#H1),(#H1,5)),6);"),
    std::string("(15:11.0,(1:10.0,((14:8.0,(((7:2.8,(10:1.6,(9:0.4,8:0.4):1.2):1.2):0.8,(11:2.8,(13:0.4,12:0.4):2.4):0.8):3.4,#H1:0.4::0.3):1.0):1.2,(((2:0.4,3:0.4):5.2,((4:3.6,5:3.6):1.2,6:4.8):0.8):1.0)#H1:2.6::0.7):0.8):1.0);")
};

// isomorphicNewick tests where the networks are truly isomorphic
BOOST_AUTO_TEST_CASE(test1) {
    int nRand = 10;
    for(std::string newick : ALL_NEWICKS) {
        Network net(newick, "newick");
        std::vector<std::string> randomNewicks = net.getRandomNewickRepresentations(nRand);

        for(int i=0; i < nRand-1; i++) {
            for(int j=i+1; j<nRand; j++) {
                BOOST_CHECK(isomorphicNewick(randomNewicks[i], randomNewicks[j]));
            }
        }
    }
}

// isomorphicNewick tests where the networks are obvioiusly not isomorphic
BOOST_AUTO_TEST_CASE(test2) {
    // Obvious falses, comparing totally different networks
    int nRand = 10;
    for(unsigned int n=0; n < ALL_NEWICKS.size()-1; n++) {
        std::vector<std::string> randoms1 = Network(ALL_NEWICKS[n], "newick").getRandomNewickRepresentations(nRand);
        std::vector<std::string> randoms2 = Network(ALL_NEWICKS[n+1], "newick").getRandomNewickRepresentations(nRand);

        for(int i=0; i<nRand; i++) {
            for(int j=0; j<nRand; j++) {
                BOOST_CHECK(!isomorphicNewick(randoms1[i], randoms2[j]));
            }
        }
    }
}