#ifndef MSCONVERTER_SRC_TESTS_TEST_HELPERS_HPP_
#define MSCONVERTER_SRC_TESTS_TEST_HELPERS_HPP_

#include <boost/test/unit_test.hpp>

#include "../core/Network.hpp"

#include <string>
#include <vector>
#include <fstream>

inline std::vector<Network*> loadNewickNetworks(void) {
    std::ifstream is("../../../src/newick-strings.txt");
    BOOST_TEST(!is.fail());
    if(is.fail()) {
        std::cerr << "Failed to load newick-strings.txt." << std::endl;
        exit(-1);
    }

    std::vector<Network*> newicks;
    for(std::string str; std::getline(is, str);) {
        Network *net = new Network(str, "newick");
        net->makeUltrametric();
        newicks.push_back(net);
    }
    BOOST_TEST(newicks.size() != 0);
    return newicks;
}

inline std::vector<std::string> loadNewickStrings(void) {
    std::ifstream is("../../../src/newick-strings.txt");
    BOOST_TEST(!is.fail());
    if(is.fail()) {
        std::cerr << "Failed to load newick-strings.txt." << std::endl;
        exit(-1);
    }

    std::vector<std::string> newicks;
    for(std::string str; std::getline(is, str);) {
        newicks.push_back(str);
    }
    BOOST_TEST(newicks.size() != 0);
    return newicks;
}

#endif