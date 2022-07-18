#ifndef SIMSUITE_SIMSUITE_HPP_
#define SIMSUITE_SIMSUITE_HPP_

#include <string>
#include <vector>

namespace SimSuite {
    std::string newickToMS(std::string newickStr);
    std::string msToNewick(std::string msStr);
    std::vector<std::string> newickFileToMS(std::string location);
    std::string msFromUserInputNewick(void);
    void disableWarnings(void);
}

#endif