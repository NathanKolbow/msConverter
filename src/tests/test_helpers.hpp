#ifndef MSCONVERTER_SRC_TESTS_TEST_HELPERS_HPP_
#define MSCONVERTER_SRC_TESTS_TEST_HELPERS_HPP_

#include <boost/test/unit_test.hpp>

#include "../core/Network.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>

namespace fs = std::filesystem;

inline std::string getNewickFilePath(void) {
    return std::string("C:\\Users\\Nathan\\OneDrive\\School\\Research\\Newick-to-ms\\newick-strings.txt");
}

inline std::vector<Network*> loadNewickNetworks(void) {
    std::ifstream is(getNewickFilePath());
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
    std::ifstream is(getNewickFilePath());
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

inline void argsOnly(std::string &msCmd) {
    int _dash = 0;
    int dashCount = 0;
    while(dashCount < 3 && msCmd[_dash] != '\0') {
        if(msCmd[_dash] == '-')
            dashCount++;
        _dash += 1;
    }

    if(msCmd[_dash] == '\0') {
        std::cerr << "FATAL ERROR: Looks like some string memory got mixed up somewhere along the line..." << std::endl;
        exit(-1);
    }

    msCmd.erase(0, _dash-1);
}

#endif