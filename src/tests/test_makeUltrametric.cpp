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


#define BOOST_TEST_MODULE test_makeUltrametric
#include <boost/test/unit_test.hpp>
#include <boost/test/debug.hpp>

#include "../core/Network.hpp"
#include "../core/Node.hpp"
#include "test_helpers.hpp"

#include <iostream>
#include <fstream>
#include <stack>
#include <map>

struct GlobalFixture {
    GlobalFixture() {
        boost::debug::detect_memory_leaks(true);
        disableNetworkWarnings();
    }
    ~GlobalFixture() { }
};
BOOST_GLOBAL_FIXTURE(GlobalFixture);

BOOST_AUTO_TEST_CASE(test1, *boost::unit_test::tolerance(1e-9)) {
    // for each network in newick-strings.txt:
    // 1. load the network
    // 2. run ::makeUltrametric
    // 3. check that all the `leaf->getTime()`'s match
    // 4. more rigorously, traverse the network from leaves to root via branches and 
    //    check EVERY SINGLE NODE for consistent times
    std::vector<Network*> newicks = loadNewickNetworks();

    for(Network *net : newicks) {
        // check all leaf times
        std::vector<Node*> leaves;
        double endTime = -1;
        for(Node *p : net->getNodes()) {
            if(p->getLft() == p->getRht()) {
                leaves.push_back(p);
                if(endTime != -1)
                    BOOST_CHECK_EQUAL(endTime, p->getTime());
                endTime = std::max(p->getTime(), endTime);
            }
        }

        // rigorous leaves --> root checks
        std::vector<double> rootTimes;
        std::stack<Node*> stack;
        std::map<Node*, double> timeMap;
        
        // prime the pump
        for(Node *p : leaves) {
            stack.push(p);
        }

        // this is just testing, so we're totally fine to overwrite the `time` variable 
        // in nodes here we'll do this to make things easier
        while(!stack.empty()) {
            Node *p = stack.top();
            stack.pop();

            // if we are root, make sure that our mapped value is 0
            if(p->getMajorAnc() == NULL) {
                BOOST_CHECK_EQUAL(p->getTime(), 0);
                if(timeMap.find(p) != timeMap.end())
                    BOOST_CHECK(timeMap[p] == 0);
                continue;
            }

            Node *majAnc = p->getMajorAnc();
            Node *minAnc = p->getMinorAnc();

            // check whether major anc times line up
            double newMajTime = p->getTime() - p->getMajorBranchLength();
            if(timeMap.find(majAnc) != timeMap.end()) {
                // if the mapping already exists
                BOOST_TEST(timeMap[majAnc] == newMajTime);
            } else {
                timeMap.insert(std::pair<Node*, double>(majAnc, newMajTime));
                // only push if we haven't seen the node yet
                stack.push(majAnc);
            }

            // check whether minor anc times line up
            if(minAnc != NULL) {
                double newMinTime = p->getTime() - p->getMinorBranchLength();
                if(timeMap.find(minAnc) != timeMap.end()) {
                    BOOST_TEST(timeMap[minAnc] == newMinTime);
                } else {
                    timeMap.insert(std::pair<Node*, double>(minAnc, newMinTime));
                    // only push if we haven't seen the node yet
                    stack.push(minAnc);
                }
            }
        }
    }

    for(Network *net : newicks)
        delete net;
}


