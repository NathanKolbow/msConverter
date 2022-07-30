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
    return fs::current_path().append("../../../newick-strings.txt").string();
}
inline std::string getNewickPairsFilePath(void) {
    return fs::current_path().append("../../../newick-string-pairs.txt").string();
}

inline std::vector<Network*> loadNewickNetworks(void) {
    std::ifstream is(getNewickFilePath());
    if(is.fail()) {
        std::cerr << "Failed to load newick-strings.txt; used path: " << getNewickFilePath() << std::endl;
        BOOST_TEST(!is.fail());
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
    if(is.fail()) {
        std::cerr << "Failed to load newick-strings.txt; used path: " << getNewickFilePath() << std::endl;
        BOOST_TEST(!is.fail());
    }

    std::vector<std::string> newicks;
    for(std::string str; std::getline(is, str);) {
        newicks.push_back(str);
    }
    BOOST_TEST(newicks.size() != 0);
    return newicks;
}

// loads paired newick strings from newick-string-pairs.txt
// used chiefly for testing against identical newicks that
// have failed to pass as isomorphic at some point in the past
inline std::vector<std::pair<std::string, std::string>> getNewickStringPairs(void) {
    std::ifstream is(getNewickPairsFilePath());
    if(is.fail()) {
        std::cerr << "Failed to load newick-string-pairs.txt; used path: " << getNewickPairsFilePath() << std::endl;
        BOOST_TEST(!is.fail());
    }

    std::vector<std::pair<std::string, std::string>> pairs;
    for(std::string str; std::getline(is, str);) {
        short _semicolon = 0;
        while(str[_semicolon] != ';' && str[_semicolon] != '\0')
            _semicolon++;
        
        if(str[_semicolon] == '\0') {
            std::cerr << "Reached end of string in newick-string-pairs.txt without a semi-colon." << std::endl;
            exit(-1);
        }

        pairs.push_back(std::pair<std::string, std::string>(std::string(str.substr(0, _semicolon+1)), std::string(str.substr(_semicolon+1, str.length() - _semicolon))));
    }

    return pairs;
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